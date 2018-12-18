
raidsim: raidsim.o disk-array.o disk.o
	gcc raidsim.o disk-array.o disk.o -o raidsim

raidsim.o: raidsim.c
	gcc -Wall -g -c raidsim.c -o raidsim.o

disk-array.o: disk-array.c
	gcc -Wall -g -c disk-array.c -o disk-array.o

disk.o: disk.c
	gcc -Wall -g -c disk.c -o disk.o

clean:
	rm -f *.o raidsim
