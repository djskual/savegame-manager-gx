#! /bin/bash
#

rev_raw=$(svnversion -n . 2>/dev/null | tr '\n' ' ' | tr -d '\r')
[ -n "$rev_raw" ] || rev_raw=$(SubWCRev . 2>/dev/null | tr '\n' ' ' | tr -d '\r')

rev_raw=$(echo $rev_raw | sed 's/[^0-9]*\([0-9]*\)\(.*\)/\1 \2/')
rev=0
a=$(echo $rev_raw | sed 's/\([0-9]*\).*/\1/')
let "a+=0"
#find max rev
while [ "$a" ]; do
	[ "$a" -gt "$rev" ] && rev=$a
	rev_raw=$(echo -n $rev_raw | sed 's/[0-9]*[^0-9]*\([0-9]*\)\(.*\)/\1 \2/')
	a=$(echo $rev_raw | sed 's/\([0-9]*\).*/\1/') 
done

rev=`expr $rev + 1`

rm -f ../branches/Updates/update.txt
cat <<EOF > ../branches/Updates/update.txt
$rev
http://savegame-manager-gx.googlecode.com/files/R$rev.dol
EOF

rm -f ../branches/Updates/meta.xml
cp ./HBC/meta.xml ../branches/Updates/meta.xml

rm -f ../branches/Updates/boot.dol
cp ./boot.dol ../branches/Updates/boot.dol

echo >&2
echo "Release files updated to Rev$rev" >&2
echo >&2

"c:\program files\winzip\wzzip" -p -r -xupdate.txt ./HBC_SetUp_R${rev}.zip ../branches/Updates
rm -f ../branches/Updates/boot.dol

cp ./boot.dol ./R$rev.dol

