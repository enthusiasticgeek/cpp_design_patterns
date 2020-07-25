#!/bin/bash
mkdir -p _build
mkdir -p _install
rm -rf _build/*
rm -rf _install/*
cd _build
cmake .. -DCMAKE_INSTALL_PREFIX=../_install
make
make install
