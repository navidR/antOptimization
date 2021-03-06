// created by emacs {navid  oct 25 2014}
#include <gtk/gtk.h>
#include <glib.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "ui_logic.h"
#if !defined(ONLYUI)
#include "graph.h"
#include "tsp_solver.h"
#endif
#include "ui_signals.h"


int main(int argc, char **argv)
{
	GtkWidget *mainwin, *graphwin;
	GtkWidget *frame;
	GtkWidget *drawing_area;
	GtkBuilder *builder;
	GtkWidget *frame_from_ui;
	GtkRequisition *minimum_size_for_frame_ui;
	GtkSpinButton *input_numofants, *input_numofedges, *input_numofvertices,
		*input_evaporation_rate, *input_numofcycle;
	GtkButton *button_solve_problem, *button_generating_random_graph;
	GtkToggleButton *button_drawing_mode;
	GtkProgressBar *progressbar;
	GPtrArray *widget_array;
	GtkAdjustment *adjustment_input_numofvertices,
	    *adjustment_input_numofants, *adjustment_evaporation_rate,
		*adjustment_input_numofedges, *adjustment_input_numofcycle;
	_mode mode = RANDOM_MODE;
	_status status = STOP;
	
#if defined(ONLYUI)
	g_debug("main:compiled via ONLYUI flag,for debuging onlyui purpuse");
#endif
	
	ui_points =
	    g_array_sized_new(FALSE, TRUE, sizeof(GdkPoint),
			      RESERVED_SIZE_OF_ARRAY_POINTS);
	if (!ui_points)
		g_error("ui_points is null , fatal error in line:%d.",
			__LINE__);
	// array for capsulizing information in widget_array for signal
	widget_array = g_ptr_array_new();
	gtk_init(&argc, &argv);
	builder = gtk_builder_new();
	gtk_builder_add_from_file(builder, mainwin_ui_name, NULL);
	mainwin = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	graphwin = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	// after loading frame_from_ui,load it to mainwin
	frame_from_ui = gtk_builder_get_object(builder, mainwin_ui_frame_name);
	gtk_container_add(GTK_CONTAINER(mainwin), frame_from_ui);

	// getting preffered size from frame_from_ui for fixing size of mainwin currectly
	minimum_size_for_frame_ui = malloc(sizeof(GtkRequisition));
	gtk_widget_get_preferred_size(frame_from_ui, minimum_size_for_frame_ui,
				      NULL);

	// loading objects from ui file
	input_numofants = gtk_builder_get_object(builder, input_numofants_name);
	input_numofedges =
		gtk_builder_get_object(builder, input_numofedges_name);
	input_numofvertices =
	    gtk_builder_get_object(builder, input_numofvertices_name);
	input_evaporation_rate =
		gtk_builder_get_object(builder, input_evaporation_rate_name);
	input_numofcycle = gtk_builder_get_object(builder, input_numofcycle_name);
	button_solve_problem =
		gtk_builder_get_object(builder, button_solve_problem_name);
	button_drawing_mode =
		gtk_builder_get_object(builder, button_drawing_mode_name);
	button_generating_random_graph =
		gtk_builder_get_object(builder,
				       button_generating_random_graph_name);
	progressbar = gtk_builder_get_object(builder, progressbar_name);

	// setting for object loaded from ui file
	adjustment_input_numofvertices =
	    gtk_adjustment_new(MIN_NUMOFVERTICES, MIN_NUMOFVERTICES,
			       MAX_NUMOFVERTICES, INCREMENT_RATE, ZERO, ZERO);
	// dont have any default value for edges 
	// it should compute it everytime value get updated
	adjustment_input_numofedges =
	    gtk_adjustment_new(DEFAULT_NUMOFEDGES, DEFAULT_NUMOFEDGES, ZERO,
			       ZERO, ZERO, ZERO);
	adjustment_input_numofants =
	    gtk_adjustment_new(DEFAULT_NUMOFANTS, MIN_NUMOFANTS, MAX_NUMOFANTS,
			       ANT_CLIMB_RATE, ZERO, ZERO);
	adjustment_evaporation_rate =
	    gtk_adjustment_new(DEFAULT_EVAPORATION_RATE, MIN_EVAPORATION_RATE,
			       MAX_EVAPORATION_RATE, EVAPORATION_INCREMENT_RATE, ZERO,
			       ZERO);
	adjustment_input_numofcycle =
		gtk_adjustment_new(DEFAULT_NUMOFCYCLE,MIN_NUMOFCYCLE,
				   MAX_NUMOFCYCLE,NUMOFCYCLE_INCREMENT_RATE, ZERO,
				   ZERO);
	// input_numofvertices
	gtk_spin_button_set_adjustment(input_numofvertices,
				       adjustment_input_numofvertices);
	gtk_spin_button_set_value(input_numofvertices,MIN_NUMOFVERTICES);
	
	// input_numofedges
	gtk_spin_button_set_adjustment(input_numofedges,
				       adjustment_input_numofedges);
	gtk_spin_button_set_value(input_numofedges,DEFAULT_NUMOFEDGES);

	// input_numofants
	gtk_spin_button_set_adjustment(input_numofants,
				       adjustment_input_numofants);
	gtk_spin_button_set_value(input_numofants,DEFAULT_NUMOFANTS);

	// input_evaporation_rate
	gtk_spin_button_set_adjustment(input_evaporation_rate,
				       adjustment_evaporation_rate);
	gtk_spin_button_set_value(input_evaporation_rate,DEFAULT_EVAPORATION_RATE);
	
	// input_numofcycle
	gtk_spin_button_set_adjustment(input_numofcycle,
				       adjustment_input_numofcycle);
	gtk_spin_button_set_value(input_numofcycle,DEFAULT_NUMOFCYCLE);
	
	// setting some options of graphwin and mainwin
	gtk_window_set_title(GTK_WINDOW(mainwin), mainwin_title);
	gtk_window_set_title(GTK_WINDOW(graphwin), graphwin_title);
	gtk_widget_set_size_request(graphwin, GRAPH_WIN_WIDTH,
				    GRAPH_WIN_HEIGHT);
	gtk_widget_set_size_request(mainwin, minimum_size_for_frame_ui->width,
				    minimum_size_for_frame_ui->height);
	gtk_window_set_resizable(GTK_WINDOW(mainwin), RESIZABLE);
	gtk_window_set_resizable(GTK_WINDOW(graphwin), RESIZABLE);
	frame = gtk_frame_new(NULL);
	// gtk_frame_set_shadow only accept frame as first argumant , should not send window
	gtk_frame_set_shadow_type(GTK_FRAME(frame), GTK_SHADOW_IN);
	gtk_container_add(GTK_CONTAINER(graphwin), frame);
	drawing_area = gtk_drawing_area_new();
	// enabling tooltip for drawing area
	gtk_widget_set_has_tooltip(drawing_area,TRUE);
	gtk_container_add(GTK_CONTAINER(frame), drawing_area);

	// adding BUTTON PRESS AND MOTION MASK to gtk events
	gtk_widget_set_events(drawing_area,
			      gtk_widget_get_events(drawing_area) | GDK_BUTTON_PRESS_MASK);

	// widget_array initialize
	// passing array of {mode,GtkWidget(input_numofvertices),GtkWidget(input_numofedges)} to singal for GtkToggleButton
	// MODE_INDEX 0
	g_ptr_array_add(widget_array, &mode);
	// INPUT_NUMOFVERTICES_INDEX = 1
	g_ptr_array_add(widget_array, input_numofvertices);
	// INPUT_NUMOFEDGES_INDEX = 2
	g_ptr_array_add(widget_array, input_numofedges);
	// INPUT_BUTTON_GENERATING_RANDOM_GRAPH = 3
	g_ptr_array_add(widget_array, button_generating_random_graph);
	// GRAPHWIN_INDEX = 4
	g_ptr_array_add(widget_array, graphwin);
	// DRAWING_AREA_INDEX = 5
	g_ptr_array_add(widget_array, drawing_area);
	// MAINWIN_INDEX = 6
	g_ptr_array_add(widget_array, mainwin);
	// INPUT_EVAPORATION_RATE_INDEX = 7
	g_ptr_array_add(widget_array, input_evaporation_rate);
	// PROGRESSBAR_INDEX = 8
	g_ptr_array_add(widget_array, progressbar);
	// STATUS_INDEX = 9
	g_ptr_array_add(widget_array, &status);
	
	// signals
	g_signal_connect(mainwin, EVENT_DESTROY, G_CALLBACK(gtk_main_quit),
			 NULL);
	g_signal_connect(graphwin, EVENT_DESTROY, G_CALLBACK(gtk_main_quit),
			 NULL);
	g_signal_connect(graphwin, EVENT_SHOW,
			 G_CALLBACK(on_show_event), NULL);
	g_signal_connect(drawing_area, EVENT_BUTTON_PRESS_EVENT,
			 G_CALLBACK(on_button_press_event), &mode);
	g_signal_connect(drawing_area, EVENT_DRAW, G_CALLBACK(on_draw), NULL);
	g_signal_connect(drawing_area, EVENT_QUERY_TOOLTIP,G_CALLBACK(on_query_tooltip),NULL);
	g_signal_connect(mainwin, EVENT_DELETE_EVENT,
			 G_CALLBACK(on_delete_event), NULL);
	g_signal_connect(mainwin,EVENT_DELETE_EVENT,G_CALLBACK(on_delete_event),widget_array);
	g_signal_connect(graphwin,EVENT_DELETE_EVENT,G_CALLBACK(on_delete_event),widget_array);
	
	// signal for object loaded from builder
	g_signal_connect(button_drawing_mode, EVENT_TOGGLED,
			 G_CALLBACK(on_toggled_button_drawing_mode),
			 widget_array);
	g_signal_connect(button_solve_problem, EVENT_CLICKED,
			 G_CALLBACK(on_clicked_button_solve_problem),
			 widget_array);
	g_signal_connect(button_generating_random_graph, EVENT_CLICKED,
			 G_CALLBACK(on_clicked_button_generating_random_graph),
			 widget_array);
	g_signal_connect(input_numofvertices, EVENT_VALUE_CHANGED,
			 G_CALLBACK(on_value_changed_input_numofvertices),
			 input_numofedges);
	g_signal_connect(input_numofants, EVENT_VALUE_CHANGED,
			 G_CALLBACK(on_value_changed_input_numofants),
			 NULL);
	g_signal_connect(input_evaporation_rate, EVENT_VALUE_CHANGED,
			 G_CALLBACK(on_value_chenged_evaporation_rate),
			 NULL);
	g_signal_connect(input_numofcycle, EVENT_VALUE_CHANGED,
			 G_CALLBACK(on_value_chenged_input_numofcycle),
			 NULL);

	// g_signal_connect(graphwin,"clicked",G_CALLBACK(graph_window_clicked),NULL);
	gtk_container_set_border_width(GTK_CONTAINER(graphwin),
				       GRAPH_WIN_CONTAINER_BORDER_WIDTH);

	gtk_widget_show(mainwin);
	gtk_widget_show_all(graphwin);

	gtk_main();
	return 0;
}
