#!/bin/bash

cd "$(dirname "$0")" || exit

./out/main.out populate ${1-https://www.wolframalpha.com/} ${2-100}