SRC = source
INC = include
BLD = build

CFLG = -Wall -ggdb
CINC = -I${INC}
COPT = ${CFLG} ${CINC}
CC   = gcc ${COPT}
CPPC = g++ -std=c++11 ${COPT}

all:

dirs: ${BLD}
	mkdir -p ${BLD}

clean:

