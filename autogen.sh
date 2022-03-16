#!/usr/bin/env bash

mkdir -p bin
mkdir -p build-aux
mkdir -p m4

if [[ ! -f ./configure ]]; then
    autoreconf -vi
fi
if [[ ! -f build-aux/depcomp ]]; then
    automake --add-missing
fi
if [[ ! -f build-aux/test-driver ]]; then
    automake --add-missing
fi
if [[ ! -f bin/Makefile ]]; then
    cd bin && ../configure && cd ..
fi
make -C bin all check
