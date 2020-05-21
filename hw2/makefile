all: hw2

hw2: b064020039-hw2-part2.c
	gcc b064020039-hw2-part2.c -o hw2 -lpthread

dep:
	gcc -M *.c > .depend

clean:
	rm -f *.o hw2 .depend