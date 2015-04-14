
ifeq ($(strip $(DEVKITPPC)),)
  $(error "Use   export DEVKITPPC=<path to>devkitPPC   and try again")
endif

include $(DEVKITPPC)/wii_rules

LIBOGC_INC	:=	$(DEVKITPRO)/libogc/include
LIBOGC_LIB	:=	$(DEVKITPRO)/libogc/lib/wii
PACK_INC	:=	../libogc/include
PACK_LIB	:=	../libogc/lib/wii

MACHDEP := -DGEKKO -mrvl -mcpu=750 -meabi -mhard-float
CPPFLAGS  := $(MACHDEP)

PREFIX	:= $(DEVKITPPC)/bin/powerpc-eabi-
CC		:=	$(PREFIX)g++
AR		:=	$(PREFIX)ar
CXX=$(CC)
CXXFLAGS=-O3 -DBIG_ENDIAN -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE -LANG:std -DSILENT
LDFLAGS=
DESTDIR=/


##########################

COMPILE=$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(DEFINES)
LINK=$(CXX)

WHAT=UNRAR

UNRAR_OBJ=filestr.o recvol.o rs.o scantree.o
LIB_OBJ=filestr.o scantree.o dll.o

OBJECTS=rar.o strlist.o strfn.o pathfn.o savepos.o smallfn.o global.o file.o filefn.o filcreat.o \
	archive.o arcread.o unicode.o system.o isnt.o crypt.o crc.o rawread.o encname.o \
	resource.o match.o timefn.o rdwrfn.o consio.o options.o ulinks.o errhnd.o rarvm.o \
	rijndael.o getbits.o sha1.o extinfo.o extract.o volume.o list.o find.o unpack.o cmddata.o

LIB     := unrar
ARC     := lib$(LIB).a
HDR     := archive.hpp array.hpp cmddata.hpp coder.hpp compress.hpp consio.hpp crc.hpp crypt.hpp \
			dll.hpp encname.hpp errhnd.hpp extinfo.hpp extract.hpp filcreat.hpp file.hpp filefn.hpp \
			filestr.hpp find.hpp getbits.hpp global.hpp headers.hpp isnt.hpp list.hpp loclang.hpp \
			log.hpp match.hpp model.hpp options.hpp os.hpp pathfn.hpp rar.hpp rardefs.hpp rarlang.hpp \
			raros.hpp rartypes.hpp rarvm.hpp rawread.hpp rdwrfn.hpp recvol.hpp resource.hpp rijndael.hpp \
			rs.hpp savepos.hpp scantree.hpp sha1.hpp smallfn.hpp strfn.hpp strlist.hpp suballoc.hpp \
			system.hpp timefn.hpp ulinks.hpp unicode.hpp unpack.hpp version.hpp volume.hpp

.cpp.o:
	$(COMPILE) -D$(WHAT) -c $<

all:	unrar

clean:
	@rm -f *.o *.bak *.a *~

install :
		mkdir -p  $(LIBOGC_LIB)  $(LIBOGC_INC)/libunrar
		cp -f $(ARC) $(LIBOGC_LIB)/
		cp -f $(HDR) $(LIBOGC_INC)/libunrar/

package : 
		mkdir -p  $(PACK_LIB)  $(PACK_INC)/libunrar
		cp -f $(ARC) $(PACK_LIB)/
		cp -f $(HDR) $(PACK_INC)/libunrar/

unrar:	$(OBJECTS) $(UNRAR_OBJ)
	@rm -f unrar
	$(AR) -r -o libunrar.a $(LDFLAGS) $(OBJECTS) $(UNRAR_OBJ)