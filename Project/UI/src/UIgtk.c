# include <err.h>
# include <stdlib.h>
# include <stdio.h>
# include <gtk/gtk.h>
# include <SDL/SDL.h>
# include <SDL/SDL_image.h>




typedef struct
{
	GtkBuilder *builder;
	gpointer user_data;
} SGlobalData;

#define UNUSED(x) (void)(x)

gchar *filename = "";
char *text = "";

int main(int argc, char *argv[])
{
    GtkWidget *window;
    //GtkWidget *filename;

    // Initializes GTK.
    gtk_init(NULL, NULL);

    // Constructs a GtkBuilder instance.
    GtkBuilder* builder = gtk_builder_new ();

    // Loads the UI description.
    // (Exits if an error occurs.)
    GError* error = NULL;
    if (gtk_builder_add_from_file(builder, "glade/main.glade", &error) == 0)
    {
        g_printerr("Error loading file: %s\n", error->message);
        g_clear_error(&error);
        return 1;
    }
    
    

    window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));
    gtk_builder_connect_signals(builder, NULL);
    
    // get pointers to the labels
    //filename = GTK_WIDGET(gtk_builder_get_object(builder, "filename"));

    g_object_unref(builder);

    gtk_widget_show(window);  
    gtk_window_set_title(GTK_WINDOW(window), "OCR window");              
    gtk_main();

    return 0;
}

// called when Launch Neural Network button is clicked
void trainNN()
{
	g_print("trainNN()\n");
}

//called when Save Neural Network button is clicked
void saveNN()
{
	g_print("saveNN()\n");
}

//called when Load Image button is clicked
void load_image(GtkButton *button, GtkImage *image)
{
	g_print("load_image()\n");
}

//called when LaunchOCR button is clicked
void launchOCR(GtkButton *button, GtkTextBuffer *buffer)
{
	g_print("launchOCR()\n");
}

//called when Save Text button is clicked
void save_text(GtkButton *button, GtkTextBuffer *buffer)
{	
	g_print("save_text()\n");
}

//new file is setted
void file_set()
{
	g_print("file_set()\n");
}

//new neural network file is setted
void file_setNN()
{
	g_print("file_setNN()\n");
}

// called when window is closed
void gtk_quit()
{
    gtk_main_quit();
}
