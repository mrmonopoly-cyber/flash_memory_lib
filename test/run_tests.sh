#!/bin/sh

YELLOW="\e[33m"
GREEN="\e[32m"
RED="\e[31m"
MAGENTA="\e[35m"
ENDCOLOR="\e[0m"

run_test() {
    echo -e ${MAGENTA}testing $(echo $1 | cut -d'/' -f1) $ENDCOLOR
    cd $1
    echo -e ${YELLOW}building in DEBUG mode $ENDCOLOR
    make debug 1>/dev/null
    echo -e ${GREEN}running in DEBUG mode $ENDCOLOR
    ./main
    make clean 1>/dev/null
    echo -e ${YELLOW}building in RELEASE mode $ENDCOLOR
    make 1>/dev/null
    echo -e ${GREEN}running in RELEASE mode $ENDCOLOR
    ./main
    cd ..
}

if [ $# -ge 3 -a $# -le 0 ]; then
    echo $0 [name]
    exit 1
fi

if [ ! -d ./test_flash -o ! -d ./.dummy ]; then
    echo run the script inside the test directory
    exit 2
fi

cd ./tests/
if [ $# -eq 1 ]; then
    if [ -d ./$1 ]; then
        run_test $1
    else
        echo test not found
    fi
    cd ..
    exit 0
fi

for TEST_DIR in $(/bin/ls -d */ 2>/dev/null ); do
    run_test $TEST_DIR
done

cd ..

exit 0
