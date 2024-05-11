RM=/usr/bin/rm
CC=/usr/bin/clang
CFLAGS=-O2 -pipe

all:
	@${CC} ${CFLAGS} -o writer writer.c png.c
	@${CC} ${CFLAGS} -o reader reader.c png.c

clean:
	@${RM} -f writer reader