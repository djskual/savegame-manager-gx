ifeq ($(strip $(DEVKITPPC)),)
$(error "Please set DEVKITPPC in your environment. export DEVKITPPC=<path to>devkitPPC")
endif

include $(DEVKITPPC)/wii_rules

LIBOGC_INC      :=      $(DEVKITPRO)/libogc/include
LIBOGC_LIB      :=      $(DEVKITPRO)/libogc/lib/wii
PACK_INC	:=	../libogc/include
PACK_LIB	:=	../libogc/lib/wii

CFLAGS  := -O2 $(MACHDEP) -I$(LIBOGC_INC) -DHAVE_LIBZ

LIB     := sevenzip
CFILES  := $(wildcard *.c)
OFILES  := $(CFILES:.c=.o)
ARC     := lib$(LIB).a
HDR     := 7zCrc.h 7zFile.h 7zIn.h 7zExtract.h 7zAlloc.h 7zHeader.h Types.h 7zItem.h 7zBuf.h 7zDecode.h 7zVersion.h Bcj2.h Bra.h CpuArch.h LzmaDec.h

all : $(OFILES)
		$(AR) -r $(ARC) $(OFILES)

clean :
		rm -f $(OFILES) $(ARC)

install :
		mkdir -p  $(LIBOGC_LIB)  $(LIBOGC_INC)/sevenzip
		cp -f $(ARC) $(LIBOGC_LIB)/
		cp -f $(HDR) $(LIBOGC_INC)/sevenzip/

package : 
		mkdir -p  $(PACK_LIB)  $(PACK_INC)/sevenzip
		cp -f $(ARC) $(PACK_LIB)/
		cp -f $(HDR) $(PACK_INC)/sevenzip/

%.o : %.c
		@echo $(notdir $<)
		@$(CC) $(CFLAGS) -c $< -o $@