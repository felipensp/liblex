CC?=gcc
CFLAGS=-fPIC -g -c -Wall -std=c99
SRC=src
BLD=build
INC=include
LIB=lib
OBJS=$(BLD)/*.o

INSTALLINC=/usr/include
INSTALLLIB=/lib
INSTALLBIN=/usr/bin

lexer:	liblex
	$(CC) -olexer -I$(INC)/ $(SRC)/lexer/main.c -L$(LIB)/ -llex -lpcre

liblex:
	@mkdir -p $(BLD)
	$(CC) $(CFLAGS) -I$(INC)/ -o$(BLD)/tokenizer.o $(SRC)/tokenizer.c
	$(CC) $(CFLAGS) -I$(INC)/ -o$(BLD)/token.o $(SRC)/token.c
	$(CC) $(CFLAGS) -I$(INC)/ -o$(BLD)/regex.o $(SRC)/regex.c
	@mkdir -p $(LIB)
	$(CC) -shared -Wl,-soname,$(LIB)/liblex.so.0 -o$(LIB)/liblex.so.0.1.0 $(OBJS)
	ln -sf liblex.so.0.1.0 $(LIB)/liblex.so.0
	ln -sf liblex.so.0.1.0 $(LIB)/liblex.so

clean:
	rm -rf lexer $(LIB) $(BLD)/*.o $(LIB)/*

install:
	cp $(LIB)/* $(INSTALLLIB)
	ln -sf liblex.so.0.1.0 $(INSTALLLIB)/liblex.so.0
	ln -sf liblex.so.0.1.0 $(INSTALLLIB)/liblex.so
	cp $(INC)/* $(INSTALLINC)

	$(CC) -o$(INSTALLBIN)/lexer -I$(INSTALLINC)/ $(SRC)/lexer/main.c -L$(INSTALLLIB)/ -llex -lpcre
