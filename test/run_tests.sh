#!/bin/sh

if [[ $# -ne 0 ]]; then
    echo $0 [name]
    exit 1
fi

if [ ! -d ./test_flash -o ! -d ./.dummy ]; then
    echo run the script inside the test directory
    exit 2
fi

cd ./tests/
for TEST_DIR in $(/bin/ls -d */ 2>/dev/null ); do
    echo testing $(echo $TEST_DIR | cut -d'/' -f1)
    cd $TEST_DIR
    echo building in DEBUG mode
    make debug
    echo running in DEBUG mode
    ./main
    echo cleaning
    make clean
    echo building in RELEASE mode
    make
    echo running in RELEASE mode
    ./main
    echo done $(echo $TEST_DIR | cut -d'/' -f1)
    cd ..
done

cd ..

exit 0
