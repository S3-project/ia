#include <"gtk/gtk.h">

GtkWidget *g_lbl_hello;
GtkWidget *g_lbl_count;

int main(int argc, char *argv[])
{

    // Initializes GTK.
    gtk_init(NULL, NULL);

    // Constructs a GtkBuilder instance.
    GtkBuilder* builder = gtk_builder_new ();

    // Loads the UI description.
    // (Exits if an error occurs.)
    GError* error = NULL;
    if (gtk_builder_add_from_file(builder, "duel.glade", &error) == 0)
    {
        g_printerr("Error loading file: %s\n", error->message);
        g_clear_error(&error);
        return 1;
    }
    
    
    
    GtkBuilder      *builder; 
    GtkWidget       *window;

    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "UI.glade", NULL);

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window_1"));
    gtk_builder_connect_signals(builder, NULL);
    
    // get pointers to the labels
    g_lbl_hello = GTK_WIDGET(gtk_builder_get_object(builder, "lbl_hello"));
    g_lbl_count = GTK_WIDGET(gtk_builder_get_object(builder, "lbl_count"));

    g_object_unref(builder);

    gtk_widget_show(window);                
    gtk_main();

    return 0;
}

// called when button is clicked
void on_btn_execute_clicked()
{
	printf("on_btn_clicked()");
}

// called when window is closed
void on_window_1_destroy()
{
    gtk_main_quit();
}
