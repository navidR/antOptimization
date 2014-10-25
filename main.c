// created by emacs {navid  oct 25 2014}

#include <gtk/gtk.h>

char *mainwin_title = "Main Window";
char *graphwin_title = "Graph Window";




int main(int argc , char **argv){
	GtkWidget *mainwin , *graphwin;
	gtk_init(&argc,&argv);
	
	mainwin = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	graphwin = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(mainwin),mainwin_title);
	gtk_window_set_title(GTK_WINDOW(graphwin),graphwin_title);

	g_signal_connect(mainwin,"destroy",G_CALLBACK(gtk_main_quit),NULL);
	g_signal_connect(graphwin,"destroy",G_CALLBACK(gtk_main_quit),NULL);


	gtk_widget_show(mainwin);
	gtk_widget_show(graphwin);

	gtk_main();

	return 0;
}
