CC?=gcc

SRC=src
BLD=build
INC=include
LIB=lib
OBJS=$(BLD)/*.o

INSTALLINC=/usr/include
INSTALLLIB=/lib
INSTALLBIN=/usr/bin

liblex:
	mkdir -p $(BLD)
	$(CC) -fPIC -g -c -Wall -std=c99 -I$(INC)/ -o$(BLD)/tokenizer.o $(SRC)/tokenizer.c
	$(CC) -fPIC -g -c -Wall -std=c99 -I$(INC)/ -o$(BLD)/token.o $(SRC)/token.c
	$(CC) -fPIC -g -c -Wall -std=c99 -I$(INC)/ -o$(BLD)/regex.o $(SRC)/regex.c

	mkdir -p $(LIB)	
	$(CC) -shared -Wl,-soname,$(LIB)/liblex.so.0 -o$(LIB)/liblex.so.0.1.0 $(OBJS)
	ln -sf liblex.so.0.1.0 $(LIB)/liblex.so.0
	ln -sf liblex.so.0.1.0 $(LIB)/liblex.so

	$(CC) -olexer -I$(INC)/ $(SRC)/lexer/main.c -L$(LIB)/ -llex -lpcre

clean:
	rm -rf rwelf $(LIB) $(BLD)/*.o

install:
	cp $(LIB)/* $(INSTALLLIB)
	ln -sf liblex.so.0.1.0 $(INSTALLLIB)/liblex.so.0
	ln -sf liblex.so.0.1.0 $(INSTALLLIB)/liblex.so
	cp $(INC)/* $(INSTALLINC)

	$(CC) -o$(INSTALLBIN)/lexer -I$(INSTALLINC)/ $(SRC)/lexer/main.c -L$(INSTALLLIB)/ -llex -lpcre
