// we should minimum using global variable , and reimplement this via second parameter
static int euclidean_distance(GdkPoint *a,GdkPoint *b)
{
#ifdef DEBUG
	g_debug("in euclidean_distance function");
	if(a == NULL || b == NULL)
		g_error("euclidean_distance function:a or b is null");
#endif
	g_debug("euclidean_distance:(%d,%d)->(%d,%d)",a->x,a->y,b->x,b->y);
	return sqrt( pow(abs(a->x - b->x) , 2 ) + pow( abs( a->y - b->y ) , 2 ) );
}

static void on_value_changed_dialog_input_numofvertices(GtkSpinButton *widget,
							gpointer data)
{
	numofvertices = gtk_spin_button_get_value_as_int(widget);
	g_debug("on_value_changed_dialog_input_numofvertices:numofvertices is %d",numofvertices);
}

static void draw_rectangle(GtkWidget * widget, GdkPoint *item_loc,
			   gboolean status)
{
	cairo_t *cr;
	g_debug("drawing here : (%d,%d)", item_loc->x, item_loc->y);
	if (!surface)
		g_error("surface is null , fatal error in line:%d", __LINE__);
	cr = cairo_create(surface);
	if (status)
		cairo_set_source_rgb(cr, RED, FALSE, FALSE);
	else
		cairo_set_source_rgb(cr, FALSE, FALSE, FALSE);
	cairo_arc(cr, item_loc->x, item_loc->y, RADIUS, FIRST_ANGLE,
		  SECOND_ANGLE);
	cairo_fill(cr);
	cairo_destroy(cr);
	gtk_widget_queue_draw(widget);
}

static void draw_line(GtkWidget * widget,gboolean status)
{
	cairo_t *cr;
	g_debug("draw_line:(%d,%d)->(%d,%d)", bag.first_item->x,
		bag.first_item->y, bag.second_item->x, bag.second_item->y);
	if (!surface)
		g_error("draw_line:surface is null , fatal error in line:%d",
			__LINE__);
	cr = cairo_create(surface);
	if(!status){
		cairo_set_line_width(cr, LINE_WIDTH_UNSELECTED);
		cairo_set_source_rgb(cr, FALSE, FALSE, FALSE);
	}
	else{
		cairo_set_line_width(cr, LINE_WIDTH_SELECTED);
		cairo_set_source_rgb(cr, RED, FALSE, FALSE);
	}
	g_debug("draw_line:line width : %f", cairo_get_line_width(cr));
	cairo_move_to(cr, bag.first_item->x, bag.first_item->y);
	cairo_line_to(cr, bag.second_item->x, bag.second_item->y);
	cairo_stroke(cr);
	cairo_destroy(cr);
	gtk_widget_queue_draw(widget);
}

void free_allocated(){
	g_debug("free_allocated:ui_points->len is %d",ui_points->len);
	bag.first_item = NULL;
	bag.second_item = NULL;
	bag.first_item_index = -1;
	if(!ui_points->len)
		return;
	for(int i = (ui_points->len - 1); i >= 0; i--){
		GdkPoint *temp_item = g_array_index(ui_points, GdkPoint* , i);
		g_array_remove_index(ui_points, i);
		free(temp_item);
	}
}

// return random bool by probability
// we use this for improving distributing of edges on vertices
static gboolean next_bool(double probability)
{
	return (rand() / (double)RAND_MAX) < probability;
}

static void clear_surface(void)
{
	g_debug("on_clear_surface");
	cairo_t *cr;
	cr = cairo_create(surface);
	cairo_set_source_rgb(cr, RGB_SURFACE, RGB_SURFACE, RGB_SURFACE);
	cairo_paint(cr);
	cairo_destroy(cr);
}

#if !defined(ONLYUI)
// this function is just only for debugging purpose
// for example in this function len of edge ignored
// and just draw edge and vertices
static void visualize_graph(GtkWidget *drawing_area){
	g_debug("visualize_graph with graph size : %d",graph->numofvertices);
	free_allocated();
	clear_surface();
	gtk_widget_queue_draw(drawing_area);
	GtkAllocation drawing_area_alloc;
	gtk_widget_get_allocation(drawing_area,&drawing_area_alloc);
	for(int i = 0; i < graph->numofvertices; i++){
		GdkPoint *item_loc = malloc(sizeof(GdkPoint));
		item_loc->x = rand() % (drawing_area_alloc.width - DISTANCE_FROM_BORDER);
		item_loc->y = rand() % (drawing_area_alloc.height - DISTANCE_FROM_BORDER);
		// adding DISTANCE
		// just for make sure there is DISTANCE between item_loc and border of drawing_area
		item_loc->x = item_loc->x + DISTANCE;
		item_loc->y = item_loc->y + DISTANCE;
		g_array_append_val(ui_points,item_loc);
		// after implementing Graph interface , we should add this edge to that interface too
		draw_rectangle(drawing_area,item_loc,FALSE);
		// we go through 0 until ((i*m)-i)/2) = (ui_points->len)
		// for checking if we have connected vertices
		// for example in third row case
		// we go from 0,1,2 and checking if we have connected vertices or not
		for(int j = 0; j < i; j++){
			if(!is_connected(graph,i,j))
				continue;
			bag.first_item = item_loc;
			bag.first_item_index = i;
			bag.second_item = g_array_index(ui_points, GdkPoint*, j);
			if(is_selected(graph,i,j))
				draw_line(drawing_area,TRUE);
			else
				draw_line(drawing_area,FALSE);
		}
	}
	
}
#endif

