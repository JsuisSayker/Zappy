#!/bin/env bash

./clean.sh
cmake -S . -B build
cmake --build build -- -j $(nproc --all)