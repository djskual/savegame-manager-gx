
default: wii-release

all: debug release

debug: cube-debug wii-debug

release: cube-release wii-release

cube-debug:
	$(MAKE) -C source PLATFORM=cube BUILD=cube_debug

wii-debug:
	$(MAKE) -C source  PLATFORM=wii BUILD=wii_debug

cube-release:
	$(MAKE) -C source  PLATFORM=cube BUILD=cube_release

wii-release:
	$(MAKE) -C source  PLATFORM=wii BUILD=wii_release

clean: 
	$(MAKE) -C source clean

install: cube-release wii-release
	$(MAKE) -C source install

wii-install: wii-release
	$(MAKE) -C source wii-install

package:
	$(MAKE) -C source package
