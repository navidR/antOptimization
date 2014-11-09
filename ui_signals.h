#ifndef UI_SIGNALS_H
#define UI_SIGNALS_H
// we should minimum using global variable , and reimplement this via second parameter


// check if graph have
// an vertex with only one edge
static bool check_graph(){
#ifdef DEBUG	
	g_debug("checking_graph");
#endif
	for(int i = 0;i < graph->numofvertices; i++){
		int num_connection = 0;
		for(int j = 0; j < graph->numofvertices ; j++){
			if(j == i)
				continue;
			if(is_connected(graph,i,j) != NULL)
				num_connection++;
		}
		if(num_connection < 2)
			return false;
	}
	return true;
}


static int euclidean_distance(GdkPoint *a,GdkPoint *b)
{
#ifdef DEBUG
	g_debug("euclidean_distance function");
	if(a == NULL || b == NULL)
		g_error("euclidean_distance function:a or b is null");
#endif
	return sqrt( pow(abs(a->x - b->x) , 2 ) + pow( abs( a->y - b->y ) , 2 ) );
}

static void on_value_changed_dialog_input_numofvertices(GtkSpinButton *widget,
							gpointer data)
{
	numofvertices = gtk_spin_button_get_value_as_int(widget);
#ifdef DEBUG	
	g_debug("on_value_changed_dialog_input_numofvertices:numofvertices is %d",numofvertices);
#endif
}

static void draw_rectangle(GtkWidget * widget, GdkPoint *item_loc,
			   gboolean status, int num)
{
	cairo_t *cr;
//	g_debug("drawing here : (%d,%d) with num:%d", item_loc->x, item_loc->y,num);
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
	if(numofvertices < MAX_NUMOF_VERTEX_WILL_SHOW){
		int x , y;
// x
		if(item_loc->x > (DRAWING_AREA_WIDTH/2))
			x = item_loc->x + TEXT_DISTANCE;
		else
			x = item_loc->x - TEXT_DISTANCE;
// y
		if(item_loc->y > (DRAWING_AREA_HEIGHT/2))
			y = item_loc->y + TEXT_DISTANCE;
		else
			y = item_loc->y - TEXT_DISTANCE;
		cairo_move_to(cr,x,y);
		char text[5];
		sprintf(text,"%d",num);
		cairo_show_text(cr , text);
	}
	cairo_destroy(cr);
	gtk_widget_queue_draw(widget);
}

static void draw_line(GtkWidget * widget,gboolean status)
{
	cairo_t *cr;
//	g_print("draw_line:(%d,%d)->(%d,%d) : numofvertices:%d\n", bag.first_item->x,bag.first_item->y, bag.second_item->x, bag.second_item->y,numofvertices);
	if (!surface)
		g_error("draw_line:surface is null , fatal error in line:%d",
			__LINE__);
	cr = cairo_create(surface);
	if(!status && numofvertices < MAX_NUMOFVERTICES_DRAWING){
		cairo_set_line_width(cr, LINE_WIDTH_UNSELECTED);
		cairo_set_source_rgb(cr, FALSE, FALSE, FALSE);
	}
	else if(status){
		cairo_set_line_width(cr, LINE_WIDTH_SELECTED);
		cairo_set_source_rgb(cr, RED, FALSE, FALSE);
	}
	else{
		cairo_destroy(cr);
		return;
	}
	cairo_move_to(cr, bag.first_item->x, bag.first_item->y);
	cairo_line_to(cr, bag.second_item->x, bag.second_item->y);
	cairo_stroke(cr);

	// show text length
	if(numofvertices < MAX_NUMOF_EDGE_WILL_SHOW){
		cairo_set_source_rgb(cr, FALSE, FALSE, FALSE);
		cairo_move_to(cr, ((bag.first_item->x + bag.second_item->x)/2) + TEXT_LINE_DISTANCE,
			      ((bag.first_item->y + bag.second_item->y)/2)+ TEXT_LINE_DISTANCE);
		char text[10];
		int distance = euclidean_distance(bag.first_item,bag.second_item);
		sprintf(text,"%d",distance);
		cairo_show_text(cr , text);
	}
	cairo_destroy(cr);
	gtk_widget_queue_draw(widget);
}

