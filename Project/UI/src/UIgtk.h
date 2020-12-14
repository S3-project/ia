#ifndef UIGTK_H
#define UIGTK_H


#include <err.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <gtk/gtk.h>
#include "../../pre-processing/Rotate/rotate.h"
#include "../../other/Bitmap/bitmap.h"
#include "../ia_graph_train.h"




#define TRAIN_REFRESH_PRINT 1000
#define UNUSED(x) (void)(x)


int UI();

gboolean on_draw_image(GtkWidget *widget, cairo_t *cr, gpointer user_data);
void on_stop_training_clicked();

typedef struct {
	BMPIMAGE *img;
	BMPIMAGE *img_original;
} AREA_IMG;
AREA_IMG area_img = {NULL,NULL};

GtkDrawingArea* area = NULL;

gchar *Image_path=NULL;
gchar *NN_path=NULL;
gchar *Input_Rotation;
char *Text;
double Angle;
int indice_sscanf;
int IA_window_open=0;
int denoise=0;

gint timeout_tag=0;

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
    area = GTK_DRAWING_AREA(gtk_builder_get_object(builder, "area"));
    
    
    gtk_builder_connect_signals(builder, NULL);
    g_signal_connect(area, "draw", G_CALLBACK(on_draw_image), &area_img);
    

    g_object_unref(builder);

    gtk_widget_show(window);  
    gtk_window_set_title(GTK_WINDOW(window), "OCR");              
    gtk_main();

    return 0;
}


void free_bitmap_preview()
{
	if(area_img.img)
	{
		FreeBitmap(area_img.img);
	}
}

void free_bitmap_preview_all()
{
	if(area_img.img)
	{
		FreeBitmap(area_img.img);
	}
	if(area_img.img_original)
	{
		FreeBitmap(area_img.img_original);
	}
}

void load_bitmap_preview(char* path)
{
	area_img.img_original=LoadBitmap(path);
	area_img.img=LoadBitmap(path);
}

// called when Launch Neural Network button is clicked
int on_trainNN_clicked(GtkWidget *IA_window)
{
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
	
		IA_window_open=1;
	}
	return 0;
}

