#! /bin/bash
#

while test -z $DEVKITPRO_REV
do
	echo "DevkitPro rev ?"
	read DEVKITPRO_REV
done
while test -z $LIBOGC_REV
do
	echo "Libogc rev ?"
	read LIBOGC_REV
done

cd libfreetype2/ 
make
make install
make package
make clean

cd ../zlib/
make
make install
make package
make clean

cd ../libfat/
make
make install
make package
make clean

cd ../libzip/
make
make install
make package
make clean

cd ../lib7zip/
make
make install
make package
make clean

cd ../libunrar/
make
make install
make package
make clean

cd ../libjpeg/
make
make install
make package
make clean

cd ../libpng/
make
make install
make package
make clean

cd ../libtiff/
make
make install
make package
make clean

cd ../libgd/
make
make install
make package
make clean

cd ../libext2fs/
make wii-install
make package
make clean

cd ../libntfs/
make
make install
make package
make clean

cd ../libseeprom/
make
make install
make package
make clean

cd ../libotp/
make
make install
make package
make clean

cd ../libuneekfs/
make
make install
make package
make clean

cd ../tremor-lowmem/

INC_TREMOR=include/tremor

#install
mkdir -p $DEVKITPRO/libogc/$INC_TREMOR	$DEVKITPRO/libogc/lib/wii
cp -f $INC_TREMOR/config_types.h		$DEVKITPRO/libogc/$INC_TREMOR/
cp -f $INC_TREMOR/ivorbiscodec.h		$DEVKITPRO/libogc/$INC_TREMOR/
cp -f $INC_TREMOR/ivorbisfile.h			$DEVKITPRO/libogc/$INC_TREMOR/
cp -f $INC_TREMOR/ogg.h					$DEVKITPRO/libogc/$INC_TREMOR/
cp -f $INC_TREMOR/os_types.h			$DEVKITPRO/libogc/$INC_TREMOR/
cp -f lib/libvorbisidec.a				$DEVKITPRO/libogc/lib/wii/

#package
mkdir -p ../libogc/$INC_TREMOR		../libogc/lib/wii
cp -f $INC_TREMOR/config_types.h	../libogc/$INC_TREMOR/
cp -f $INC_TREMOR/ivorbiscodec.h	../libogc/$INC_TREMOR/
cp -f $INC_TREMOR/ivorbisfile.h		../libogc/$INC_TREMOR/
cp -f $INC_TREMOR/ogg.h				../libogc/$INC_TREMOR/
cp -f $INC_TREMOR/os_types.h		../libogc/$INC_TREMOR/
cp -f lib/libvorbisidec.a			../libogc/lib/wii/

cd ../libogc/
touch readme_tmp.txt
echo "Compiled for DevkitPro r$DEVKITPRO_REV" >> readme_tmp.txt
echo "" >> readme_tmp.txt
echo "libogc rev $LIBOGC_REV" >> readme_tmp.txt
echo "zlib 1.2.7" >> readme_tmp.txt
echo "tremor lowmem" >> readme_tmp.txt
echo "libpng 1.5.13" >> readme_tmp.txt
echo "libfat 1.0.11" >> readme_tmp.txt
echo "libzip 1.01e" >> readme_tmp.txt
echo "lib7zip 4.65" >> readme_tmp.txt
echo "libunrar 3.91.0" >> readme_tmp.txt
echo "libfreetype 2.4.3" >> readme_tmp.txt
echo "libjpeg 8d" >> readme_tmp.txt
echo "libtiff 1.42" >> readme_tmp.txt
echo "libgd 2.1.0a" >> readme_tmp.txt
echo "libext2fs 1.0.5 r21" >> readme_tmp.txt
echo "libntfs 2012-1-15 r13" >> readme_tmp.txt
echo "libseeprom 2010-8-22 r304" >> readme_tmp.txt
echo "libotp 2010-8-22 r304" >> readme_tmp.txt
echo "libuneekfs 1.0.7" >> readme_tmp.txt

sed 's/$/\r/' readme_tmp.txt > readme.txt
rm readme_tmp.txt
cp -f readme.txt $DEVKITPRO/libogc/

cd ..

echo $a
