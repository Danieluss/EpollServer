#!/bin/bash

cd "$(dirname "$0")" || exit

make
./run_server.sh