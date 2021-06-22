CC	=	gcc
CFLAGS	=	-g -Wall
BUILD	=	lexer

all: $(BUILD)

clean:
	/bin/rm -f $(BUILD) *.o cor *.s

lexer: lexer.o error.o functionTable.o functions.o parser.o instrlist.o utils.o
lexer.o: typedef.h error.h lexer.h functionTable.h
error.o: typedef.h
functionTable.o: typedef.h lexer.h
parser.o: typedef.h error.h lexer.h functionTable.h
functions.o: instrlist.h typedef.h lexer.h functionTable.h 
instrlist.o: typedef.h utils.h functions.h
utils.o: typedef.h functionTable.h error.h functions.h
