############################# Makefile ##########################
# MC548 Projeto Pratico #
all: linux clean

mac: saveWorldMac.o clean
saveWorldMac.o: saveWorld.c
	gcc -o saveWorldMac.o -c saveWorld.c -W -Wall -pedantic

linux: saveWorld.o
saveWorld.o: saveWorld.c
	gcc -o saveWorld.o -c saveWorld.c -W -Wall -pedantic

clean:
	rm -rf *.o
	rm -rf *.dSYM