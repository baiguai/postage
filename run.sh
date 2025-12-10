#!/bin/bash
set -e

# Build the project
./build.sh

# Run the application
./build/postage
