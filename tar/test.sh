#!/bin/bash

set -e

rm -rf tmp
cp -R res tmp

cd tmp

tar -cf t.tar data0.jpg data1.png

mkdir t0
cp t.tar t0/

pushd t0 >> /dev/null
tar -xf t.tar
diff data0.jpg ../data0.jpg
diff data1.png ../data1.png
popd >> /dev/null
