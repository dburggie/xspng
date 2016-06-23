
SRC = source
INC = include
BLD = build

ZLIB = -lz

CFLG = -Wall -ggdb
CINC = -I${INC}
COPT = ${CFLG} ${CINC}
CC   = gcc ${COPT}
CPPC = g++ -std=c++11 ${COPT}

HDR  = ${INC}/xspng.h ${INC}/private_xspng.h

OBJ  = ${BLD}/xspng.o ${BLD}/private_xspng.o
OBJ += ${BLD}/xspng_image_write.o ${BLD}/xspng_chunk_deflate.o

HDRPP = ${INC}/xspngpp.h ${HDR}
OBJPP = ${BLD}/xspngpp.o



all: ${BLD} ${TBLD} ${OBJ} ${OBJPP}



${BLD}/xspng.o: ${SRC}/xspng.c ${HDR}
	${CC} -o $@ -c $<

${BLD}/private_xspng.o: ${SRC}/private_xspng.c ${HDR}
	${CC} -o $@ -c $<

${BLD}/xspng_image_write.o: ${SRC}/xspng_image_write.c ${HDR}
	${CC} -o $@ -c $<

${BLD}/xspng_chunk_deflate.o: ${SRC}/xspng_chunk_deflate.c ${HDR}
	${CC} -o $@ -c $<

${BLD}/xspngpp.o: ${SRC}/xspngpp.cpp ${HDRPP}
	${CPPC} -o $@ -c $<


TSRC = test
TBLD = ${BLD}/test
TOBJ = ${TBLD}/example.o ${TBLD}/examplepp.o
TEXE = ${TBLD}/example ${TBLD}/examplepp

tests: ${TBLD} ${TEXE}

${TBLD}/example: ${TBLD}/example.o ${OBJ}
	${CC} -o $@ $^ ${ZLIB}
	./$@

${TBLD}/example.o: ${TSRC}/example.c ${HDR}
	${CC} -o $@ -c $<

${TBLD}/examplepp: ${TBLD}/examplepp.o ${OBJ} ${OBJPP}
	${CPPC} -o $@ $^ ${ZLIB}
	./$@

${TBLD}/examplepp.o: ${TSRC}/examplepp.cpp ${HDRPP}
	${CPPC} -o $@ -c $<



${BLD}:
	mkdir -p ${BLD}

${TBLD}:
	mkdir -p $@

clean:
	rm -f ${OBJ}
	rm -f ${TOBJ}
	rm -f ${TEXE}


