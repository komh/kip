EXE_NAME   = kip.exe
SRC_LIST   = kip.c
A_LIST     =
LIB_LIST   = kiphook.lib

.INCLUDE : "makefile.inc"

kip.obj : kip.h

kiphook.lib : kiphook.dll kiphook.def
    $(IMP) -o $@ kiphook.def

kiphook.dll : kiphook.obj kiphook.def
    $(CC) $(LFDLL) $&

kiphook.obj : kiphook.h
