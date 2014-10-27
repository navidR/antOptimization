gtk_flag = $(shell pkg-config --cflags gtk+-3.0 --libs gtk+-3.0)
src = main.c
output = antGraph
cflags = --std=c11

compile:
	gcc $(gtk_flag) $(cflags) -o $(output) $(src)
run:
	gcc $(gtk_flag) $(cflags) -o $(output) $(src)
	./$(output)
