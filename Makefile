gtk_flag = $(shell pkg-config --cflags gtk+-3.0 --libs gtk+-3.0)
src = ui_logic.c graph.c tsp_solver.c ant.c
onlyuisrc = ui_logic.c
output = antGraph
cflags = --std=c11 -lm
debugflag = -g -DDEBUG
debugvariable = G_MESSAGES_DEBUG="all"
production = -O3 -w
gprof-flags = -pg
no-warning = -w
reformatflags = -ut --linux-style
onlyuiflags = -DONLYUI
indent = indent
reformat-src = ui_logic.c ui_logic.h ui_signals.h graph.c graph.h
cppcheck = cppcheck
cppcheck-flags = --enable=all --language=c --std=c11 --verbose --report-progress

compile:
	gcc $(gtk_flag) $(cflags) $(production) -o $(output) $(src)
run:
	gcc $(gtk_flag) $(cflags) $(production) -o $(output) $(src)
	./$(output)
debug:
	gcc $(gtk_flag) $(cflags) $(no-warning) $(debugflag) -o $(output) $(src)
	$(debugvariable) ./$(output)
gporf:
	gcc $(gtk_flag) $(cflags) $(no-warning) $(production) $(gprof-flags) -o $(output) $(src)
	./$(output)

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
cppcheck:
	$(cppcheck) $(cppcheck-flags) $(src)
win:
	gcc $(no-warning) $(cflags) $(production) -o $(output) $(src) $(gtk_flag)
