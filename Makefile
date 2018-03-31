CC=gcc
SRC=./src
TARGET=./build
BINDIR=./bin
FLAGS=

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
	${CC} ${FLAGS} -c ${SRC}/cpu.c -o ${TARGET}/cpu.o

debug:
	${CC} ${FLAGS} -c ${SRC}/debug.c -o ${TARGET}/debug.o

disassm:
	${CC} ${FLAGS} -c ${SRC}/disassm.c -o ${TARGET}/disassm.o

time:
	${CC} ${FLAGS} -c ${SRC}/time.c -o ${TARGET}/time.o

memory:
	${CC} ${FLAGS} -c ${SRC}/memory.c -o ${TARGET}/memory.o

emulator:
	${CC} ${FLAGS} -c ${SRC}/emulator.c -o ${TARGET}/emulator.o

disassm_bin:
	${CC} ${FLAGS} ${FLAGS} ${TARGET}/memory.o ${TARGET}/debug.o ${TARGET}/disassm.o ${TARGET}/time.o ${TARGET}/cpu.o -o ${BINDIR}/disassm

emulator_bin:
	${CC} ${FLAGS} ${TARGET}/memory.o ${TARGET}/debug.o ${TARGET}/emulator.o ${TARGET}/time.o ${TARGET}/cpu.o -o ${BINDIR}/emulator


all: dirs memory debug cpu time disassm disassm_bin emulator emulator_bin