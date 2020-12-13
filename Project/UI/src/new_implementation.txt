#include <gtk/gtk.h>
#include "Bitmap/bitmap.h"



GtkDrawingArea* area = NULL;
typedef struct {
	BMPIMAGE *img;
} AREA_IMG;
AREA_IMG area_img = {NULL};

// Signal handler for the "clicked" signal of the start button.
void on_start(GtkButton *button, gpointer user_data)
{

	FreeBitmap(area_img.img);
	area_img.img = LoadBitmap("shut.bmp");
	gtk_widget_queue_draw(GTK_WIDGET(area));
	g_print("on_start()\n");
}


// Signal handler for the "draw" signal of the drawing area.
gboolean on_draw(GtkWidget *widget, cairo_t *cr, gpointer user_data)
{
	g_print("d");
	// Sets the background to white.
	cairo_set_source_rgb(cr, 1, 1, 1);
	cairo_paint(cr);

	// Gets the image.
	AREA_IMG *aimg = user_data;
	BMPIMAGE *img = aimg->img;
	if (img == NULL){
		return FALSE;
	}

	int width_area = gtk_widget_get_allocated_width(widget);
	int height_area = gtk_widget_get_allocated_height(widget);
	int width = img->header.width;
	int height = img->header.heigth;


	double ratio = width_area / width;
	if (height * ratio > height_area){
		ratio = height_area / height;
	}
	int size_x = (int)(width * ratio);
	int size_y = (int)(height * ratio);
	int x_start = (width_area - size_x) / 2;
	int y_start = (height_area - size_y) / 2;
	double step_y = height / (double)size_y;
	double value_y = 0;
	for (int y = 0; y < size_y; y++){
		double step_x = width / (double)size_x;
		double value_x = 0;
		for (int x = 0; x < size_x; x++){
			RGB rgb = img->data[(int)value_y][(int)value_x];
			cairo_set_source_rgb(cr, rgb.R / 255.0, rgb.G / 255.0, rgb.B / 255.0);
			cairo_rectangle(cr, x+x_start, y+y_start, 1, 1);
			cairo_fill(cr);
			value_x += step_x;
		}
		value_y += step_y;
	}

	// Propagates the signal.
	return FALSE;
	
}


// Main function.
int main (int argc, char *argv[])
{
	// Initializes GTK.
	gtk_init(NULL, NULL);

	area_img.img = LoadBitmap("img.bmp");
	//BMPIMAGE *img2 = LoadBitmap("shut.bmp");
	//BMPIMAGE *img = NULL;

	// Loads the UI description and builds the UI.
	// (Exits if an error occurs.)
	GtkBuilder* builder = gtk_builder_new();
	GError* error = NULL;
	if (gtk_builder_add_from_file(builder, "duel.glade", &error) == 0)
	{
		g_printerr("Error loading file: %s\n", error->message);
		g_clear_error(&error);
		return 1;
	}

	// Gets the widgets.
	GtkWindow* window = GTK_WINDOW(gtk_builder_get_object(builder, "org.gtk.duel"));
	GtkButton* start_button = GTK_BUTTON(gtk_builder_get_object(builder, "start_button"));
	area = GTK_DRAWING_AREA(gtk_builder_get_object(builder, "area"));

	GdkRectangle rect = {100, 100, 10, 10};


	// Connects signal handlers.
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect(start_button, "clicked", G_CALLBACK(on_start), NULL);
	g_signal_connect(area, "draw", G_CALLBACK(on_draw), &area_img);

	// Runs the main loop.
	gtk_main();

	// Exits.
	return 0;
}
