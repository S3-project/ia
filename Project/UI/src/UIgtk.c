# include <err.h>
# include <stdlib.h>
# include <stdio.h>
# include <gtk/gtk.h>


# include "../../main.h"


typedef struct
{
	GtkBuilder *builder;
	gpointer user_data;
} SGlobalData;

#define UNUSED(x) (void)(x)
gchar *Image_path=NULL;
gchar *NN_path=NULL;
gchar *Input_Rotation;
char *Text;
double Angle;
int indice_sscanf;

int UI()
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
    if (gtk_builder_add_from_file(builder, "UI/glade/main.glade", &error) == 0)
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
    gtk_window_set_title(GTK_WINDOW(window), "OCR");              
    gtk_main();

    return 0;
}

// called when Launch Neural Network button is clicked
void signal_trainNN(GtkTextBuffer *buffer)
{
	g_print("trainNN()\n");
	double d=0;
	gtk_text_buffer_set_text(buffer,"IA trained",10);
	TrainIA("../Ressources/Lettres/emnist-letters-train-images-idx3-ubyte",
	        "../Ressources/Lettres/emnist-letters-train-labels-idx1-ubyte",
	        NULL,
	        1,
	        &d);
}

//called when LaunchOCR button is clicked
void launchOCR(GtkButton *button,GtkTextBuffer *buffer)
{
	g_print("launchOCR()\n");
	UNUSED(button);
	//Third parameter is the rotation
	Text="Error no files given";
	if(Image_path && NN_path)
		Text=LaunchOCR((char*)Image_path,(char*)NN_path,Angle);
	int len=0;
	while(Text[len]!='\0')
		len++;
	gtk_text_buffer_set_text(buffer,(gchar*)Text,len);
}

//called when Save Text button is clicked
void save_text(GtkButton *button, GtkTextBuffer *buffer)
{	
	g_print("save_text()\n");
	GtkWidget *dialog;
  	GtkWidget *toplevel = gtk_widget_get_toplevel (GTK_WIDGET(button));
  	dialog = gtk_file_chooser_dialog_new ("Save Text ",
                    GTK_WINDOW (toplevel),
                    GTK_FILE_CHOOSER_ACTION_SAVE,
                    "Cancel", GTK_RESPONSE_CANCEL,
                    "Save", GTK_RESPONSE_ACCEPT,
                    NULL);
        if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
  	{
    		char *filename;
    		filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
    		// set the contents of the file to the text from the buffer
    		g_file_set_contents (filename, Text, strlen(Text), NULL);
  	}
  	gtk_widget_destroy (dialog);
  	gtk_text_buffer_set_text(buffer,"File saved",10);
}

//new file is setted
void file_set(GtkFileChooser *file,GtkImage *image)
{
	Image_path=gtk_file_chooser_get_filename(file);
	gtk_image_set_from_file (GTK_IMAGE (image), Image_path);
}

void activate_rotation(GtkEntry *entry)
{
	Input_Rotation=(gchar*)gtk_entry_get_text(entry);
	g_print("    %s \n",Input_Rotation);
	Angle=0;
	indice_sscanf =sscanf((char*)Input_Rotation, "%lf", &Angle);
}

//new neural network file is setted
void file_setNN(GtkFileChooser *file)
{
	NN_path=gtk_file_chooser_get_filename(file);	
}

// called when window is closed
void gtk_quit()
{
    gtk_main_quit();
}
