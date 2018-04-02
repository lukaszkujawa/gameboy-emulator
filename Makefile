CC=clang
SRC=./src
TARGET=./build
BINDIR=./bin
LDFLAGS=-framework SDL2
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

gpu:
	${CC} ${FLAGS} -I/Library/Frameworks/SDL2.framework/Headers -c ${SRC}/gpu.c -o ${TARGET}/gpu.o

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

emulator_bin:
	${CC} ${FLAGS} ${LDFLAGS} ${TARGET}/gpu.o ${TARGET}/memory.o ${TARGET}/debug.o ${TARGET}/emulator.o ${TARGET}/time.o ${TARGET}/cpu.o -o ${BINDIR}/emulator


all: gpu dirs memory debug cpu time emulator emulator_bin


