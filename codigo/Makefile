############################# Makefile ##########################
# MC548 Projeto Pratico #
all: linux clean

mac: saveWorldMac.o clean
saveWorldMac.o: saveWorld.c
	gcc -O2 quicksort.c saveWorld.c -o saveWorldMac

linux: saveWorld.o
saveWorld.o: saveWorld.c
	gcc -O2 quicksort.c saveWorld.c -o saveWorld

clean:
	rm -rf *.o
	rm -rf *.dSYM
