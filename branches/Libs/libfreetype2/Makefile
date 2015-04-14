# Quick'n'dirty makefile [BC]

ifeq ($(strip $(DEVKITPPC)),)
  $(error "Use   export DEVKITPPC=<path to>devkitPPC   and try again")
endif

ifeq ($(strip $(DEVKITPRO)),)
  $(error "Use   export DEVKITPRO=<path to>devkitPRO   and try again")
endif

# FreeType Makefile
PROJECT       := freetype
PROJECT_TITLE := FreeType
TOP_DIR ?= .
OBJ_DIR ?= $(TOP_DIR)
include $(TOP_DIR)/builds/toplevel.mk


LIBOGC_INC	:=	$(DEVKITPRO)/libogc/include
LIBOGC_LIB	:=	$(DEVKITPRO)/libogc/lib/wii
PACK_INC	:=	../libogc/include
PACK_LIB	:=	../libogc/lib/wii

clean :
	rm -f lib$(PROJECT).a ftmodule.h

install :
	mkdir -p  $(LIBOGC_LIB)  $(LIBOGC_INC)/freetype/config/
	cp -rf  $(shell ls include/*.h)                     $(LIBOGC_INC)/
	cp -rf  $(shell ls include/freetype/*.h)            $(LIBOGC_INC)/freetype/
	cp -rf  $(shell ls include/freetype/config/*.h)     $(LIBOGC_INC)/freetype/config/
	cp -rf  lib$(PROJECT).a                             $(LIBOGC_LIB)/

package : 
	mkdir -p  $(PACK_LIB)  $(PACK_INC)/freetype/config/
	cp -rf  $(shell ls include/*.h)                     $(PACK_INC)/
	cp -rf  $(shell ls include/freetype/*.h)            $(PACK_INC)/freetype/
	cp -rf  $(shell ls include/freetype/config/*.h)     $(PACK_INC)/freetype/config/
	cp -rf  lib$(PROJECT).a                             $(PACK_LIB)/