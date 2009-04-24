//===--- SourceLocation.h - Compact identifier for Source Files -*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
//  This file defines the SourceLocation class.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_SOURCELOCATION_H
#define LLVM_CLANG_SOURCELOCATION_H

#include <cassert>
#include "llvm/Bitcode/SerializationFwd.h"
#include <utility>

namespace llvm {
  class MemoryBuffer;
  template <typename T> struct DenseMapInfo;
}

namespace clang {
  
class SourceManager;
class FileEntry;
  
/// FileID - This is an opaque identifier used by SourceManager which refers to
/// a source file (MemoryBuffer) along with its #include path and #line data.
///
class FileID {
  /// ID - Opaque identifier, 0 is "invalid".
  unsigned ID;
public:
  FileID() : ID(0) {}
  
  bool isInvalid() const { return ID == 0; }
  
  bool operator==(const FileID &RHS) const { return ID == RHS.ID; }
  bool operator<(const FileID &RHS) const { return ID < RHS.ID; }
  bool operator<=(const FileID &RHS) const { return ID <= RHS.ID; }
  bool operator!=(const FileID &RHS) const { return !(*this == RHS); }
  bool operator>(const FileID &RHS) const { return RHS < *this; }
  bool operator>=(const FileID &RHS) const { return RHS <= *this; }
  
  static FileID getSentinel() { return get(~0U); }
  unsigned getHashValue() const { return ID; }
  
private:
  friend class SourceManager;
  static FileID get(unsigned V) {
    FileID F;
    F.ID = V;
    return F;
  }
  unsigned getOpaqueValue() const { return ID; }
};
  
    
/// SourceLocation - This is a carefully crafted 32-bit identifier that encodes
/// a full include stack, line and column number information for a position in
/// an input translation unit.
class SourceLocation {
  unsigned ID;
  friend class SourceManager;
  enum {
    MacroIDBit = 1U << 31
  };
public:

  SourceLocation() : ID(0) {}  // 0 is an invalid FileID.
  
  bool isFileID() const  { return (ID & MacroIDBit) == 0; }
  bool isMacroID() const { return (ID & MacroIDBit) != 0; }
  
  /// isValid - Return true if this is a valid SourceLocation object.  Invalid
  /// SourceLocations are often used when events have no corresponding location
  /// in the source (e.g. a diagnostic is required for a command line option).
  ///
  bool isValid() const { return ID != 0; }
  bool isInvalid() const { return ID == 0; }
  
private:
  /// getOffset - Return the index for SourceManager's SLocEntryTable table,
  /// note that this is not an index *into* it though.
  unsigned getOffset() const {
    return ID & ~MacroIDBit;
  }

  static SourceLocation getFileLoc(unsigned ID) {
    assert((ID & MacroIDBit) == 0 && "Ran out of source locations!");
    SourceLocation L;
    L.ID = ID;
    return L;
  }
  
  static SourceLocation getMacroLoc(unsigned ID) {
    assert((ID & MacroIDBit) == 0 && "Ran out of source locations!");
    SourceLocation L;
    L.ID = MacroIDBit | ID;
    return L;
  }
public:
  
  /// getFileLocWithOffset - Return a source location with the specified offset
  /// from this file SourceLocation.
  SourceLocation getFileLocWithOffset(int Offset) const {
    assert(((getOffset()+Offset) & MacroIDBit) == 0 && "invalid location");
    SourceLocation L;
    L.ID = ID+Offset;
    return L;
  }
  
  /// getRawEncoding - When a SourceLocation itself cannot be used, this returns
  /// an (opaque) 32-bit integer encoding for it.  This should only be passed
  /// to SourceLocation::getFromRawEncoding, it should not be inspected
  /// directly.
  unsigned getRawEncoding() const { return ID; }
  
  
  /// getFromRawEncoding - Turn a raw encoding of a SourceLocation object into
  /// a real SourceLocation.
  static SourceLocation getFromRawEncoding(unsigned Encoding) {
    SourceLocation X;
    X.ID = Encoding;
    return X;
  }
  
  /// Emit - Emit this SourceLocation object to Bitcode.
  void Emit(llvm::Serializer& S) const;
  
  /// ReadVal - Read a SourceLocation object from Bitcode.
  static SourceLocation ReadVal(llvm::Deserializer& D);
  
  void dump(const SourceManager &SM) const;
};

inline bool operator==(const SourceLocation &LHS, const SourceLocation &RHS) {
  return LHS.getRawEncoding() == RHS.getRawEncoding();
}

inline bool operator!=(const SourceLocation &LHS, const SourceLocation &RHS) {
  return !(LHS == RHS);
}
  
inline bool operator<(const SourceLocation &LHS, const SourceLocation &RHS) {
  return LHS.getRawEncoding() < RHS.getRawEncoding();
}

/// SourceRange - a trival tuple used to represent a source range.
class SourceRange {
  SourceLocation B;
  SourceLocation E;
public:
  SourceRange(): B(SourceLocation()), E(SourceLocation()) {}
  SourceRange(SourceLocation loc) : B(loc), E(loc) {}
  SourceRange(SourceLocation begin, SourceLocation end) : B(begin), E(end) {}
    
