all: hw3

hw3: b064020039-hw3-part2.cpp
	gcc b064020039-hw3-part2.cpp -o hw3 -lpthread -lstdc++

dep:
	gcc -MM *.cpp > .depend

clean:
	rm -f *.o hw3 .depend
