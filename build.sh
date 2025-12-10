#!/bin/bash
set -e

# Create the build directory and generate the build system
cmake -B build

# Compile the project
cmake --build build
