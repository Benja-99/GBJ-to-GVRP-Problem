all: run
run: main.cpp
	g++ -o exe.o otro.cpp
	./exe.o

clean:
	rm -rf *.o
