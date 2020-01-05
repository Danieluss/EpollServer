#!/bin/bash

cd "$(dirname "$0")" || exit

./out/main.out host ${1-0.0.0.0} ${2-10101}