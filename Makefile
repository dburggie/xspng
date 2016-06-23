SRC = source
INC = include
BLD = build

CFLG = -Wall -ggdb
CINC = -I${INC}
COPT = ${CFLG} ${CINC}
CC   = gcc ${COPT}
CPPC = g++ -std=c++11 ${COPT}

HDR  = ${INC}/xspng.h ${INC}/private_xspng.h

OBJ  = ${BLD}/xspng.o ${BLD}/private_xspng.o
OBJ += ${BLD}/xspng_image_write.o ${BLD}/xspng_chunk_deflate.o



all: ${OBJ}

dirs: ${BLD}
	mkdir -p ${BLD}

clean:


${BLD}/xspng.o: ${SRC}/xspng.c ${HDR}
	${CC} -o $@ -c $<

${BLD}/private_xspng.o: ${SRC}/private_xspng.c ${HDR}
	${CC} -o $@ -c $<

${BLD}/xspng_image_write.o: ${SRC}/xspng_image_write.c ${HDR}
	${CC} -o $@ -c $<

${BLD}/xspng_chunk_deflate.o: ${SRC}/xspng_chunk_deflate.c ${HDR}
	${CC} -o $@ -c $<

