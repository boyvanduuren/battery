CC=gcc
CFLAGS=-Wall
LIBS=-lm
SOURCEDIR=./src
HEADERS=$(SOURCEDIR)/main.h $(SOURCEDIR)/battery.h $(SOURCEDIR)/output.h
SOURCES=$(SOURCEDIR)/main.c $(SOURCEDIR)/battery.c $(SOURCEDIR)/output.c
OUT=./bin
BIN=battery

$(BIN): $(HEADERS) $(SOURCES) $(OUT)
	$(CC) $(CFLAGS) -o $(OUT)/$(BIN) $(SOURCES) $(LIBS)

debug: $(HEADERS) $(SOURCES) $(OUT)
	$(CC) -g $(CFLAGS) -o $(OUT)/$(BIN) $(SOURCES) $(LIBS)

$(OUT):
	mkdir $(OUT)

clean:
	rm -rf $(OUT)
