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
    echo $TEST_DIR
    cd $TEST_DIR
    echo building
    make
    echo running
    ./main
    echo done
    cd ..
done

cd ..

exit 0
