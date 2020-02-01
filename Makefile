build: main.cpp
	g++ -std=c++17 -O3 main.cpp -o main

run: main
	./main $(arg)

clean:
	rm -f main