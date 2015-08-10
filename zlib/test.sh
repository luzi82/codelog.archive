#!/bin/bash

set -e

rm -rf tmp
cp -R res tmp

cd tmp

cp data0.jpg data0.jpg.ori
gzip data0.jpg
mv data0.jpg.ori data0.jpg

gzip -d data0.jpg.gz -c > data0.jpg.gz.-

diff data0.jpg.gz.- data0.jpg