// memory leak
// should free all allocated memory for points
static void on_toggled_button_drawing_mode(GtkToggleButton * button,
					   GPtrArray *widget_array)
{
	g_debug("on_toggled_button_drawing_mode");
	free_allocated();
	gboolean g = gtk_toggle_button_get_active(button);
	GtkWidget *input_numofvertices, *input_numofedges , *button_generating_random_graph, *drawing_area, *dialog, *mainwin; 
	_mode *mode = g_ptr_array_index(widget_array, MODE_INDEX);
	input_numofvertices =
	    g_ptr_array_index(widget_array, INPUT_NUMOFVERTICES_INDEX);
	input_numofedges = g_ptr_array_index(widget_array, INPUT_NUMOFEDGES_INDEX);
	button_generating_random_graph = g_ptr_array_index(widget_array,BUTTON_GENERATING_RANDOM_GRAPH_INDEX);
	drawing_area = g_ptr_array_index(widget_array,DRAWING_AREA_INDEX);
	mainwin = g_ptr_array_index(widget_array,MAINWIN_INDEX);
	if (g){
		*mode = DRAWING_MODE;
		dialog = gtk_dialog_new_with_buttons(dialog_title,mainwin,
						     GTK_DIALOG_MODAL,
						     dialog_first_button_text,
						     GTK_RESPONSE_OK,
						     NULL);
		g_signal_connect_swapped(dialog,EVENT_RESPONSE,G_CALLBACK(gtk_widget_destroy),dialog);
		GtkWidget *content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
		GtkAdjustment *adjustment_input_numofvertices = gtk_adjustment_new(MIN_NUMOFVERTICES,
										  MIN_NUMOFVERTICES,
										  MAX_NUMOFVERTICES,
										  INCREMENT_RATE, ZERO, ZERO);
		numofvertices = MIN_NUMOFVERTICES;
		GtkWidget *dialog_input_numofvertices = gtk_spin_button_new(adjustment_input_numofvertices,CLIMB_RATE,ZERO);
		gtk_container_add(GTK_CONTAINER(content),dialog_input_numofvertices);
		gtk_dialog_set_default_response(dialog,GTK_RESPONSE_OK);
		gtk_window_set_resizable(GTK_WINDOW(dialog), RESIZABLE);
		gtk_widget_show_all(dialog);
		g_signal_connect(dialog_input_numofvertices, EVENT_VALUE_CHANGED,
				 G_CALLBACK(on_value_changed_dialog_input_numofvertices),
				 NULL);
		int result = gtk_dialog_run(dialog);
		g_debug("result is %d and numofvertices is %d",result,numofvertices);
	}
	else
		*mode = RANDOM_MODE;
	gtk_widget_set_sensitive(input_numofvertices, !g);
	gtk_widget_set_sensitive(input_numofedges, !g);
	gtk_widget_set_sensitive(button_generating_random_graph, !g);
	clear_surface();
	gtk_widget_queue_draw(drawing_area);
#if !defined(ONLYUI)
		if(graph)
			free_graph(graph);
		graph = NULL;
#endif
}

static gboolean on_draw(GtkWidget * widget, cairo_t * cr, gpointer data)
{
	g_debug("on_draw");
	cairo_set_source_surface(cr, surface, 0, 0);
	cairo_paint(cr);
	return FALSE;
}

static gboolean on_show_event(GtkWidget * widget,
			      gpointer data)
{
	g_debug("on_configure_event");

	if (surface)
		cairo_surface_destroy(surface);
	surface =
	    gdk_window_create_similar_surface(gtk_widget_get_window(widget),
					      CAIRO_CONTENT_COLOR,
					      gtk_widget_get_allocated_width
					      (widget),
					      gtk_widget_get_allocated_height
					      (widget));
	clear_surface();
	return TRUE;
}

