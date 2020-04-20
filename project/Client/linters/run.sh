#!/usr/bin/env bash

set -e
set -o pipefail

function print_header() {
    echo -e "\n***** ${1} *****"
}

print_header "RUN cppcheck"
cppcheck source --enable=all --error-exitcode=1 -I application/*

print_header "RUN cpplint.py"
python2.7 ./linters/cpplint/cpplint.py --extensions=c --headers=h,hpp --filter=-runtime/references,-legal/copyright,-build/include_subdir,-whitespace/line_length application/*

print_header "RUN PVS-Studio"
pvs-studio-analyzer analyze -l linters/pvs/PVS-Studio.lic -o linter/pvs/project.log
plog-converter -a GA:1,2 -t tasklist linters/pvs/project.log

print_header "SUCCESS"