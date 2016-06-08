CC=gcc
FLAGS=-o
TARGET="Target"

OBJ=Basic_V4L2_API.o main.o Image.o

${TARGET}:${OBJ}
	@${CC} ${OBJ} ${FLAGS} ${TARGET}
	@rm *.o
	@./${TARGET}

main.o:main.c
Basic_V4L2_API.o:Basic_V4L2_API.c
Image.o:Image.c

.PHONY: clean
clean:
	@rm ${TARGET}
