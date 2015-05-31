#!/bin/bash

set -e

rm -rf tmp
cp -R res tmp

cd tmp

xz -z --keep data.jpg

xz -d data.jpg.xz -c > data.jpg.xz.-

diff data.jpg.xz.- data.jpg
