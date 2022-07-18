all: run
run: main.cpp
	g++ -o exe.o otro_test.cpp
	./exe.o

clean:
	rm -rf *.o
