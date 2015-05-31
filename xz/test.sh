#!/bin/bash

set -e

rm -rf tmp
cp -R res tmp

cd tmp

xz -z --keep data0.jpg

xz -d data0.jpg.xz -c > data0.jpg.xz.-

diff data0.jpg.xz.- data0.jpg
