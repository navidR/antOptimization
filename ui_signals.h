
static void draw_line(GtkWidget * widget){
	cairo_t *cr;
	g_debug("draw_line:(%d,%d)->(%d,%d)", bag.first_item->x,bag.first_item->y,bag.second_item->x,bag.second_item->y);
	if (!surface) 
		g_error("draw_line:surface is null , fatal error in line:%d", __LINE__);
	cr = cairo_create(surface);
	g_debug("draw_line:line width : %f",cairo_get_line_width(cr));
	cairo_move_to(cr,bag.first_item->x,bag.first_item->y);
	cairo_line_to(cr,bag.second_item->x,bag.second_item->y);
	cairo_stroke(cr);
	cairo_arc(cr,bag.first_item->x,bag.first_item->y,RADIUS,FIRST_ANGLE, SECOND_ANGLE);
	cairo_fill(cr);
	cairo_destroy(cr);
	gtk_widget_queue_draw(widget);
}

static void on_toggled_button_drawing_mode(GtkToggleButton *button,gpointer data)
{
	g_debug("on_toggled");
	GPtrArray *widget_array = data;
	gboolean g = gtk_toggle_button_get_active(button);
	GtkWidget *input_numofvertices,*input_numofedges;
	_mode *mode = g_ptr_array_index(widget_array,MODE_INDEX);
	input_numofvertices = g_ptr_array_index(widget_array,SPIN_VERTEX_INDEX);
	input_numofedges = g_ptr_array_index(widget_array,SPIN_EDGE_INDEX);
	if(g){
		*mode = DRAWING_MODE;
		gtk_widget_set_sensitive(input_numofvertices,FALSE);
		gtk_widget_set_sensitive(input_numofedges,FALSE);
	}
	else{
		*mode = RANDOM_MODE;
		gtk_widget_set_sensitive(input_numofvertices,TRUE);
		gtk_widget_set_sensitive(input_numofedges,TRUE);
	}
	
}


static gboolean on_draw(GtkWidget * widget, cairo_t * cr, gpointer data)
{
	g_debug("on_draw");
	cairo_set_source_surface(cr, surface, 0, 0);
	cairo_paint(cr);
	return FALSE;
}

static void clear_surface(void)
{
	g_debug("on_clear_surface");
	cairo_t *cr;
	cr = cairo_create(surface);
	cairo_set_source_rgb(cr, RGB_SURFACE , RGB_SURFACE , RGB_SURFACE);
	cairo_paint(cr);
	cairo_destroy(cr);
}

static gboolean on_configure_event(GtkWidget * widget, GdkEventConfigure * event, gpointer data)
{
	g_debug("on_configure_event");
	if (surface)
		cairo_surface_destroy(surface);
	surface = gdk_window_create_similar_surface(gtk_widget_get_window(widget),
						    CAIRO_CONTENT_COLOR,gtk_widget_get_allocated_width
						    (widget),
						    gtk_widget_get_allocated_height
						    (widget));
	clear_surface();
	return TRUE;
}

static void draw_rectangle(GtkWidget * widget, struct _points* item_loc , gboolean status)
{
	cairo_t *cr;
	g_debug("drawing here : (%d,%d)", item_loc->x, item_loc->y);
	if (!surface) 
		g_error("surface is null , fatal error in line:%d", __LINE__);
	cr = cairo_create(surface);
	if(status)
		cairo_set_source_rgb(cr,RED,FALSE,FALSE);
	cairo_arc(cr,item_loc->x,item_loc->y,RADIUS,FIRST_ANGLE, SECOND_ANGLE);
	cairo_fill(cr);
	cairo_destroy(cr);
	gtk_widget_queue_draw(widget);
}

static gboolean on_delete_event(GtkWidget * widget, GdkEvent * event, gpointer data)
{
	g_debug("on_delete_event");
	/* ask from user , if he wants really quit then return FALSE , otherwise return TRUE , OK? */

	return FALSE;
}

static void on_clicked_button_generating_random_graph(GtkWidget *widget,gpointer data){
	g_debug("on_clicked_button_generating_random_graph:not implemented yet");
	// TODO
	// not implemented yet

}

static void on_clicked_button_solve_problem(GtkWidget * widget, gpointer data)
{
	g_debug("clicked on problem solve button, not implemented yet");
	// TODO
	// not implement yet
}

// we send Mode as pointer in gpointer (3rd)
static gboolean on_button_press_event(GtkWidget * widget, GdkEventButton * event, gpointer pmode)
{
	g_debug("on_button_press_event:(%d,%d)", (int) event->x, (int) event->y);
	_mode *mode = pmode;
	if (*mode == DRAWING_MODE) {
		g_debug("on_button_press_event:new item_loc adding to ui_points->len : %d",ui_points->len);
		struct _points *item_loc = malloc(sizeof(struct _points));
		item_loc->x = (int) event->x;
		item_loc->y = (int) event->y;
		for(int i = 0 ; i < ui_points->len ; i++){
			struct _points *temp_item = g_array_index(ui_points,struct _points*,i);
			if(abs(item_loc->x - temp_item->x) < DISTANCE && abs(item_loc->y - temp_item->y) < DISTANCE)
			{
				// there is no need for this debuging information
				// g_debug("x-x is %d y-y is %d", abs(item_loc->x_ - temp_item->x_),abs(item_loc->y_ - temp_item->y_));
				g_debug("on_button_press_event:on_button_press_event:already (%d,%d) there is point so close to this one (%d,%d)",temp_item->x,temp_item->y,item_loc->x,item_loc->y);
				free(item_loc);
				// we dont have any vertex selected
				if(bag.first_item == NULL && bag.second_item == NULL){
					draw_rectangle(widget,temp_item,TRUE);
					bag.first_item = temp_item;
				}
				// draw line , we a have a vertex selected already
				else{
					bag.second_item = temp_item;
					draw_line(widget);
					bag.first_item = bag.second_item;
					draw_rectangle(widget,temp_item,TRUE);
					bag.second_item = NULL;
				}
				return TRUE;
			}
		}
		// we check if we have any selected item then free item and just return
		if(bag.first_item != NULL){
			g_debug("on_button_press_event:bag.first_item is selected but second item , not select currectly");
			draw_rectangle(widget,bag.first_item,FALSE);
			bag.first_item = NULL;
			bag.second_item = NULL;
			free(item_loc);
			return TRUE;
		}
		draw_rectangle(widget, item_loc , FALSE);
		g_array_append_val(ui_points,item_loc);
		g_debug("on_button_press_event:number of elements in ui_points : %d",ui_points->len);
	}

	return TRUE;
}

static gboolean
on_motion_notify_event(GtkWidget * widget,
		       GdkEventMotion * event, gpointer data)
{
//      g_debug("from on_motion_notify_event (%f,%f)\n",event->x,event->y);

	return TRUE;
}

static void on_value_changed_input_numofvertices(GtkSpinButton *widget,gpointer data)
{
	g_debug("on_value_changed_input_numofvertices");
	GtkSpinButton *input_numofedges = data;
	int min_numofedges = gtk_spin_button_get_value_as_int(widget);
	int max_numofedges = ((min_numofedges * (min_numofedges - 1 )) / 2);
	g_debug("on_value_changed_input_numofvertices:max_numofedges:%d",max_numofedges);
	GtkAdjustment *adjustment_input_numofedges = gtk_adjustment_new(min_numofedges,min_numofedges,max_numofedges,INCREMENT_RATE,ZERO,ZERO);
	gtk_spin_button_set_adjustment(input_numofedges,adjustment_input_numofedges);
	gtk_spin_button_set_value(input_numofedges,min_numofedges);
}
