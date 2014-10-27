gtk_flag = $(shell pkg-config --cflags gtk+-3.0 --libs gtk+-3.0)
src = ui_logic.c
output = antGraph
cflags = --std=c11
debugflag = -g
debugvariable = G_MESSAGES_DEBUG="all"
production = -O3
no-warning= -w
compile:
	gcc $(gtk_flag) $(cflags) $(production) -o $(output) $(src)
run:
	gcc $(gtk_flag) $(cflags) $(production) -o $(output) $(src)
	./$(output)
debug:
	gcc $(gtk_flag) $(cflags) $(no-warning) $(debugflag) -o $(output) $(src)
	$(debugvariable) ./$(output)
debug-warning:
	gcc $(gtk_flag) $(cflags) $(debugflag) -o $(output) $(src)
	$(debugvariable) ./$(output)
clean:
	rm antGraph
