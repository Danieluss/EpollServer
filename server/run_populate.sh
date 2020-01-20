#!/bin/bash

cd "$(dirname "$0")" || exit

mkdir -p res
./out/main.out populate ${1-https://www.wolframalpha.com/} ${2-2000} ${3-10}
