// created by emacs {navid  oct 25 2014}

#include <gtk/gtk.h>

#define GRAPH_WIN_CONTAINER_BORDER_WIDTH 2
#define GRAPH_WIN_WIDTH 1000
#define GRAPH_WIN_HEIGHT 600
#define DRAWING_AREA_WIDTH 800
#define DRAWING_AREA_HEIGHT 600
#define MAIN_WIN_WIDTH 400
#define MAIN_WIN_HEIGHT 600
#define RESIZABLE FALSE
#define RECT_ 6

const char *mainwin_ui_name = "mainwindow.glade";
const char *mainwin_title = "Main Window";
const char *graphwin_title = "Graph Window";
const char *mainwin_ui_frame_name = "frame_ui";
static cairo_surface_t *surface = NULL;


typedef enum { Vertex , Random } Mode;

static gboolean on_draw(GtkWidget *widget,
		    cairo_t *cr,
		    gpointer data){
	cairo_set_source_surface(cr,surface,0,0);
	cairo_paint(cr);
	return FALSE;
}


static void clear_surface(void){
	cairo_t *cr;
	cr = cairo_create(surface);
	cairo_set_source_rgb(cr,245,245,245);
	cairo_paint(cr);
	cairo_destroy(cr);
}


static gboolean on_configure_event(GtkWidget *widget,
				   GdkEventConfigure *event,
				   gpointer data){
	g_printf("on_configure_event\n");
	if(surface)
		cairo_surface_destroy(surface);
	surface = gdk_window_create_similar_surface(gtk_widget_get_window(widget),
						    CAIRO_CONTENT_COLOR,
						    gtk_widget_get_allocated_width(widget),
						    gtk_widget_get_allocated_height(widget));
	clear_surface();
	return TRUE;
}

static void draw_rectangle(GtkWidget *widget,
			   gdouble x,
			   gdouble y){
	cairo_t *cr;
	g_printf("drawing here : (%f,%f)\n",x,y);
	if(!surface){
		g_printf("fatal error in line:%d",__LINE__);
		exit(1);
	}
	cr = cairo_create(surface);
	cairo_rectangle(cr,x,y,RECT_,RECT_);
	cairo_fill(cr);
	cairo_destroy(cr);
	gtk_widget_queue_draw_area(widget,x,y,RECT_,RECT_);
}


static gboolean 
on_delete_event(GtkWidget *widget,
		GdkEvent *event,
		gpointer data){
	g_print("on_delete_event\n");
	/* ask from user , if he wants really quit then return FALSE , otherwise return TRUE , OK? */


	return FALSE;
}

static void
on_clicked(GtkWidget *widget,
	   gpointer data){
	g_print("from graph_window_clicked\n");

}

static gboolean
on_button_press_event(GtkWidget *widget,
		      GdkEventButton *event,
		      gpointer data){
	g_print("from on_button_press_event");
	g_print(" (%f,%f)\n",event->x,event->y);
	Mode *mode = data;
	if((*mode) == Vertex)
	{
		g_print("should draw an rectangle\n");
		draw_rectangle(widget,event->x,event->y);
	}

	return TRUE;
}

static gboolean
on_motion_notify_event(GtkWidget *widget,
		       GdkEventMotion *event,
		       gpointer data){
//	g_print("from on_motion_notify_event (%f,%f)\n",event->x,event->y);

	return TRUE;
}

int main(int argc , char **argv){
	GtkWidget *mainwin , *graphwin;
	GtkWidget *frame;
	GtkWidget *drawing_area;
	GtkBuilder *builder;
	GtkWidget *frame_from_ui;
	GtkRequisition *minimum_size_for_frame_ui;
	Mode mode = Vertex;

	gtk_init(&argc,&argv);
	
	builder = gtk_builder_new();
	gtk_builder_add_from_file(builder,mainwin_ui_name,NULL);
	frame_from_ui = gtk_builder_get_object(builder,mainwin_ui_frame_name);
	
	mainwin = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	graphwin = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	gtk_container_add(GTK_CONTAINER(mainwin),frame_from_ui);
	
	// getting preffered size from frame_from_ui for fixing size of mainwin currectly
	minimum_size_for_frame_ui = malloc(sizeof(GtkRequisition));
	gtk_widget_get_preferred_size(frame_from_ui,minimum_size_for_frame_ui,NULL);
	// setting some options of graphwin and mainwin
	gtk_window_set_title(GTK_WINDOW(mainwin),mainwin_title);
	gtk_window_set_title(GTK_WINDOW(graphwin),graphwin_title);
	gtk_widget_set_size_request(graphwin,GRAPH_WIN_WIDTH,GRAPH_WIN_HEIGHT);
//	gtk_widget_set_size_request(mainwin,MAIN_WIN_WIDTH,MAIN_WIN_HEIGHT);
	gtk_widget_set_size_request(mainwin,minimum_size_for_frame_ui->width,minimum_size_for_frame_ui->height);
	gtk_window_set_resizable(GTK_WINDOW(mainwin),RESIZABLE);
	gtk_window_set_resizable(GTK_WINDOW(graphwin),RESIZABLE);
	


	frame = gtk_frame_new(NULL);
	gtk_frame_set_shadow_type(GTK_FRAME(graphwin),GTK_SHADOW_IN);
	gtk_container_add(GTK_CONTAINER(graphwin),frame);

	drawing_area = gtk_drawing_area_new();
	gtk_widget_set_size_request(drawing_area,DRAWING_AREA_WIDTH,DRAWING_AREA_HEIGHT);
	gtk_container_add(GTK_CONTAINER(frame),drawing_area);
	gtk_widget_set_events(drawing_area,gtk_widget_get_events(drawing_area) | GDK_BUTTON_PRESS_MASK | GDK_POINTER_MOTION_MASK);

	g_signal_connect(mainwin,"destroy",G_CALLBACK(gtk_main_quit),NULL);
	g_signal_connect(graphwin,"destroy",G_CALLBACK(gtk_main_quit),NULL);
	g_signal_connect(graphwin,"configure-event",G_CALLBACK(on_configure_event),NULL);
	g_signal_connect(drawing_area,"motion-notify-event",G_CALLBACK(on_motion_notify_event),NULL);
	g_signal_connect(drawing_area,"button-press-event",G_CALLBACK(on_button_press_event),&mode);
	g_signal_connect(drawing_area,"clicked",G_CALLBACK(on_clicked),NULL);
	g_signal_connect(drawing_area,"draw",G_CALLBACK(on_draw),NULL);
	/* connecting signals and events */
	g_signal_connect(mainwin,"delete-event",G_CALLBACK(on_delete_event),NULL);


//	g_signal_connect(graphwin,"clicked",G_CALLBACK(graph_window_clicked),NULL);
	gtk_container_set_border_width(GTK_CONTAINER(graphwin),GRAPH_WIN_CONTAINER_BORDER_WIDTH);

	
	gtk_widget_show(mainwin);
	gtk_widget_show_all(graphwin);
	
	gtk_main();

	return 0;
}
