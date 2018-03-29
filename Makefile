CC=gcc
SRC=./src
TARGET=./build
BINDIR=./bin

default_target: all

clean:
	rm ${TARGET}/*.o
	rm ${BINDIR}/*

emulator:
	${CC} -c ${SRC}/emulator.c -o ${TARGET}/emulator.o

disassm:
	${CC} -c ${SRC}/disassm.c -o ${TARGET}/disassm.o

disassm_bin:
	${CC} ${TARGET}/disassm.o ${TARGET}/emulator.o -o ${BINDIR}/disassm

all: emulator disassm disassm_bin