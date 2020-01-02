#!/bin/bash

cd "$(dirname "$0")"

make
mvn spring-boot:run