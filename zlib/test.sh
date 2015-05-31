#!/bin/bash

set -e

rm -rf tmp
cp -R res tmp

cd tmp

gzip --keep data0.jpg

gzip -d data0.jpg.gz -c > data0.jpg.gz.-

diff data0.jpg.gz.- data0.jpg
