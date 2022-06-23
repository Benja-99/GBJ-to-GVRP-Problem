all: run
run: main.cpp
	g++ -o exe.o main.cpp
	./exe.o

clean:
	rm -rf *.o
