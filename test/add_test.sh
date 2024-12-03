#!/bin/sh

name=$1

if [[ $# -ne 1 ]]; then
    echo $0 [name]
    exit 1
fi

if [ ! -d ./test_flash -o ! -d ./.dummy -o ! -d ./tests -o -d ./tests/$name ]; then
    echo run the script inside the test directory
    exit 2
fi

cp -r ./.dummy ./tests/$name
