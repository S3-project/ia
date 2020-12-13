# include <err.h>
# include <stdlib.h>
# include <stdio.h>
#include <stdint.h>
# include <gtk/gtk.h>


# include "../../main.h"
# include "../../pre-processing/Rotate/rotate.h"
# include "../../other/Bitmap/bitmap.h"


#define UNUSED(x) (void)(x)

gchar *Image_path=NULL;
gchar *NN_path=NULL;
gchar *Input_Rotation;
char *Text;
double Angle;
int indice_sscanf;
int IA_window_open=0;
int denoise=0;


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
int signal_trainNN(GtkWidget *IA_window)
{
	g_print("trainNN()\n");
	if(!IA_window_open)
	{
		GtkBuilder* builder2 = gtk_builder_new ();
		GError* error2 = NULL;
		if (gtk_builder_add_from_file(builder2, "UI/glade/main.glade", &error2) == 0)
		{
			g_printerr("Error loading file: %s\n", error2->message);
			g_clear_error(&error2);
			return 1;
		}
		IA_window= GTK_WIDGET(gtk_builder_get_object(builder2, "IA_window"));
	    	gtk_builder_connect_signals(builder2, NULL);
	    	g_object_unref(builder2);
		gtk_widget_show(IA_window);
		gtk_window_set_title(GTK_WINDOW(IA_window), "Train IA");
		//gtk_text_buffer_set_text(buffer,"IA training window opened",10);
		/*
		double d=0;
		gtk_text_buffer_set_text(buffer,"IA trained",10);
		TrainIA("../Ressources/Lettres/emnist-letters-train-images-idx3-ubyte",
			"../Ressources/Lettres/emnist-letters-train-labels-idx1-ubyte",
			NULL,
			1,
			&d);*/
		IA_window_open=1;
	}
	return 0;
}

//called when LaunchOCR button is clicked
void launchOCR(GtkButton *button,GtkTextBuffer *buffer)
{
	g_print("launchOCR()\n");
	UNUSED(button);
	//Third parameter is the rotation
	Text="Error no files given";
	if(Image_path && NN_path)
		Text=LaunchOCR((char*)Image_path,(char*)NN_path,Angle,denoise);
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

void activate_rotation(GtkEntry *entry,GtkImage *image)
{
	Input_Rotation=(gchar*)gtk_entry_get_text(entry);
	g_print("    %s \n",(char*)Input_Rotation);
	Angle=0;
	indice_sscanf =sscanf((char*)Input_Rotation, "%lf", &Angle);
	UNUSED(image);
	/*if(indice_sscanf)
	{
		BMPIMAGE *bmp_image = LoadBitmap((char*)Image_path);
		
		BMPIMAGE *rotateimage = Rotate(Angle,bmp_image);
		
		SaveBitmap(rotateimage, (char*)Image_path);
		FreeBitmap(rotateimage);
		FreeBitmap(bmp_image);
		gtk_image_set_from_file (GTK_IMAGE (image), Image_path);
	}*/
}

//new neural network file is setted
void file_setNN(GtkFileChooser *file)
{
	NN_path=gtk_file_chooser_get_filename(file);	
}


void IA_window_delete()
{
	IA_window_open=0;
}

void denoise_check()
{
	if(!denoise)
		denoise=1;
	else
		denoise=0;
	g_print("%d \n",denoise);
}

// called when window is closed
void gtk_quit()
{
    gtk_main_quit();
}



///////////////////
///Second Window///
///////////////////

gchar *database_images=NULL;
gchar *labels_images=NULL;
gchar *IA_optional=NULL;

gchar *Input_hiddenlayer;
gchar *Input_learningrate;
gchar *Input_iteration;

int hidden_layers=0;
double learning_rate=0;
int iteration=0;

int stop=0;
double progression=0;
int running =0;

/////The three file chooser button and their signals
void database_im(GtkFileChooser *file)
{
	database_images=gtk_file_chooser_get_filename(file);
	g_print("    %s \n",(char*)database_images);
}

void labels_im(GtkFileChooser *file)
{
	labels_images=gtk_file_chooser_get_filename(file);
	g_print("    %s \n",(char*)labels_images);
}

void existingNN(GtkFileChooser *file)
{
	IA_optional=gtk_file_chooser_get_filename(file);
	g_print("    %s \n",(char*)IA_optional);
}


/////Start and stop button
void start_training()
{
	//if(!running)
	/*int a=10;
	progression=0;
	stop=0;
	void** arg[9];
	arg[0]=(char*)database_images;
	arg[1]=(char*)labels_images;
	arg[2]=(char*)IA_optional;
	arg[3]=&iteration;
	arg[4]=&learning_rate;
	arg[5]=&hidden_layers;
	arg[6]=&a;
	arg[7]=&stop;
	arg[8]=&progression;
	arg[9]=&running;*/
}

void stop_training()
{
	stop=1;
}


/////Parameters of hidden layers, learning rate, iteration
void activate_hiddenlayer(GtkEntry *entry)
{
	Input_hiddenlayer=(gchar*)gtk_entry_get_text(entry);
	g_print("    %s \n",(char*)Input_hiddenlayer);
	Angle=0;
	sscanf((char*)Input_hiddenlayer, "%d", &hidden_layers);
}


void activate_learningrate(GtkEntry *entry)
{
	Input_learningrate=(gchar*)gtk_entry_get_text(entry);
	g_print("    %s \n",(char*)Input_learningrate);
	sscanf((char*)Input_learningrate, "%lf", &learning_rate);
}


void activate_iteration(GtkEntry *entry)
{
	Input_iteration=(gchar*)gtk_entry_get_text(entry);
	g_print("    %s \n",(char*)Input_iteration);
	sscanf((char*)Input_iteration, "%d", &iteration);
}
