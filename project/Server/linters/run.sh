#!/usr/bin/env bash

set -e
set -o pipefail

function print_header() {
    echo -e "\n***** ${1} *****"
}

print_header "RUN cppcheck"
if [ "${1}" == "--local" ]; then
	CPPCHECK="cppcheck"
else
	CPPCHECK="./linters/cppcheck/cppcheck"
fi
${CPPCHECK} project --enable=all --error-exitcode=1 -I source/* --suppress=missingIncludeSystem  --check-config

print_header "RUN cpplint.py"
python2.7 ./linters/cpplint/cpplint.py --extensions=c,cpp --headers=h,hpp --filter=-runtime/references source/Controller/* source/Command/* source/Parser/Test/testparsefrom.cpp

print_header "SUCCESS"