  SourceLocation getBegin() const { return B; }
  SourceLocation getEnd() const { return E; }
  
  void setBegin(SourceLocation b) { B = b; }
  void setEnd(SourceLocation e) { E = e; }
  
  bool isValid() const { return B.isValid() && E.isValid(); }
  
  /// Emit - Emit this SourceRange object to Bitcode.
  void Emit(llvm::Serializer& S) const;    

  /// ReadVal - Read a SourceRange object from Bitcode.
  static SourceRange ReadVal(llvm::Deserializer& D);
};
  
/// FullSourceLoc - A SourceLocation and its associated SourceManager.  Useful
/// for argument passing to functions that expect both objects.
class FullSourceLoc : public SourceLocation {
  SourceManager* SrcMgr;
public:
  /// Creates a FullSourceLoc where isValid() returns false.
  explicit FullSourceLoc() : SrcMgr((SourceManager*) 0) {}

  explicit FullSourceLoc(SourceLocation Loc, SourceManager &SM) 
    : SourceLocation(Loc), SrcMgr(&SM) {}
    
  SourceManager &getManager() {
    assert(SrcMgr && "SourceManager is NULL.");
    return *SrcMgr;
  }
  
  const SourceManager &getManager() const {
    assert(SrcMgr && "SourceManager is NULL.");
    return *SrcMgr;
  }
  
  FileID getFileID() const;
  
  FullSourceLoc getInstantiationLoc() const;
  FullSourceLoc getSpellingLoc() const;

  unsigned getLineNumber() const;
  unsigned getColumnNumber() const;
  
  unsigned getInstantiationLineNumber() const;
  unsigned getInstantiationColumnNumber() const;

  unsigned getSpellingLineNumber() const;
  unsigned getSpellingColumnNumber() const;

  const char *getCharacterData() const;
  
  const llvm::MemoryBuffer* getBuffer() const;
  
  /// getBufferData - Return a pointer to the start and end of the source buffer
  /// data for the specified FileID.
  std::pair<const char*, const char*> getBufferData() const;
  
  bool isInSystemHeader() const;
  
  /// Prints information about this FullSourceLoc to stderr. Useful for
  ///  debugging.
  void dump() const { SourceLocation::dump(*SrcMgr); }

  friend inline bool 
  operator==(const FullSourceLoc &LHS, const FullSourceLoc &RHS) {
    return LHS.getRawEncoding() == RHS.getRawEncoding() &&
          LHS.SrcMgr == RHS.SrcMgr;
  }

  friend inline bool 
  operator!=(const FullSourceLoc &LHS, const FullSourceLoc &RHS) {
    return !(LHS == RHS);
  }

};
  
/// PresumedLoc - This class represents an unpacked "presumed" location which
/// can be presented to the user.  A 'presumed' location can be modified by
/// #line and GNU line marker directives and is always the instantiation point
/// of a normal location.
///
/// You can get a PresumedLoc from a SourceLocation with SourceManager.
class PresumedLoc {
  const char *Filename;
  unsigned Line, Col;
  SourceLocation IncludeLoc;
public:
  PresumedLoc() : Filename(0) {}
  PresumedLoc(const char *FN, unsigned Ln, unsigned Co, SourceLocation IL)
    : Filename(FN), Line(Ln), Col(Co), IncludeLoc(IL) {
  }
  
  /// isInvalid - Return true if this object is invalid or uninitialized. This
  /// occurs when created with invalid source locations or when walking off
  /// the top of a #include stack.
  bool isInvalid() const { return Filename == 0; }
  bool isValid() const { return Filename != 0; }
  
  /// getFilename - Return the presumed filename of this location.  This can be
  /// affected by #line etc.
  const char *getFilename() const { return Filename; }

  /// getLine - Return the presumed line number of this location.  This can be
  /// affected by #line etc.
  unsigned getLine() const { return Line; }
  
  /// getColumn - Return the presumed column number of this location.  This can
  /// not be affected by #line, but is packaged here for convenience.
  unsigned getColumn() const { return Col; }

  /// getIncludeLoc - Return the presumed include location of this location.
  /// This can be affected by GNU linemarker directives.
  SourceLocation getIncludeLoc() const { return IncludeLoc; }
};

  
}  // end namespace clang

namespace llvm {
  /// Define DenseMapInfo so that FileID's can be used as keys in DenseMap and
  /// DenseSets.
  template <>
  struct DenseMapInfo<clang::FileID> {
    static inline clang::FileID getEmptyKey() {
      return clang::FileID();
    }
    static inline clang::FileID getTombstoneKey() {
      return clang::FileID::getSentinel(); 
    }
    
    static unsigned getHashValue(clang::FileID S) {
      return S.getHashValue();
    }
    
    static bool isEqual(clang::FileID LHS, clang::FileID RHS) {
      return LHS == RHS;
    }
    
    static bool isPod() { return true; }
  };
  
}  // end namespace llvm

#endif