//called when LaunchOCR button is clicked
void on_launchOCR_button_clicked(GtkButton *button,GtkTextBuffer *buffer)
{
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
void on_savetext_clicked(GtkButton *button, GtkTextBuffer *buffer)
{
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
void on_fileimage_selected(GtkFileChooser *file)
{
	Image_path=gtk_file_chooser_get_filename(file);
	free_bitmap_preview();
	load_bitmap_preview((char*)Image_path);
	gtk_widget_queue_draw(GTK_WIDGET(area));
}

void on_rotation_activated(GtkEntry *entry)
{
	Input_Rotation=(gchar*)gtk_entry_get_text(entry);
	Angle=0;
	indice_sscanf =sscanf((char*)Input_Rotation, "%lf", &Angle);
	
	BMPIMAGE *img_rot=Rotate(Angle,area_img.img_original);
	free_bitmap_preview();
	area_img.img=img_rot;
	gtk_widget_queue_draw(GTK_WIDGET(area));
}


gboolean on_draw_image(GtkWidget *widget, cairo_t *cr, gpointer user_data)
{
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


	double ratio = width_area / (double)width;
	
	if (height * ratio > height_area){
		ratio = height_area / (double)height;
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


//new neural network file is setted
void on_fileNN_selected(GtkFileChooser *file)
{
	NN_path=gtk_file_chooser_get_filename(file);	
}

void IA_window_delete()
{
	on_stop_training_clicked();
	IA_window_open=0;
}

void on_denoise_checked()
{
	if(!denoise)
		denoise=1;
	else
		denoise=0;
	
	if(denoise)
	{
		BMPIMAGE *img_rot=Rotate(Angle,area_img.img_original);
		BMPIMAGE *img_den=Denoising(img_rot);
		FreeBitmap(img_rot);
		free_bitmap_preview();
		area_img.img=img_den;
		gtk_widget_queue_draw(GTK_WIDGET(area));
	}
	else
	{
		
		BMPIMAGE *img_rot=Rotate(Angle,area_img.img_original);
		free_bitmap_preview();
		area_img.img=img_rot;
		gtk_widget_queue_draw(GTK_WIDGET(area));
	}
}

// called when window is closed
void gtk_quit()
{
    free_bitmap_preview_all();
    gtk_main_quit();
}

///////////////////
///Second Window///
///////////////////

gchar *database_images=NULL;
gchar *labels_images=NULL;
gchar *IA_optional=NULL;
gchar *New_File=NULL;

gchar *Input_hiddenlayer1;
gchar *Input_hiddenlayer2;
gchar *Input_learningrate;
gchar *Input_iteration;

int hidden_layers1=0;
int hidden_layers2=0;
double learning_rate=0;
int iteration=0;

int stop=0;
double progression=0;
int running =0;

GRAPH_TRAIN_INFO train_info={NULL,NULL,NULL,0,0.1,15,15,&stop,&progression,&running};


/////The three file chooser button and their signals
void on_databaseim_selected(GtkFileChooser *file)
{
	database_images=gtk_file_chooser_get_filename(file);
}

void on_databaselb_selected(GtkFileChooser *file)
{
	labels_images=gtk_file_chooser_get_filename(file);
}

void on_NNtrain_selected(GtkFileChooser *file)
{
	IA_optional=gtk_file_chooser_get_filename(file);
}

void on_newfile_activated(GtkEntry *entry)
{
	New_File=(gchar*)gtk_entry_get_text(entry);
	if(!New_File[0])
		New_File=NULL;	
}


/////Start and stop button
gboolean display_progression(gpointer buff)
{
	GtkTextBuffer *buffer=buff;
	if(running)
	{
		if(progression==0)
		{
			gtk_text_buffer_set_text(buffer,"Loading database...",19);
		}
		else if(progression<10)
		{
			char progression_text[7];
			sprintf(progression_text, "%.2lf %c",progression,'%');
			progression_text[6]=0;
			gtk_text_buffer_set_text(buffer,progression_text,6);
		}
		else
		{	
			char progression_text[8];
			sprintf(progression_text, "%.2lf %c",progression,'%');
			progression_text[7]=0;
			gtk_text_buffer_set_text(buffer,progression_text,7);
		}
	}
	if(!running)
	{
		gtk_text_buffer_set_text(buffer,"The training is finished !",26);
		timeout_tag=0;
		progression=0;
		return FALSE;
	}
	return TRUE;
}

void on_start_training_clicked(GtkButton *button,GtkTextBuffer *buffer)
{
	UNUSED(button);
	if(New_File)
	{
		train_info.NN_PATH=(char*)New_File;
	}
	else
	{
		train_info.NN_PATH=(char*)IA_optional;
	}
	
	if(train_info.NN_PATH)
	{
		if(database_images && labels_images)
		{
			if(!running)
			{	
				timeout_tag = g_timeout_add(25,display_progression,buffer);
				stop=0;
				pthread_t thread;
				
				train_info.PROGRESSION=0;
				train_info.STOP=0;
				
				train_info.TDB_IM=(char*)database_images;
				train_info.TDB_LB=(char*)labels_images;
				train_info.ITERATION=iteration;
				train_info.LR=learning_rate;
				train_info.HL1=hidden_layers1;
				train_info.HL2=hidden_layers2;
				train_info.STOP=&stop;
				train_info.PROGRESSION=&progression;
				train_info.RUNNING=&running;
				pthread_create(&thread,NULL,Train_graph_NN,&train_info);
			}
			else
			{
				gtk_text_buffer_set_text(buffer,"Training is already running.",28);
			}
		}
		else
		{
			gtk_text_buffer_set_text(buffer,"Database not given.",19);
		}
	}
	else
	{
		gtk_text_buffer_set_text(buffer,"Error with the neural network name. Please choose a file or enter a name.",73);
	}
}

void on_stop_training_clicked()
{
	stop=1;
	progression=0;
	if(timeout_tag!=0)
	{
		g_source_remove(timeout_tag);
		timeout_tag=0;
	}
}

/////Parameters of hidden layers, learning rate, iteration
void on_nbhiddenlayer1_activated(GtkEntry *entry)
{
	Input_hiddenlayer1=(gchar*)gtk_entry_get_text(entry);
	sscanf((char*)Input_hiddenlayer1, "%d", &hidden_layers1);
}


void on_nbhiddenlayer2_activated(GtkEntry *entry)
{
	Input_hiddenlayer2=(gchar*)gtk_entry_get_text(entry);
	sscanf((char*)Input_hiddenlayer2, "%d", &hidden_layers2);
}


void on_lr_activated(GtkEntry *entry)
{
	Input_learningrate=(gchar*)gtk_entry_get_text(entry);
	sscanf((char*)Input_learningrate, "%lf", &learning_rate);
}


void on_iterations_activated(GtkEntry *entry)
{
	Input_iteration=(gchar*)gtk_entry_get_text(entry);
	sscanf((char*)Input_iteration, "%d", &iteration);
}

#endif
