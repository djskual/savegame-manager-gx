ifeq ($(strip $(DEVKITPPC)),)
$(error "Please set DEVKITPPC in your environment. export DEVKITPPC=<path to>devkitPPC")
endif

include $(DEVKITPPC)/wii_rules

LIBOGC_INC      :=      $(DEVKITPRO)/libogc/include
LIBOGC_LIB      :=      $(DEVKITPRO)/libogc/lib/wii

CFLAGS  := -O2 $(MACHDEP) -I$(LIBOGC_INC) -DHAVE_LIBZ

LIB     := zip
CFILES  := $(wildcard *.c)
OFILES  := $(CFILES:.c=.o)
ARC     := lib$(LIB).a
HDR     := unzip.h ioapi.h zip.h

all : $(OFILES)
		$(AR) -r $(ARC) $(OFILES)

clean :
		rm -f $(OFILES) $(ARC)

install :
		mkdir -p  $(LIBOGC_LIB)  $(LIBOGC_INC)/zip
		cp -f $(ARC) $(LIBOGC_LIB)/
		cp -f $(HDR) $(LIBOGC_INC)/zip/

%.o : %.c
		$(CC) $(CFLAGS) -c $< -o $@