#!/bin/bash
SOURCE="source"
BUILD="build"
COMPILER_CALL="gcc -std=c11"
C_OBJECTS=""

mkdir -p build/
mkdir -p execute/

for C_SOURCE in $(find ${SOURCE} -name "*.c")
do
    # echo ${C_SOURCE}
    # echo $(basename ${C_SOURCE} .c)
    BASE_NAME=$(basename ${C_SOURCE} .c)
    # echo ${BASE_NAME}
    ${COMPILER_CALL} -c ${C_SOURCE} -o ${BUILD}/${BASE_NAME}.o
    C_OBJECTS="${C_OBJECTS}${BUILD}/${BASE_NAME}.o "
done

# echo ${C_OBJECTS}

${COMPILER_CALL} ${C_OBJECTS} -o execute/main

# ./execute/main.exe

rm -rf build/*
# rm -rf main.exe