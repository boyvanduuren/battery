CC=gcc
CFLAGS=-Wall
LIBS=-lm
SOURCEDIR=./src
HEADERS=$(SOURCEDIR)/main.h $(SOURCEDIR)/battery.h $(SOURCEDIR)/output.h
SOURCES=$(SOURCEDIR)/main.c $(SOURCEDIR)/battery.c $(SOURCEDIR)/output.c
OUT=./bin
BIN=battery
MAJOR=0
MINOR=2
PATCH=2

$(BIN): $(HEADERS) $(SOURCES) $(OUT)
	$(CC) $(CFLAGS) -o $(OUT)/$(BIN) $(SOURCES) $(LIBS) -DMAJOR=$(MAJOR) \
		-DMINOR=$(MINOR) -DPATCH=$(PATCH)

debug: $(HEADERS) $(SOURCES) $(OUT)
	$(CC) -g $(CFLAGS) -o $(OUT)/$(BIN) $(SOURCES) $(LIBS) -DMAJOR=$(MAJOR) \
		-DMINOR=$(MINOR) -DPATCH=$(PATCH)

$(OUT):
	mkdir $(OUT)

clean:
	rm -rf $(OUT)
