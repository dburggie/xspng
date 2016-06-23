
SRC = source
INC = include
BLD = build

LIBZ  = libz/adler32.o libz/compress.o libz/crc32.o libz/deflate.o
LIBZ += libz/gzclose.o libz/gzlib.o libz/gzread.o libz/gzwrite.o
LIBZ += libz/infback.o libz/inffast.o libz/inflate.o libz/inftrees.o
LIBZ += libz/trees.o libz/uncompr.o libz/zutil.o

LIB  = lib

CFLG = -Wall -ggdb
CINC = -I${INC}
CLIB = -L${LIB}
COPT = ${CFLG} ${CINC} ${CLIB}
CC   = gcc ${COPT}
CPPC = g++ -std=c++11 ${COPT}

HDR  = ${INC}/xspng.h ${INC}/private_xspng.h ${INC}/zlib.h

OBJ  = ${BLD}/xspng.o ${BLD}/private_xspng.o
OBJ += ${BLD}/xspng_image_write.o ${BLD}/xspng_chunk_deflate.o

HDRPP = ${INC}/xspngpp.h ${HDR}
OBJPP = ${BLD}/xspngpp.o


all: ${BLD} ${TBLD} dist



dist: ${LIB} ${LIB}/libxspng.a ${LIB}/libxspngpp.a

${LIB}/libxspng.a: ${OBJ} ${LIBZ}
	ar rcs $@ $^

${LIB}/libxspngpp.a: ${OBJ} ${OBJPP} ${LIBZ}
	ar rcs $@ $^


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

${TBLD}/example: ${TBLD}/example.o ${LIB}/libxspng.a
	${CC} -o $@ $< -lxspng

${TBLD}/example.o: ${TSRC}/example.c ${HDR}
	${CC} -o $@ -c $<

${TBLD}/examplepp: ${TBLD}/examplepp.o ${LIB}/libxspngpp.a
	${CPPC} -o $@ $< -lxspngpp

${TBLD}/examplepp.o: ${TSRC}/examplepp.cpp ${HDRPP}
	${CPPC} -o $@ -c $<



${BLD}:
	mkdir -p $@

${TBLD}:
	mkdir -p $@

${LIB}:
	mkdir -p $@

clean:
	rm -f ${OBJ}
	rm -f ${OBJPP}
	rm -f ${TOBJ}
	rm -f ${TEXE}


