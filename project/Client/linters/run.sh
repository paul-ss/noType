#!/usr/bin/env bash

set -e
set -o pipefail

function print_header() {
    echo -e "\n***** ${1} *****"
}
print_header "RUN cppcheck"
if [ "${1}" == "--local" ]; then
	CPPCHECK="cppcheck --check-config"
else
	CPPCHECK="./linters/cppcheck/cppcheck"
fi
${CPPCHECK} source --enable=all --error-exitcode=1 -I application/* # --check-config

print_header "RUN cpplint.py"
python2.7 ./linters/cpplint/cpplint.py --extensions=c --headers=h,hpp --filter=-runtime/references,-legal/copyright,-build/include_subdir,-whitespace/line_length application/*

print_header "SUCCESS"
