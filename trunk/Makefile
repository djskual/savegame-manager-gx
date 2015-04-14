#---------------------------------------------------------------------------------
# Clear the implicit built in rules
#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------
ifeq ($(strip $(DEVKITPPC)),)
$(error "Please set DEVKITPPC in your environment. export DEVKITPPC=<path to>devkitPPC")
endif

include $(DEVKITPPC)/wii_rules

#---------------------------------------------------------------------------------
# TARGET is the name of the output
# BUILD is the directory where object files & intermediate files will be placed
# SOURCES is a list of directories containing source code
# INCLUDES is a list of directories containing extra header files
#---------------------------------------------------------------------------------
TARGET		:=	boot
BUILD		:=	build
SOURCES		:=	source \
				source/ArchiveOperations \
				source/BootHomebrew \
				source/Controls \
				source/DeviceControls \
				source/EmuNand \
				source/FileOperations \
				source/Files \
				source/GC \
				source/GUI \
				source/ImageOperations \
				source/Language \
				source/Manage \
				source/ManageAll \
				source/Memory \
				source/Menu \
				source/Miis \
				source/Mload \
				source/Mload/modules \
				source/Network \
				source/Prompts \
				source/Saves \
				source/Saves/DataBin \
				source/Saves/DataBin/Crypto \
				source/Saves/WiiSave \
				source/Settings \
				source/Settings/menus \
				source/SoundOperations \
				source/System \
				source/TextOperations \
				source/Themes \
				source/Tools \
				source/XML
INCLUDES	:=	source
DATA		:=	data/binary \
				data/fonts \
				data/images \
				data/sounds

#---------------------------------------------------------------------------------
# options for code generation
#---------------------------------------------------------------------------------

CFLAGS		=	-g -O3 -Wall -Wno-multichar -Wno-unused-parameter -Wextra $(MACHDEP) $(INCLUDE)
CXXFLAGS	=	-save-temps -Xassembler -aln=$@.lst $(CFLAGS)
LDFLAGS		=	-g $(MACHDEP) -Wl,-Map,$(notdir $@).map,-wrap,malloc,-wrap,free,-wrap,memalign,-wrap,calloc,-wrap,realloc,-wrap,malloc_usable_size

#---------------------------------------------------------------------------------
# any extra libraries we wish to link with the project
#---------------------------------------------------------------------------------
LIBS := -ldb -lzip -lunrar -lsevenzip -lgd -ltiff -ljpeg -lpng -lz -lasnd -lwiiuse -lbte \
		-lmad -lseeprom -lotp -logc -lvorbisidec -lfreetype -lfat -lntfs -lext2fs \
		-luneekfs

#---------------------------------------------------------------------------------
# list of directories containing libraries, this must be the top level containing
# include and lib
#---------------------------------------------------------------------------------
LIBDIRS	:= $(DEVKITPPC)/lib $(CURDIR)

#---------------------------------------------------------------------------------
# no real need to edit anything past this point unless you need to add additional
# rules for different file extensions
#---------------------------------------------------------------------------------
ifneq ($(BUILD),$(notdir $(CURDIR)))
#---------------------------------------------------------------------------------
export PROJECTDIR 	:= $(CURDIR)
export OUTPUT		:=	$(CURDIR)/$(TARGETDIR)/$(TARGET)
export VPATH		:=	$(foreach dir,$(SOURCES),$(CURDIR)/$(dir)) \
						$(foreach dir,$(DATA),$(CURDIR)/$(dir))
export DEPSDIR		:=	$(CURDIR)/$(BUILD)

#---------------------------------------------------------------------------------
# automatically build a list of object files for our project
#---------------------------------------------------------------------------------
SVNREV			:=	$(shell bash ./svnrev.sh)
IMPORTFILES		:=  $(shell bash ./filelist.sh)
export CFILES	:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
export CPPFILES	:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
sFILES			:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))
SFILES			:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.S)))
BINFILES		:=	$(foreach dir,$(DATA),$(notdir $(wildcard $(dir)/*.bin)))
TTFFILES		:=	$(foreach dir,$(DATA),$(notdir $(wildcard $(dir)/*.ttf)))
PNGFILES		:=	$(foreach dir,$(DATA),$(notdir $(wildcard $(dir)/*.png)))
OGGFILES		:=	$(foreach dir,$(DATA),$(notdir $(wildcard $(dir)/*.ogg)))
WAVFILES		:=	$(foreach dir,$(DATA),$(notdir $(wildcard $(dir)/*.wav)))
DOLFILES		:=	$(foreach dir,$(DATA),$(notdir $(wildcard $(dir)/*.dol)))
MP3FILES		:=	$(foreach dir,$(DATA),$(notdir $(wildcard $(dir)/*.mp3)))
PCMFILES		:=	$(foreach dir,$(DATA),$(notdir $(wildcard $(dir)/*.pcm)))
DATFILES		:=	$(foreach dir,$(DATA),$(notdir $(wildcard $(dir)/*.dat)))

#---------------------------------------------------------------------------------
# use CXX for linking C++ projects, CC for standard C
#---------------------------------------------------------------------------------
ifeq ($(strip $(CPPFILES)),)
	export LD	:=	$(CC)
else
	export LD	:=	$(CXX)
endif

export OFILES	:=	$(CPPFILES:.cpp=.o) $(CFILES:.c=.o) \
					$(sFILES:.s=.o) $(SFILES:.S=.o) \
					$(TTFFILES:.ttf=.ttf.o) $(PNGFILES:.png=.png.o) \
					$(OGGFILES:.ogg=.ogg.o) $(PCMFILES:.pcm=.pcm.o) \
					$(WAVFILES:.wav=.wav.o) $(BINFILES:.bin=.bin.o) \
					$(DATFILES:.dat=.dat.o) \
					$(addsuffix .o,$(ELFFILES)) $(addsuffix .o,$(DOLFILES)) \
					$(CURDIR)/data/binary/magic_patcher.o

#---------------------------------------------------------------------------------
# build a list of include paths
#---------------------------------------------------------------------------------
export INCLUDE	:=	$(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) \
					$(foreach dir,$(LIBDIRS),-I$(dir)/include) \
					-I$(CURDIR)/$(BUILD) \
					-I$(LIBOGC_INC)

#---------------------------------------------------------------------------------
# build a list of library paths
#---------------------------------------------------------------------------------
export LIBPATHS	:=	$(foreach dir,$(LIBDIRS),-L$(dir)/lib) \
					-L$(LIBOGC_LIB)

export OUTPUT	:=	$(CURDIR)/$(TARGET)
.PHONY: $(BUILD) clean release

#---------------------------------------------------------------------------------
$(BUILD):
	@[ -d $@ ] || mkdir -p $@
	@$(MAKE) --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile

#---------------------------------------------------------------------------------
release:
	@ rm -fr $(BUILD) $(OUTPUT).elf $(OUTPUT).dol $(CURDIR)/source/Network/svnrev.c \
			$(CURDIR)/HBC/meta.xml $(CURDIR)/source/Themes/filelist.h
	@ make --no-print-directory
	@ ./release.sh

#---------------------------------------------------------------------------------
clean:
	@rm -fr $(BUILD) $(OUTPUT).elf $(OUTPUT).dol $(CURDIR)/source/Network/svnrev.c \
			$(CURDIR)/HBC/meta.xml $(CURDIR)/source/Themes/filelist.h

#---------------------------------------------------------------------------------
run:
	wiiload $(OUTPUT).dol

#---------------------------------------------------------------------------------
reload:
	wiiload -r $(OUTPUT).dol

#---------------------------------------------------------------------------------
else

DEPENDS	:=	$(OFILES:.o=.d)

#---------------------------------------------------------------------------------
# main targets
#---------------------------------------------------------------------------------
$(OUTPUT).dol: $(OUTPUT).elf language theme
$(OUTPUT).elf: $(OFILES)

language: $(wildcard $(PROJECTDIR)/Languages/*.lang)
theme: $(wildcard $(PROJECTDIR)/Themes/*.them)
#---------------------------------------------------------------------------------
# This rule links in binary data with .ttf, .png, and .mp3 extensions
#---------------------------------------------------------------------------------
%.dol.o : %.dol
	@echo $(notdir $<)
	@bin2s -a 32 $< | $(AS) -o $(@)

%.ttf.o : %.ttf
	@echo $(notdir $<)
	@bin2s -a 32 $< | $(AS) -o $(@)

%.png.o : %.png
	@echo $(notdir $<)
	@bin2s -a 32 $< | $(AS) -o $(@)

%.ogg.o : %.ogg
	@echo $(notdir $<)
	@bin2s -a 32 $< | $(AS) -o $(@)

%.wav.o : %.wav
	@echo $(notdir $<)
	@bin2s -a 32 $< | $(AS) -o $(@)

%.bin.o	:	%.bin
	@echo $(notdir $<)
	@bin2s -a 32 $< | $(AS) -o $(@)

%.dat.o	:	%.dat
	@echo $(notdir $<)
	@bin2s -a 32 $< | $(AS) -o $(@)

export PATH		:=	$(PROJECTDIR)/gettext-bin:$(PATH)

%.pot: $(CFILES) $(CPPFILES)
	@echo Updating Languagefiles ...
	@touch $(PROJECTDIR)/Languages/$(TARGET).pot
	@xgettext -C -cTRANSLATORS --from-code=utf-8 --sort-output --no-wrap --no-location -ktr -ktrNOOP -o$(PROJECTDIR)/Languages/$(TARGET).pot -p $@ $^
	@echo Updating Themefiles ...
	@touch $(PROJECTDIR)/Themes/$(TARGET).pot
	@xgettext -C -cTRANSLATORS --from-code=utf-8 -F --no-wrap --add-location -kthInt -kthColor -kthAlign -o$(PROJECTDIR)/Themes/$(TARGET).pot -p $@ $^

%.lang: $(PROJECTDIR)/Languages/$(TARGET).pot
	@msgmerge -U -N --no-wrap --no-location --backup=none -q $@ $<
	@touch $@

%.them: $(PROJECTDIR)/Themes/$(TARGET).pot
	@msgmerge -U -N --no-wrap --no-location --backup=none -q $@ $<
	@touch $@

-include $(DEPENDS)

#---------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------
