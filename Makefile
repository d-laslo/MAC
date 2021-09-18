CC=g++
FLAGS=-std=c++17 -Wall -g
CFLAGS=$(FLAGS) -c

once: obj/main obj/source
	$(CC) -o $@ $^ $(FLAGS)

r: once 
	./$<

obj/main: main.cpp
	$(CC) -o $@ $< $(CFLAGS)

obj/source: source/source.cpp
	$(CC) -o $@ $< $(CFLAGS)

rmall:
	rm obj/*