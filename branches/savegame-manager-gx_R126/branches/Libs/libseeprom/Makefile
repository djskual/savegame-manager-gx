# Quick'n'dirty makefile [BC] v2

ifeq ($(strip $(DEVKITPPC)),)
$(error "Please set DEVKITPPC in your environment. export DEVKITPPC=<path to>devkitPPC")
endif

include $(DEVKITPPC)/wii_rules

LIBOGC_INC	:=	$(DEVKITPRO)/libogc/include
LIBOGC_LIB	:=	$(DEVKITPRO)/libogc/lib/wii
PACK_INC	:=	../libogc/include
PACK_LIB	:=	../libogc/lib/wii


CFLAGS  := -O4 $(MACHDEP) -I$(LIBOGC_INC) -DHAVE_CONFIG_H

LIB     := seeprom
CFILES  := $(wildcard *.c)
OFILES  := $(CFILES:.c=.o)
ARC     := lib$(LIB).a
HDR     := seeprom.h mini_seeprom.h

all : $(OFILES)
		$(AR) -r $(ARC) $(OFILES)

clean :
		rm -f $(OFILES) $(ARC)

install :
		mkdir -p  $(LIBOGC_LIB)  $(LIBOGC_INC)/seeprom
		cp -f $(ARC) $(LIBOGC_LIB)/
		cp -f $(HDR) $(LIBOGC_INC)/seeprom/

package :
		mkdir -p  $(PACK_LIB)  $(PACK_INC)/seeprom
		cp -f $(ARC) $(PACK_LIB)/
		cp -f $(HDR) $(PACK_INC)/seeprom/

%.o : %.c
		@echo $(notdir $<)
		@$(CC) $(CFLAGS) -c $< -o $@