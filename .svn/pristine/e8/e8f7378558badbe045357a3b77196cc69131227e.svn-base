# Quick'n'dirty makefile [BC] v2

ifeq ($(strip $(DEVKITPPC)),)
  $(error "Use   export DEVKITPPC=<path to>devkitPPC   and try again")
endif

ifeq ($(strip $(DEVKITPRO)),)
  $(error "Use   export DEVKITPRO=<path to>devkitPRO   and try again")
endif

PREFIX	:= $(DEVKITPPC)/bin/powerpc-eabi-
CC		:=	$(PREFIX)gcc
AR		:=	$(PREFIX)ar

LIBOGC_INC	:=	$(DEVKITPRO)/libogc/include
LIBOGC_LIB	:=	$(DEVKITPRO)/libogc/lib/wii
PACK_INC	:=	../libogc/include
PACK_LIB	:=	../libogc/lib/wii

MACHDEP := -DGEKKO -mrvl -mcpu=750 -meabi -mhard-float -DPNG_CONFIGURE_LIBPNG -DHAVE_CONFIG_H
CFLAGS  := -O4 -Wall $(MACHDEP) -I $(LIBOGC_INC)

LIB 	:= png
CFILES	:= $(wildcard *.c)
OFILES	:= $(CFILES:.c=.o)
ARC 	:= lib$(LIB).a
HDR 	:= $(LIB).h $(LIB)conf.h $(LIB)libconf.h

all : $(OFILES)
	$(AR) -r $(ARC) $(OFILES)

clean :
	rm -f $(OFILES) $(ARC)

install :
	mkdir -p  $(LIBOGC_LIB)  $(LIBOGC_INC)
	cp -f $(ARC) $(LIBOGC_LIB)/
	cp -f $(HDR) $(LIBOGC_INC)/

package : 
	mkdir -p  $(PACK_LIB)  $(PACK_INC)
	cp -f $(ARC) $(PACK_LIB)/
	cp -f $(HDR) $(PACK_INC)/

%.o : %.c
	@echo $(notdir $<)
	@$(CC) $(CFLAGS) -c $< -o $@