static gboolean on_delete_event(GtkWidget * widget, GdkEvent * event,
				gpointer data)
{
	g_debug("on_delete_event");
	/* ask from user , if he wants really quit then return FALSE , otherwise return TRUE , OK? */

	return FALSE;
}

static void on_clicked_button_generating_random_graph(GtkWidget * widget,
						      GPtrArray * widget_array)
{
	g_debug
	    ("on_clicked_button_generating_random_graph");
	GtkWidget *input_numofvertices, *input_numofedges, *button_generating_random_graph, *graphwin, *drawing_area;
	GtkAllocation drawing_area_alloc;
	gint numofedges_drew = 0;
	input_numofvertices = g_ptr_array_index(widget_array,INPUT_NUMOFVERTICES_INDEX);
	input_numofedges = g_ptr_array_index(widget_array,INPUT_NUMOFEDGES_INDEX);
	graphwin = g_ptr_array_index(widget_array,GRAPHWIN_INDEX);
	drawing_area = g_ptr_array_index(widget_array,DRAWING_AREA_INDEX);
	numofvertices = gtk_spin_button_get_value_as_int(input_numofvertices);
	numofedges = gtk_spin_button_get_value_as_int(input_numofedges);
	g_debug("on_clicked_button_generating_random_graph:numofvertices is %d , numofedges is %d", numofvertices, numofedges);
	// just clear surface and re-draw
	clear_surface();
	gtk_widget_queue_draw(drawing_area);
	gtk_widget_get_allocation(drawing_area,&drawing_area_alloc);
	free_allocated();
        #if !defined(ONLYUI)
	g_debug("before freeing graph");
	if(graph)
		free_graph(graph);
	graph = NULL;
	g_debug("initialize graph with %d vertices",numofvertices);
	graph = initialize(numofvertices);
        #endif
	srand(time(NULL));
	double probability = (double) numofedges/((numofvertices * (numofvertices - 1)) / 2);
	for(int i = 0; i < numofvertices; i++)
	{
		GdkPoint *item_loc = malloc(sizeof(GdkPoint));
		item_loc->x = rand() % (drawing_area_alloc.width - DISTANCE_FROM_BORDER);
		item_loc->y = rand() % (drawing_area_alloc.height - DISTANCE_FROM_BORDER);
		// adding DISTANCE
		// just for make sure there is DISTANCE between item_loc and border of drawing_area
		item_loc->x = item_loc->x + DISTANCE;
		item_loc->y = item_loc->y + DISTANCE;
		g_array_append_val(ui_points,item_loc);
		// after implementing Graph interface , we should add this edge to that interface too
		draw_rectangle(drawing_area,item_loc,FALSE);
		gboolean connectivity = FALSE;
		for(int j = 0; j < (ui_points->len - 1); j++){
			double having_edge = next_bool(probability);
			if(numofedges_drew >= numofedges)
				break;
			if(having_edge == ZERO)
				continue;
			// after implementing Graph interface , we should add this edge to that interface too
			numofedges_drew++;
			connectivity = TRUE;
			bag.first_item = item_loc;
			bag.first_item_index = i;
			bag.second_item = g_array_index(ui_points, GdkPoint*, j);
			draw_line(drawing_area,FALSE);
			#if !defined(ONLYUI)
			// initializing graph's line
			int distance = euclidean_distance(bag.first_item,bag.second_item);
			struct _edge *edge = create_edge(distance,ZERO);
			connect_edge(graph,i,j,edge);
			#endif
		}
		if(!connectivity && i != 0){
			int index = rand() % i;
			numofedges_drew++;
			bag.first_item = item_loc;
			bag.first_item_index = i;
			bag.second_item = g_array_index(ui_points, GdkPoint*, index);
			draw_line(drawing_area,FALSE);
			
			#if !defined(ONLYUI)
			int distance = euclidean_distance(bag.first_item,bag.second_item); // initializing graph's line
			struct _edge *edge = create_edge(distance,ZERO);
			connect_edge(graph,i,index,edge);
			#endif
		}
	}
	numofedges = numofedges_drew;
}

static void on_clicked_button_solve_problem(GtkWidget * widget, GPtrArray *widget_array)
{
	g_debug("clicked on problem solve button, not implemented yet");
	// TODO
	// not implement yet
	// just for testing purpose
	// for now we clear all drawing area and redraw all graph
	// we want check our graph Implementation
       
#if !defined(ONLYUI)
	GtkWidget *drawing_area = g_ptr_array_index(widget_array,DRAWING_AREA_INDEX);
	visualize_graph(drawing_area);
#endif
}

