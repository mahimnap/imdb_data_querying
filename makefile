CC = gcc
CFLAGS = -std=c99 -Wall -pedantic

all: buildidx key2val val2key bacon1 actors util.o hashfn.o

clean: 
	rm *.o buildidx key2val val2key actors bacon1

buildidx: buildidx.o util.o hashfn.o
	$(CC) $(CFLAGS) buildidx.o util.o hashfn.o -o buildidx
	
key2val: key2val.o util.o hashfn.o
	$(CC) $(CFLAGS) key2val.o util.o hashfn.o -o key2val 
	
val2key: val2key.o util.o hashfn.o 
	$(CC) $(CFLAGS) val2key.o util.o hashfn.o -o val2key
	
actors: actors.o util.o hashfn.o 
	$(CC) $(CFLAGS) actors.o util.o hashfn.o -o actors
	
bacon1: bacon1.o util.o hashfn.o 
	$(CC) $(CFLAGS) bacon1.o util.o hashfn.o -o bacon1

util.o: util.c util.h
	$(CC) $(CFLAGS) -c util.c -o util.o
	
hashfn.o: hashfn.c hashfn.h
	$(CC) $(CFLAGS) -c hashfn.c -o hashfn.o
	
buildidx.o: util.h hashfn.h buildidx.c
	$(CC) $(CFLAGS) -c buildidx.c -o buildidx.o

key2val.o: util.h hashfn.h key2val.c
	$(CC) $(CFLAGS) -c key2val.c -o key2val.o 
	
val2key.o: util.h hashfn.h val2key.c
	$(CC) $(CFLAGS) -c val2key.c -o val2key.o 

actors.o: util.h hashfn.h actors.c
	$(CC) $(CFLAGS) -c actors.c -o actors.o 
	
bacon1.o: util.h hashfn.h bacon1.c
	$(CC) $(CFLAGS) -c bacon1.c -o bacon1.o 
