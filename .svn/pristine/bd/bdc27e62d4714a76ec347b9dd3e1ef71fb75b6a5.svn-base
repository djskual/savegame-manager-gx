ifeq ($(strip $(DEVKITPPC)),)
$(error "Please set DEVKITPPC in your environment. export DEVKITPPC=<path to>devkitPPC")
endif

include $(DEVKITPPC)/wii_rules

LIBOGC_INC      :=      $(DEVKITPRO)/libogc/include
LIBOGC_LIB      :=      $(DEVKITPRO)/libogc/lib/wii

CFLAGS  := -O2 $(MACHDEP) -I$(LIBOGC_INC) -DHAVE_CONFIG_H

LIB     := gd
CFILES  := $(wildcard *.c)
OFILES  := $(CFILES:.c=.o)
ARC     := lib$(LIB).a
HDR     := gd.h gd_io.h gdfx.h

all : $(OFILES)
		$(AR) -r $(ARC) $(OFILES)

clean :
		rm -f $(OFILES) $(ARC)

install :
		mkdir -p  $(LIBOGC_LIB)  $(LIBOGC_INC)
		cp -f $(ARC) $(LIBOGC_LIB)/
		cp -f $(HDR) $(LIBOGC_INC)/

%.o : %.c
		$(CC) $(CFLAGS) -c $< -o $@