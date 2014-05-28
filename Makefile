CC=gcc
LIBS=-lm
SRC=./src/
OUT=./bin/

battery: $(SRC)main.c $(SRC)main.h $(SRC)battery.c $(SRC)battery.h $(SRC)output.c $(SRC)output.h
	test -d $(OUT) || mkdir $(OUT)
	$(CC) -o $(OUT)battery $(SRC)main.c $(SRC)battery.c $(SRC)output.c $(LIBS)

debug: $(SRC)main.c $(SRC)main.h $(SRC)battery.c $(SRC)battery.h $(SRC)output.c $(SRC)output.h
	test -d $(OUT) || mkdir $(OUT)
	$(CC) -g -Wall -o $(OUT)battery $(SRC)main.c $(SRC)battery.c $(SRC)output.c $(LIBS)