// we send Mode as pointer in gpointer (3rd)
static gboolean on_button_press_event(GtkWidget * widget,
				      GdkEventButton * event, gpointer pmode)
{
	g_debug("on_button_press_event:(%d,%d) and ui_points->len is %d and numofvetices is %d", (int)event->x, (int)event->y, ui_points->len, numofvertices);
	_mode *mode = pmode;
	if (*mode == DRAWING_MODE) {
		// if graph is null then this is first point and we should initialize graph
#if !defined(ONLYUI)
		if(!graph)
			graph = initialize(numofvertices);
#endif
		// carefull if we dont want append this item , we should free space allocated for it
		// otherwize we should free it when we generating random graph
		GdkPoint *item_loc = malloc(sizeof(GdkPoint));
		item_loc->x = (int)event->x;
		item_loc->y = (int)event->y;
		for (int i = 0; i < ui_points->len; i++ ) {
			GdkPoint *temp_item = g_array_index(ui_points, GdkPoint* , i);
			if (abs(item_loc->x - temp_item->x) < DISTANCE_CLICK
			    && abs(item_loc->y - temp_item->y) < DISTANCE_CLICK) {
				free(item_loc);
				// we dont have any vertex selected
				if (bag.first_item == NULL
				    && bag.second_item == NULL) {
					draw_rectangle(widget, temp_item, TRUE);
					bag.first_item = temp_item;
					bag.first_item_index = i;
				}
				// draw line , we a have a vertex selected already
				else {
					bag.second_item = temp_item;
#if !defined(ONLYUI)
					// check it out if there is edge or not
					// if there is edge this is memory leak and should be avoid by an if
					if(bag.first_item_index !=i  && is_connected(graph,bag.first_item_index,i) == NULL){
						g_debug("in_button_press_press_event:adding edge with first_index:%d,and second_index:%d",bag.first_item_index,i);
						int distance = euclidean_distance(bag.first_item,bag.second_item);
						g_debug("distance is :%d",distance);
						struct _edge *edge = create_edge(distance,ZERO);
						connect_edge(graph,bag.first_item_index,i,edge);
						// turning second selected vertex to red
						draw_rectangle(widget, bag.first_item, FALSE);
						draw_line(widget,FALSE);
						bag.first_item = bag.second_item;
						bag.first_item_index = i;
						draw_rectangle(widget, temp_item, TRUE);
						bag.second_item = NULL;
					}else{
						// we have already edge or we select selected vertex
						draw_rectangle(widget, bag.first_item, FALSE);
						bag.first_item = bag.second_item;
						bag.first_item_index = i;
						draw_rectangle(widget, temp_item, TRUE);
						bag.second_item = NULL;
					}
#else
					// turning second selected vertex to red
					draw_rectangle(widget, bag.first_item, FALSE);
					draw_line(widget,FALSE);
					bag.first_item = bag.second_item;
					bag.first_item_index = i;
					draw_rectangle(widget, temp_item, TRUE);
					bag.second_item = NULL;
#endif
				}
				return TRUE;
			}
		}
		// we check if we have any selected item then free item and just return
		if (bag.first_item != NULL) {
			g_debug
			    ("on_button_press_event:bag.first_item is selected but second item , not select currectly");
			draw_rectangle(widget, bag.first_item, FALSE);
			bag.first_item = NULL;
			bag.first_item_index = -1;
			bag.second_item = NULL;
			free(item_loc);
			return TRUE;
		}
		// drawing rectangle only if we have allowed
		if(numofvertices > ui_points->len)
		{
			draw_rectangle(widget, item_loc, FALSE);
			g_array_append_val(ui_points, item_loc);
			g_debug
				("on_button_press_event:number of elements in ui_points : %d",
				 ui_points->len);
		}
	}

	return TRUE;
}

static void on_value_changed_input_numofvertices(GtkSpinButton * widget,
						 gpointer data)
{
	g_debug("on_value_changed_input_numofvertices");
	free_allocated();
	GtkSpinButton *input_numofedges = data;
	int min_numofedges = gtk_spin_button_get_value_as_int(widget);
	numofvertices = min_numofedges; // just setting numofvertices to value of input_numofvertices
	int max_numofedges = ((min_numofedges * (min_numofedges - 1)) / 2);
	g_debug("on_value_changed_input_numofvertices:numofvertices:%d max_numofedges:%d",
		numofvertices,
		max_numofedges);
	GtkAdjustment *adjustment_input_numofedges =
	    gtk_adjustment_new(min_numofedges, min_numofedges, max_numofedges,
			       INCREMENT_RATE, ZERO, ZERO);
	gtk_spin_button_set_adjustment(input_numofedges,
				       adjustment_input_numofedges);
	gtk_spin_button_set_value(input_numofedges, min_numofedges);
}
