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
diff c000.out HelloWorld.png

g++ -o c001 c001.cpp unzip/unzip.c unzip/ioapi.c -lz -Iunzip
./c001
diff c001.out QIxndqXX/square.png

g++ -o c002 c002.cpp unzip/unzip.c unzip/ioapi.c -lz -Iunzip
./c002
diff c002.out HelloWorld.png

g++ -o c003 c003.cpp unzip/unzip.c unzip/ioapi.c -lz -Iunzip
./c003

echo ${0} OK
