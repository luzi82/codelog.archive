#!/bin/bash

set -e

rm -rf tmp
cp -R res tmp

cd tmp

tar -cf t.tar data0.jpg roboto-1.2.zip data1.png
gzip -c t.tar > t.tar.gz
bzip2 -c t.tar > t.tar.bz2

g++ -o c000 c000.cpp -larchive
./c000

g++ -o c001 c001.cpp -larchive
./c001

g++ -o c002 c002.cpp -larchive
mkdir c002.tmp
cp c002 t.tar.gz c002.tmp/
pushd c002.tmp >> /dev/null
./c002
diff data0.jpg ../data0.jpg
diff data1.png ../data1.png
diff roboto-1.2.zip ../roboto-1.2.zip
popd >> /dev/null

echo c003 START
g++ -o c003 c003.cpp -larchive
./c003
echo c003 END

echo c004 START
g++ -o c004 c004.cpp -larchive
./c004
echo c004 END

echo c005 START
g++ -o c005 c005.cpp -larchive
./c005
echo c005 END
