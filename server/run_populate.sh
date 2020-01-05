#!/bin/bash

cd "$(dirname "$0")" || exit

./out/main.out populate ${1-https://www.put.poznan.pl/} ${2-1000}