#
# Makefile for idobata_server and idobata_client
#
MYLIBDIR=./mynet
MYLIB=-lmynet
CFLAGS=-I${MYLIBDIR} -L${MYLIBDIR}
OBJS=idobata.o idobata_server.o idobata_client.o idobata_util.o idobata_list.o

all: idobata

idobata: ${OBJS}
	${CC} ${CFLAGS} -o $@ $^ ${MYLIB}

clean:
	${RM} *.o idobata *~
