#! /bin/bash
#
rev_new_raw=$(svnversion -n . 2>/dev/null | tr '\n' ' ' | tr -d '\r')
[ -n "$rev_new_raw" ] || rev_new_raw=$(SubWCRev . 2>/dev/null | tr '\n' ' ' | tr -d '\r')

date=$(date "+%Y%m%d")

rev_new_raw=$(echo $rev_new_raw | sed 's/[^0-9]*\([0-9]*\)\(.*\)/\1 \2/')
rev_new=0
a=$(echo $rev_new_raw | sed 's/\([0-9]*\).*/\1/')
let "a+=0"
#find max rev
while [ "$a" ]; do
	[ "$a" -gt "$rev_new" ] && rev_new=$a
	rev_new_raw=$(echo -n $rev_new_raw | sed 's/[0-9]*[^0-9]*\([0-9]*\)\(.*\)/\1 \2/')
	a=$(echo $rev_new_raw | sed 's/\([0-9]*\).*/\1/') 
done

rev_old=$(cat ./source/network/svnrev.c 2>/dev/null | tr -d '\n' | sed 's/[^0-9]*\([0-9]*\).*/\1/')

if [ "$rev_new" != "$rev_old" ] || [ ! -f ./source/network/svnrev.c ]; then
	
	cat <<EOF > ./source/network/svnrev.c
#define SVN_REV "$rev_new"

const char *SvnRev()
{
	return SVN_REV;
}
EOF

	rev_new=`expr $rev_new + 1`
	rev_date=`date +%Y%m%d%H%M -u`
	
	cat <<EOF > ./HBC/meta.xml
<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<app version="1">
  <name>SaveGame Manager GX</name>
  <coder>Dj_Skual</coder>
  <version>r$rev_new</version>
  <release_date>$rev_date</release_date>
  <short_description>SaveGames/Mii Manager</short_description> 
  <long_description>A Savegame/Mii extractor and installer for the Wii by Dj_Skual.
The GUI is done with LibWiiGui by Tantric and graphics by NeoRame/Cyrex from USB Loader GX.

Features:
 * SDHC and USB2 support
 * Protected save support
 * Mii Support
 * Multilanguage support with Custom Font
 * WiiTDB support
 * Display information about savegame in a special windows
   (Name, Subname, ID, BlockSize, Icon)
 * Extract, Install, Delete, Copy, Move fonctions are available
 * Custom Background Music support (AIF/MP3/OGG/BNS/WAV)
 * Online AutoUpdate
 * Widesreen support
 * ScreenSaver

Credits:
  Coding: Dj_Skual, Giantpune
  Artworks: Neorame, Cyrex, Dj_Skual
  Languages: Dj Skual and Translaters

  Libwiigui: Tantric
  Libogc/Devkit: Shagkur and Wintermute
  FreeTypeGX: Armin Tamzarian.
  
Thanks to:
  Waninkoko for SaveGame Manager sources
  USB Loader GX Team for sources and artworks
  WiiXplorer Team for WiiXplorer sources
  Lustar for WiiTDB (Wii Title DataBase)

Links:
  SaveGame Manager GX Project Page:
    http://code.google.com/p/
	savegame-manager-gx/
  WiiXplorer Support Site:
    http://code.google.com/p/
	savegame-manager-gx/updates/list
  USB Loader GX Support Site:
    http://code.google.com/p/
	usbloader-gui/updates/list
  Libwiigui Website:
    http://wiibrew.org/wiki/Libwiigui/
  WiiTDB Website:
    http://wiitdb.com/Main/HomePage
  FreeTypeGX Project Page:
    http://code.google.com/p/freetypegx/
  Gettext Official Page:
    http://www.gnu.org/software/gettext/
    gettext.html
  </long_description> 
</app>
EOF

fi
echo "$rev_new"