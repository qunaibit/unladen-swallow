# Generated by h2py from /usr/include/netinet/in.h

# Included from net/nh.h

# Included from sys/machine.h
LITTLE_ENDIAN = 1234
BIG_ENDIAN = 4321
PDP_ENDIAN = 3412
BYTE_ORDER = BIG_ENDIAN
DEFAULT_GPR = 0xDEADBEEF
MSR_EE = 0x8000
MSR_PR = 0x4000
MSR_FP = 0x2000
MSR_ME = 0x1000
MSR_FE = 0x0800
MSR_FE0 = 0x0800
MSR_SE = 0x0400
MSR_BE = 0x0200
MSR_IE = 0x0100
MSR_FE1 = 0x0100
MSR_AL = 0x0080
MSR_IP = 0x0040
MSR_IR = 0x0020
MSR_DR = 0x0010
MSR_PM = 0x0004
DEFAULT_MSR = (MSR_EE | MSR_ME | MSR_AL | MSR_IR | MSR_DR)
DEFAULT_USER_MSR = (DEFAULT_MSR | MSR_PR)
CR_LT = 0x80000000
CR_GT = 0x40000000
CR_EQ = 0x20000000
CR_SO = 0x10000000
CR_FX = 0x08000000
CR_FEX = 0x04000000
CR_VX = 0x02000000
CR_OX = 0x01000000
XER_SO = 0x80000000
XER_OV = 0x40000000
XER_CA = 0x20000000
def XER_COMP_BYTE(xer): return ((xer >> 8) & 0x000000FF)

def XER_LENGTH(xer): return (xer & 0x0000007F)

DSISR_IO = 0x80000000
DSISR_PFT = 0x40000000
DSISR_LOCK = 0x20000000
DSISR_FPIO = 0x10000000
DSISR_PROT = 0x08000000
DSISR_LOOP = 0x04000000
DSISR_DRST = 0x04000000
DSISR_ST = 0x02000000
DSISR_SEGB = 0x01000000
DSISR_DABR = 0x00400000
DSISR_EAR = 0x00100000
SRR_IS_PFT = 0x40000000
SRR_IS_ISPEC = 0x20000000
SRR_IS_IIO = 0x10000000
SRR_IS_PROT = 0x08000000
SRR_IS_LOOP = 0x04000000
SRR_PR_FPEN = 0x00100000
SRR_PR_INVAL = 0x00080000
SRR_PR_PRIV = 0x00040000
SRR_PR_TRAP = 0x00020000
SRR_PR_IMPRE = 0x00010000
def ntohl(x): return (x)

def ntohs(x): return (x)

def htonl(x): return (x)

def htons(x): return (x)

IPPROTO_IP = 0
IPPROTO_ICMP = 1
IPPROTO_GGP = 3
IPPROTO_TCP = 6
IPPROTO_EGP = 8
IPPROTO_PUP = 12
IPPROTO_UDP = 17
IPPROTO_IDP = 22
IPPROTO_TP = 29
IPPROTO_LOCAL = 63
IPPROTO_EON = 80
IPPROTO_BIP = 0x53
IPPROTO_RAW = 255
IPPROTO_MAX = 256
IPPORT_RESERVED = 1024
IPPORT_USERRESERVED = 5000
IPPORT_TIMESERVER = 37
def IN_CLASSA(i): return (((long)(i) & 0x80000000) == 0)

IN_CLASSA_NET = 0xff000000
IN_CLASSA_NSHIFT = 24
IN_CLASSA_HOST = 0x00ffffff
IN_CLASSA_MAX = 128
def IN_CLASSB(i): return (((long)(i) & 0xc0000000) == 0x80000000)

IN_CLASSB_NET = 0xffff0000
IN_CLASSB_NSHIFT = 16
IN_CLASSB_HOST = 0x0000ffff
IN_CLASSB_MAX = 65536
def IN_CLASSC(i): return (((long)(i) & 0xe0000000) == 0xc0000000)

IN_CLASSC_NET = 0xffffff00
IN_CLASSC_NSHIFT = 8
IN_CLASSC_HOST = 0x000000ff
def IN_CLASSD(i): return (((long)(i) & 0xf0000000) == 0xe0000000)

def IN_MULTICAST(i): return IN_CLASSD(i)

def IN_EXPERIMENTAL(i): return (((long)(i) & 0xe0000000) == 0xe0000000)

def IN_BADCLASS(i): return (((long)(i) & 0xf0000000) == 0xf0000000)

INADDR_ANY = 0x00000000
INADDR_LOOPBACK = 0x7f000001
INADDR_BROADCAST = 0xffffffff
INADDR_NONE = 0xffffffff
IN_LOOPBACKNET = 127
IP_OPTIONS = 1
IP_HDRINCL = 2
IP_TOS = 3
IP_TTL = 4
IP_RECVOPTS = 5
IP_RECVRETOPTS = 6
IP_RECVDSTADDR = 7
IP_RETOPTS = 8