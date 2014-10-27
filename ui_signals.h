

static void on_toggled(GtkToggleButton *button,gpointer data)
{
	g_warning("on_toggled\n");
	gboolean g = gtk_toggle_button_get_active(button);
	Mode *mode = data;
	if(g)
		*mode = Vertex;
	else
		*mode = Random;
}


static gboolean on_draw(GtkWidget * widget, cairo_t * cr, gpointer data)
{
	cairo_set_source_surface(cr, surface, 0, 0);
	cairo_paint(cr);
	return FALSE;
}

static void clear_surface(void)
{
	cairo_t *cr;
	cr = cairo_create(surface);
	cairo_set_source_rgb(cr, RGB_SURFACE , RGB_SURFACE , RGB_SURFACE);
	cairo_paint(cr);
	cairo_destroy(cr);
}

static gboolean
on_configure_event(GtkWidget * widget, GdkEventConfigure * event, gpointer data)
{
	g_warning("on_configure_event\n");
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

static void draw_rectangle(GtkWidget * widget, gdouble x, gdouble y)
{
	cairo_t *cr;
	g_warning("drawing here : (%f,%f)\n", x, y);
	if (!surface) {
		g_warning("fatal error in line:%d", __LINE__);
		exit(1);
	}
	cr = cairo_create(surface);
	cairo_rectangle(cr, x, y, RECT_, RECT_);
	cairo_fill(cr);
	cairo_destroy(cr);
	gtk_widget_queue_draw_area(widget, x, y, RECT_, RECT_);
}

static gboolean
on_delete_event(GtkWidget * widget, GdkEvent * event, gpointer data)
{
	g_warning("on_delete_event\n");
	/* ask from user , if he wants really quit then return FALSE , otherwise return TRUE , OK? */

	return FALSE;
}
static void on_clicked_button_solve_problem(GtkWidget * widget, gpointer data)
{
	g_warning("clicked on problem solve button, not implemented yet\n");
	// TODO
	// not implement yet
}
static void on_clicked_drawing_area(GtkWidget * widget, gpointer data)
{
	g_warning("from graph_window_clicked\n");

}

static gboolean
on_button_press_event(GtkWidget * widget, GdkEventButton * event, gpointer data)
{
	g_warning("from on_button_press_event");
	g_warning(" (%f,%f)\n", event->x, event->y);
	Mode *mode = data;
	if ((*mode) == Vertex) {
		g_warning("should draw an rectangle\n");
		draw_rectangle(widget, event->x, event->y);
	}

	return TRUE;
}

static gboolean
on_motion_notify_event(GtkWidget * widget,
		       GdkEventMotion * event, gpointer data)
{
//      g_warning("from on_motion_notify_event (%f,%f)\n",event->x,event->y);

	return TRUE;
}

