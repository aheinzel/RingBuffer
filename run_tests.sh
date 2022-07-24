#!/bin/bash

set -e

cmake -S . -B build
cmake --build build
./build/ringbuffer_test