void free_allocated(){
#ifdef DEBUG
	g_debug("free_allocated:ui_points->len is %d",ui_points->len);
#endif
	lenofanswer = ZERO;
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
#ifdef DEBUG
	g_debug("on_clear_surface");
#endif
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
// TODO
// not implement yet
// just for testing purpose
// for now we clear all drawing area and redraw all graph
// we want check our graph Implementation
static void visualize_graph(GtkWidget *drawing_area){
#ifdef DEBUG
	g_debug("visualize_graph with graph size : %d",graph->numofvertices);
#endif
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
		draw_rectangle(drawing_area,item_loc,FALSE,i);
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
			bag.second_item_index = j;
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
#ifdef DEBUG
	g_debug("on_toggled_button_drawing_mode");
#endif
	free_allocated();
	gboolean g = gtk_toggle_button_get_active(button);
	GtkWidget *input_numofvertices, *input_numofedges , *button_generating_random_graph, *drawing_area, *mainwin; 
	_mode *mode = g_ptr_array_index(widget_array, MODE_INDEX);
	input_numofvertices =
	    g_ptr_array_index(widget_array, INPUT_NUMOFVERTICES_INDEX);
	input_numofedges = g_ptr_array_index(widget_array, INPUT_NUMOFEDGES_INDEX);
	button_generating_random_graph = g_ptr_array_index(widget_array,BUTTON_GENERATING_RANDOM_GRAPH_INDEX);
	drawing_area = g_ptr_array_index(widget_array,DRAWING_AREA_INDEX);
	mainwin = g_ptr_array_index(widget_array,MAINWIN_INDEX);
	if (g){
		*mode = DRAWING_MODE;
		GtkWidget *dialog = gtk_dialog_new_with_buttons(dialog_title,mainwin,
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
#ifdef DEBUG
		g_debug("result is %d and numofvertices is %d",result,numofvertices);
#endif
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
	cairo_set_source_surface(cr, surface, 0, 0);
	cairo_paint(cr);
	return FALSE;
}

static gboolean on_show_event(GtkWidget * widget,
			      gpointer data)
{
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
				gpointer widget_array)
{
	condition = false;
	return FALSE;
}

static void on_clicked_button_generating_random_graph(GtkWidget * widget,
						      GPtrArray * widget_array)
{
	GtkWidget *input_numofvertices, *input_numofedges, *drawing_area,*input_evaporation_rate;
	GtkAllocation drawing_area_alloc;
	gint numofedges_drew = 0;
	input_numofvertices = g_ptr_array_index(widget_array,INPUT_NUMOFVERTICES_INDEX);
	input_numofedges = g_ptr_array_index(widget_array,INPUT_NUMOFEDGES_INDEX);
	input_evaporation_rate = g_ptr_array_index(widget_array,INPUT_EVAPORATION_RATE_INDEX);
	drawing_area = g_ptr_array_index(widget_array,DRAWING_AREA_INDEX);
	numofvertices = gtk_spin_button_get_value_as_int(input_numofvertices);
	numofedges = gtk_spin_button_get_value_as_int(input_numofedges);
	// just clear surface and re-draw
	clear_surface();
	gtk_widget_queue_draw(drawing_area);
	gtk_widget_get_allocation(drawing_area,&drawing_area_alloc);
	free_allocated();
#if !defined(ONLYUI)
	if(graph)
		free_graph(graph);
	graph = NULL;
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
		draw_rectangle(drawing_area,item_loc,FALSE,i);
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
			bag.second_item_index = j;
			draw_line(drawing_area,FALSE);
			#if !defined(ONLYUI)
			// initializing graph's line
			int distance = euclidean_distance(bag.first_item,bag.second_item);
			struct _edge *edge = create_edge(distance);
			connect_edge(graph,i,j,edge);
			#endif
		}
		if(!connectivity && i != 0){
			int index = rand() % i;
			numofedges_drew++;
			bag.first_item = item_loc;
			bag.first_item_index = i;
			bag.second_item = g_array_index(ui_points, GdkPoint*, index);
			bag.second_item_index = index;
			draw_line(drawing_area,FALSE);
			
#if !defined(ONLYUI)
			int distance = euclidean_distance(bag.first_item,bag.second_item); // initializing graph's line
			struct _edge *edge = create_edge(distance);
			connect_edge(graph,i,index,edge);
#endif
		}
	}
	numofedges = numofedges_drew;
}

static void redraw(GPtrArray *widget_array){
	GtkWidget *drawing_area ;
	drawing_area = g_ptr_array_index(widget_array,DRAWING_AREA_INDEX);
	for(int i = 0 ; i < ui_points->len ; i++){
		draw_rectangle(drawing_area,g_array_index(ui_points,GdkPoint*,i) , TRUE, i);
		for(int j = 0;j < i;j++){
			if(is_connected(graph,i,j) != NULL){
				bag.first_item = g_array_index(ui_points, GdkPoint* , i);
				bag.first_item_index = i;
				bag.second_item = g_array_index(ui_points, GdkPoint* ,j);
				bag.second_item_index = j;
				if(is_selected(graph,i,j))				
					draw_line(drawing_area,TRUE);
				else
					draw_line(drawing_area,FALSE);
			}
		}
	}
}

static void on_clicked_button_solve_problem(GtkWidget * widget,
				GPtrArray *widget_array)
{
	if(!check_graph()){
		GtkWidget *mainwin = g_ptr_array_index(widget_array,MAINWIN_INDEX);
		GtkWidget *dialog = gtk_dialog_new_with_buttons(dialog_wrong_graph_title,
								mainwin,
								GTK_DIALOG_MODAL,
								dialog_first_button_text,
								GTK_RESPONSE_OK,
								NULL);
		g_signal_connect_swapped(dialog,EVENT_RESPONSE,G_CALLBACK(gtk_widget_destroy),dialog);
		GtkWidget *label = gtk_label_new(dialog_wrong_graph_text);
		GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
		gtk_container_add(GTK_CONTAINER(content_area),label);
		gtk_widget_show_all(dialog);
		return;
	}
	_status *status = g_ptr_array_index(widget_array,STATUS_INDEX);
	GtkProgressBar *progressbar = g_ptr_array_index(widget_array,PROGRESSBAR_INDEX);
	if(*status == STOP){
		*status = RUN;
		condition = true;
		gtk_button_set_label(widget,button_cancel_problem_text);
		gtk_widget_set_sensitive(progressbar,TRUE);
		clear(graph);
		clear_surface();
		redraw(widget_array);
		struct _tsp_solver* tsp_solver =  init_tsp_solver(graph,numofants,numofcycle,&condition);
		lenofanswer = solve_tsp(graph,tsp_solver,evaporation_rate,progressbar);
		freeing_tsp_solver(tsp_solver);
		redraw(widget_array);
		gtk_widget_set_sensitive(progressbar,FALSE);
	}
	*status = STOP;
	condition = false;
	gtk_button_set_label(widget,button_solve_problem_text);
	gtk_progress_bar_set_fraction(progressbar,ZERO);
}

/*
 * for visualizing graph we can change solve button handler to this:
 * #if !defined(ONLYUI)
 * GtkWidget *drawing_area = g_ptr_array_index(widget_array,DRAWING_AREA_INDEX);
 * visualize_graph(drawing_area);
 * #endif
*/

// we send Mode as pointer in gpointer (3rd)
static gboolean on_button_press_event(GtkWidget * widget,
				      GdkEventButton * event, gpointer pmode)
{
#ifdef DEBUG
	g_debug("on_button_press_event:(%d,%d) and ui_points->len is %d and numofvetices is %d", (int)event->x, (int)event->y, ui_points->len, numofvertices);
#endif
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
					draw_rectangle(widget, temp_item, TRUE,i);
					bag.first_item = temp_item;
					bag.first_item_index = i;
				}
				// draw line , we a have a vertex selected already
				else {
					bag.second_item = temp_item;
					bag.second_item_index = i;
#if !defined(ONLYUI)
					// check it out if there is edge or not
					// if there is edge this is memory leak and should be avoid by an if
					if(bag.first_item_index !=i  && is_connected(graph,bag.first_item_index,i) == NULL){
						int distance = euclidean_distance(bag.first_item,bag.second_item);
						struct _edge *edge = create_edge(distance);
						connect_edge(graph,bag.first_item_index,i,edge);
						// turning second selected vertex to red
						draw_rectangle(widget, bag.first_item, FALSE, bag.first_item_index);
						draw_line(widget,FALSE);
						bag.first_item = bag.second_item;
						bag.first_item_index = i;
						draw_rectangle(widget, temp_item, TRUE, bag.first_item_index);
						bag.second_item = NULL;
						bag.second_item_index = NULL;
					}else{
						// we have already edge or we select selected vertex
						draw_rectangle(widget, bag.first_item, FALSE , bag.first_item_index);
						bag.first_item = bag.second_item;
						bag.first_item_index = i;
						draw_rectangle(widget, temp_item, TRUE, bag.first_item_index);
						bag.second_item = NULL;
						bag.second_item_index = NULL;
					}
#else
					// turning second selected vertex to red
					draw_rectangle(widget, bag.first_item, FALSE, bag.first_item_index);
					draw_line(widget,FALSE);
					bag.first_item = bag.second_item;
					bag.first_item_index = i;
					draw_rectangle(widget, temp_item, TRUE, bag.first_item_index);
					bag.second_item = NULL;
					bag.second_item_index = NULL;
#endif
				}
				return TRUE;
			}
		}
		// we check if we have any selected item then free item and just return
		if (bag.first_item != NULL) {
			draw_rectangle(widget, bag.first_item, FALSE , bag.first_item_index);
			bag.first_item = NULL;
			bag.first_item_index = -1;
			bag.second_item = NULL;
			bag.second_item_index = NULL;
			free(item_loc);
			return TRUE;
		}
		// drawing rectangle only if we have allowed
		if(numofvertices > ui_points->len)
		{
			draw_rectangle(widget, item_loc, FALSE, ui_points->len);
			g_array_append_val(ui_points, item_loc);
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
	GtkAdjustment *adjustment_input_numofedges =
	    gtk_adjustment_new(min_numofedges, min_numofedges, max_numofedges,
			       INCREMENT_RATE, ZERO, ZERO);
	gtk_spin_button_set_adjustment(input_numofedges,
				       adjustment_input_numofedges);
	gtk_spin_button_set_value(input_numofedges, min_numofedges);
}

