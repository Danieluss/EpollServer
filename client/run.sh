#!/bin/bash

cd "$(dirname "$0")"

java -jar net-search-engine-*.jar -Dserver.port=${1-8080} -DsearchServer.host=${2-127.0.0.1} -DsearchServer.port=${3-10101}