ifeq ($(strip $(DEVKITPRO)),)
$(error "Please set DEVKITPRO in your environment. export DEVKITPRO=<path to>devkitPro)
endif
 
export TOPDIR	:=	$(CURDIR)
 
default:
	$(MAKE) -C libogc PLATFORM=wii BUILD=wii_release

clean:
	$(MAKE) -C libogc clean

install:
	$(MAKE) -C libogc install

package:
	$(MAKE) -C libogc package
