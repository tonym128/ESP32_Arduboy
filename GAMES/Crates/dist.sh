#!/usr/bin/env bash

cd dist
cp ../crates.ino.leonardo.hex .
butler push . jessemillar/crates:win-linux-mac
cd ..
butler push crates.ino.leonardo.hex jessemillar/crates:arduboy
