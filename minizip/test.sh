#!/bin/bash

set -e

rm -rf tmp
cp -R res tmp

cd tmp

unzip zip000.zip

mkdir unzip
cd unzip
unzip ../unzip11.zip
patch ioapi.c < ../ioapi.c.patch
cd ..

g++ -o c000 c000.cpp unzip/unzip.c unzip/ioapi.c -lz -Iunzip
./c000
diff c000.png HelloWorld.png

echo ${0} OK