#!/bin/sh

YELLOW="\e[33m"
GREEN="\e[32m"
RED="\e[31m"
MAGENTA="\e[35m"
ENDCOLOR="\e[0m"

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
    echo -e ${MAGENTA}testing $(echo $TEST_DIR | cut -d'/' -f1) $ENDCOLOR
    cd $TEST_DIR
    echo -e ${YELLOW}building in DEBUG mode $ENDCOLOR
    make debug 1>/dev/null
    echo -e ${GREEN}running in DEBUG mode $GREEN
    ./main
    echo cleaning
    make clean
    echo -e ${YELLOW}building in RELEASE mode $ENDCOLOR
    make 1>/dev/null
    echo -e ${GREEN}running in RELEASE mode $GREEN
    ./main
    echo -e ${MAGENTA}done $(echo $TEST_DIR | cut -d'/' -f1) $ENDCOLOR
    cd ..
done

cd ..

exit 0