static void on_value_chenged_input_numofcycle(GtkSpinButton *widget,
					      gpointer data)
{
	g_debug("on_value_chenged_input_numofcycle:numofcycle:%d",numofcycle);
	numofcycle = gtk_spin_button_get_value_as_int(widget);
}

static void on_value_changed_input_numofants(GtkSpinButton *widget,
					     gpointer data)
{
	g_debug("on_value_changed_input_numofants:old numofants:%d",numofants);
	numofants = gtk_spin_button_get_value_as_int(widget);
}

static void on_value_chenged_evaporation_rate(GtkSpinButton *widget,
					      gpointer data)
{
	g_debug("on_value_changed_input_evaporation_rate:old evaporation %f",evaporation_rate);	
	evaporation_rate = gtk_spin_button_get_value(widget);
}

static gboolean on_query_tooltip(GtkWidget *widget,
				 gint x,
				 gint y,
				 gboolean keyboard_mode,
				 GtkTooltip *tooltip,
				 gpointer data)
{
	if(graph == NULL)
		return FALSE;
	GdkPoint *item_loc = malloc(sizeof(GdkPoint));
	item_loc->x = (int)x;
	item_loc->y = (int)y;
	for(int i = 0 ; i < ui_points->len && graph->numofedges < MAX_EDGE_TOOLTIP;i++){
		GdkPoint *temp_item = g_array_index(ui_points, GdkPoint* , i);
		if (abs(item_loc->x - temp_item->x) < DISTANCE_CLICK
		    && abs(item_loc->y - temp_item->y) < DISTANCE_CLICK) {
			if(tooltip_text != NULL){
				free(tooltip_text);
				tooltip_text = NULL;
			}
			tooltip_text = malloc(sizeof(char)*graph->numofvertices * LENGTH_TOOLTIP);
			sprintf(tooltip_text,"V:%d (%d,%d)",i,temp_item->x,temp_item->y);
			for(int j = 0; j < graph->numofvertices;j++){
				if(j == i){
					continue;
				}
				else if(is_connected(graph,i,j) != NULL){
					GdkPoint *t = g_array_index(ui_points,GdkPoint*, j);
					struct _edge *edge = is_connected(graph,i,j);
					sprintf(tooltip_text + strlen(tooltip_text),"\nV:%d:(%d,%d):L:%d:P:%f",j,t->x,t->y,edge->len,edge->pheromone_value);
				}
			}
			gtk_tooltip_set_text(tooltip,tooltip_text);
			free(item_loc);
			return TRUE;
		}
	}
	free(item_loc);
	if(tooltip_text != NULL){
		free(tooltip_text);
		tooltip_text = NULL;
	}
	if(graph != NULL){
		tooltip_text = malloc(sizeof(char)*graph->numofvertices* LENGTH_TOOLTIP);
		sprintf(tooltip_text,"Graph Statistic\nNumber of Vertices : %d\nNumber of Edges : %d\nSum of all Edges length : %d",graph->numofvertices,graph->numofedges,graph->lenofalledges);
		if(lenofanswer != ZERO){
			sprintf(tooltip_text + strlen(tooltip_text), "\nlength of Cycle is %d",lenofanswer);
		}
		gtk_tooltip_set_text(tooltip,tooltip_text);
		return TRUE;
	}
	return FALSE;
}

/*
 *
 * important point of this implementation is ui_points index directly map to graph vertices index
 * ui_points[3] is vertex number 3 of graph implementation
 *
 */

#endif
