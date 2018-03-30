CC=gcc
SRC=./src
TARGET=./build
BINDIR=./bin

default_target: all

clean:
	rm ${TARGET}/*.o
	rm ${BINDIR}/*
	rmdir ${BINDIR}
	rmdir ${TARGET}

dirs:
	mkdir -p ${BINDIR}
	mkdir -p ${TARGET}	

cpu:
	${CC} -c ${SRC}/cpu.c -o ${TARGET}/cpu.o

disassm:
	${CC} -c ${SRC}/disassm.c -o ${TARGET}/disassm.o

time:
	${CC} -c ${SRC}/time.c -o ${TARGET}/time.o

emulator:
	${CC} -c ${SRC}/emulator.c -o ${TARGET}/emulator.o

disassm_bin:
	${CC} ${TARGET}/disassm.o ${TARGET}/time.o ${TARGET}/cpu.o -o ${BINDIR}/disassm

emulator_bin:
	${CC} ${TARGET}/emulator.o ${TARGET}/time.o ${TARGET}/cpu.o -o ${BINDIR}/emulator


all: dirs cpu time disassm disassm_bin emulator emulator_bin