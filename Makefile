gtk_flag = $(shell pkg-config --cflags gtk+-3.0 --libs gtk+-3.0)
src = ui_logic.c graph.c
onlyuisrc = ui_logic.c
output = antGraph
cflags = --std=c11 -lm
debugflag = -g
debugvariable = G_MESSAGES_DEBUG="all"
production = -O3
no-warning = -w
reformatflags = -ut --linux-style
onlyuiflags = -DONLYUI
indent = indent
reformat-src = ui_logic.c ui_logic.h ui_signals.h graph.c graph.h
compile:
	gcc $(gtk_flag) $(cflags) $(production) -o $(output) $(src)
run:
	gcc $(gtk_flag) $(cflags) $(production) -o $(output) $(src)
	./$(output)
debug:
	gcc $(gtk_flag) $(cflags) $(no-warning) $(debugflag) -o $(output) $(src)
	$(debugvariable) ./$(output)
debug-onlyui:
	gcc $(onlyuiflags)  $(gtk_flag) $(cflags) $(no-warning) $(debugflag) -o $(output) $(onlyuisrc)
	$(debugvariable) ./$(output)

debug-warning:
	gcc $(gtk_flag) $(cflags) $(debugflag) -o $(output) $(src)
	$(debugvariable) ./$(output)
clean:
	rm antGraph
reformat:
	$(shell indent $(reformatflags) $(reformat-src))
	$(shell rm *~)
