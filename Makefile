CC=g++
FLAGS=-std=c++17 -Wall -g
CFLAGS=$(FLAGS) -c

once: obj/main obj/long_int obj/source obj/goppa obj/mcellice
	$(CC) -o $@ $^ $(FLAGS)

r: once 
	./$<

obj/main: main.cpp
	$(CC) -o $@ $< $(CFLAGS)

obj/long_int: source/long_int.cpp
	$(CC) -o $@ $< $(CFLAGS)

obj/source: source/source.cpp
	$(CC) -o $@ $< $(CFLAGS)

obj/goppa: source/goppa.cpp
	$(CC) -o $@ $< $(CFLAGS)

# obj/matrix: source/matrix.cpp
# 	$(CC) -o $@ $< $(CFLAGS)

obj/mcellice: source/mcellice.cpp
	$(CC) -o $@ $< $(CFLAGS)

rmall:
	rm obj/*