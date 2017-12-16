#include <gtk/gtk.h>
#include <glib.h>
#include <glib/gprintf.h>
#include <stdio.h>
#include <string.h> 


GtkWidget *window; //global window
GtkWidget *grid; // global fixed container
//number of messages




struct log
{
  GtkWidget *entry1;
  GtkWidget *entry2;
};
struct user
{
  GtkWidget *entry1;
  GtkWidget *entry2;
  GtkWidget *entry3;
};

void deleteChildren(){
  GList *children, *iter;
  
  children = gtk_container_get_children(GTK_CONTAINER(grid));

  for(iter = children; iter != NULL; iter = g_list_next(iter))
     {
      gtk_widget_destroy(GTK_WIDGET(iter->data));
     
     } 
  
  g_list_free(children);
} 
void show_info(gchar *inf) {

  GtkWidget *dialog;
  dialog = gtk_message_dialog_new(GTK_WINDOW(window),
            GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_INFO,
            GTK_BUTTONS_OK,"%s",
            inf);
  gtk_window_set_title(GTK_WINDOW(dialog), "Dialog");
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
}
void listUsers(GtkWidget *widget);

void mainWind();
void login(GtkWidget *widget);
void dashboard();
void messages();
void editUser(GtkWidget *widget, gpointer i);
void updateUser(GtkWidget *widget, gpointer user);

void submitLogin(GtkWidget *widget, gpointer en);


int main(int argc, char *argv[]) {
  gtk_init(&argc, &argv);
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "Restaurant Management System");
  gtk_window_set_default_size(GTK_WINDOW(window), 600, 400);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_container_set_border_width(GTK_CONTAINER(window), 15);

  grid = gtk_grid_new ();  
  g_object_ref_sink(grid);

  gtk_container_add(GTK_CONTAINER(window), grid);
  gtk_grid_set_column_spacing(GTK_GRID(grid), 5);
  gtk_grid_set_row_spacing(GTK_GRID(grid), 5);

  mainWind();
  gtk_main();
 

  return 0;
}
void mainWind(){
  deleteChildren();
  GtkWidget *button1;
  button1 = gtk_button_new_with_label("Login");

  gtk_grid_attach(GTK_GRID(grid), button1, 0, 0, 1, 1);
  g_signal_connect(button1, "clicked", G_CALLBACK(login), NULL);  
  g_signal_connect(G_OBJECT(window), "destroy",G_CALLBACK(gtk_main_quit), NULL); 

  gtk_widget_show_all(window); 
}
void login(GtkWidget *widget) {
  deleteChildren();

  GtkWidget *label1;
  GtkWidget *label2;
  GtkWidget *entry1;
  GtkWidget *entry2;
  GtkWidget *loginBut;
  GtkWidget *backBut;

  label1 = gtk_label_new("Email");
  label2 = gtk_label_new("Password");

  struct log en; 

  entry1 = gtk_entry_new();
  entry2 = gtk_entry_new();
  gtk_entry_set_visibility (GTK_ENTRY(entry2), FALSE);

  backBut = gtk_button_new_with_label("Back");
  loginBut = gtk_button_new_with_label("Login");

  gtk_grid_set_column_spacing(GTK_GRID(grid), 5);
  gtk_grid_set_row_spacing(GTK_GRID(grid), 5);

  gtk_grid_attach(GTK_GRID(grid), label1, 0,  0, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), label2, 0, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), entry1, 1, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), entry2, 1, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), loginBut, 1, 2, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), backBut, 5, 0, 1, 1);

  en.entry1 = entry1;
  en.entry2 = entry2;

  g_signal_connect(backBut, "clicked", G_CALLBACK(mainWind), NULL);  
  g_signal_connect(loginBut, "clicked", G_CALLBACK(submitLogin), &en);
  g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL); 

  gtk_widget_show_all(window);
  gtk_main();  

}
void submitLogin(GtkWidget *widget, gpointer en) {

  struct log *d = en;
  printf("email: %s\n", gtk_entry_get_text (GTK_ENTRY(d->entry1)));
  printf("Password: %s\n", gtk_entry_get_text (GTK_ENTRY(d->entry2)));
  

  char*a = "shoxrux";
  if(strcmp(gtk_entry_get_text(GTK_ENTRY(d->entry1)),a) == 0){
    gtk_entry_set_text(GTK_ENTRY(d->entry1), "");
    gtk_entry_set_text(GTK_ENTRY(d->entry2), "");
    dashboard();
  }else{
    gchar *message;
    message= "Entered wrong credentials";
    show_info(message);
  }  
}

void dashboard(){
  deleteChildren();
  GtkWidget *label1;
  GtkWidget *label2;
  GtkWidget *label3;

  GtkWidget *button1;
  GtkWidget *button2;
  GtkWidget *button3;


  GtkWidget *logoutBut;
  

  button1 = gtk_button_new_with_label("List users");
  button2 = gtk_button_new_with_label("Modify users");
  button3 = gtk_button_new_with_label("Remove users");
  
  logoutBut = gtk_button_new_with_label("Log Out");

  gtk_grid_attach(GTK_GRID(grid), button1, 1,  1, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), button2, 2, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), button3, 3, 1, 1, 1);

  gtk_grid_attach(GTK_GRID(grid), logoutBut, 4, 1, 1, 1);


  g_signal_connect(G_OBJECT(button1), "clicked", G_CALLBACK(listUsers), NULL);
  g_signal_connect(G_OBJECT(logoutBut), "clicked", G_CALLBACK(login), NULL);
  g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
  gtk_widget_show_all(window);
}
void listUsers(GtkWidget *widget){
  deleteChildren();

  int size = 5;
 
  gtk_grid_set_column_spacing(GTK_GRID(grid), 5);
  gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
  
  GtkWidget *number = gtk_label_new(NULL);  
  gtk_label_set_justify(GTK_LABEL(number), GTK_JUSTIFY_LEFT);
  gtk_label_set_markup(GTK_LABEL(number), "<b>#</b>");
  gtk_grid_attach(GTK_GRID(grid), number, 0, 1, 1, 1);

  GtkWidget *name = gtk_label_new(NULL);  
  gtk_label_set_justify(GTK_LABEL(name), GTK_JUSTIFY_LEFT);
  gtk_label_set_markup(GTK_LABEL(name), "<b>name</b>");
  gtk_grid_attach(GTK_GRID(grid), name, 1, 1, 1, 1);

  GtkWidget *phone = gtk_label_new(NULL);
  gtk_label_set_justify(GTK_LABEL(phone), GTK_JUSTIFY_LEFT);
  gtk_label_set_markup(GTK_LABEL(phone), "<b>phone</b>");
  gtk_grid_attach(GTK_GRID(grid), phone, 2, 1, 1, 1);

  GtkWidget *email = gtk_label_new(NULL);
  gtk_label_set_justify(GTK_LABEL(email), GTK_JUSTIFY_LEFT);
  gtk_label_set_markup(GTK_LABEL(email), "<b>email</b>");
  gtk_grid_attach(GTK_GRID(grid), email, 3, 1, 1, 1);

  GtkWidget *backBut;
  backBut = gtk_button_new_with_label("Back");
  gtk_grid_attach(GTK_GRID(grid), backBut, 4, 1, 1, 1);

  for(int j = 2; j <= size; j++)
  {
    gchar* index;
    index = g_strdup_printf("%d", j + 1);

    GtkWidget *number = gtk_label_new(index);
    gtk_grid_attach(GTK_GRID(grid), number, 0, j, 1, 1);

    GtkWidget *name = gtk_label_new("Alexandrer Stokes");
    gtk_grid_attach(GTK_GRID(grid), name, 1, j, 1, 1);

    GtkWidget *phone = gtk_label_new("+90 808-22-43");
    gtk_grid_attach(GTK_GRID(grid), phone, 2, j, 1, 1);

    GtkWidget *email = gtk_label_new("shohrux.shomaxmudov@mail.ru");
    gtk_grid_attach(GTK_GRID(grid), email, 3, j, 1, 1);

    GtkWidget *edit = gtk_button_new_with_label("edit");
    gtk_grid_attach(GTK_GRID(grid), edit, 4, j, 1, 1);

    GtkWidget *delete = gtk_button_new_with_label("delete");
    gtk_grid_attach(GTK_GRID(grid), delete, 5, j, 1, 1);

    g_signal_connect(G_OBJECT(edit),"clicked", G_CALLBACK(editUser), ((gpointer) (glong) (j+1)));
  }
 
  g_signal_connect(G_OBJECT(backBut), "clicked", G_CALLBACK(dashboard), NULL);
  g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
  
  gtk_widget_show_all(window); 
}
void editUser(GtkWidget *widget, gpointer i){
  deleteChildren();
  int index = ((gint) (glong) (i));
  GtkWidget *label1;
  GtkWidget *label2;
  GtkWidget *label3;

  label1 = gtk_label_new("Name:");
  label2 = gtk_label_new("Phone:");
  label3 = gtk_label_new("Email:");

  GtkWidget *entry1;
  GtkWidget *entry2;
  GtkWidget *entry3;

  entry1 = gtk_entry_new();
  entry2 = gtk_entry_new();
  entry3 = gtk_entry_new();
  gtk_entry_set_width_chars (GTK_ENTRY(entry1), 60);
  gtk_entry_set_text(GTK_ENTRY(entry1), "Shokhrukh");
   gtk_entry_set_width_chars (GTK_ENTRY(entry2), 60);
  gtk_entry_set_text(GTK_ENTRY(entry2), "+97 808-12-34");
   gtk_entry_set_width_chars (GTK_ENTRY(entry3), 60);
  gtk_entry_set_text(GTK_ENTRY(entry3), "shohrux.shomaxmudov@gmail.com");

  GtkWidget *backBut;
  backBut = gtk_button_new_with_label("back");
  GtkWidget *update;
  update = gtk_button_new_with_label("update");

  gtk_grid_attach(GTK_GRID(grid), label1, 1, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), label2, 1, 2, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), label3, 1, 3, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), entry1, 2, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), entry2, 2, 2, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), entry3, 2, 3, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), backBut, 3, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), update, 2, 4, 1, 1);

  struct user us;
  us.entry1 = entry1;
  us.entry2 = entry2;
  us.entry3 = entry3;

  gtk_widget_show_all(window);

  g_signal_connect(update, "clicked", G_CALLBACK(updateUser), &us);  
  g_signal_connect(backBut, "clicked", G_CALLBACK(listUsers), NULL);  
  g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
  gtk_main();
}
void updateUser(GtkWidget *widget, gpointer user){
  struct user *us = user;

  printf("Name:%s\n", gtk_entry_get_text (GTK_ENTRY(us->entry1)));
  printf("Phone:%s\n", gtk_entry_get_text (GTK_ENTRY(us->entry2)));
  printf("Email:%s\n", gtk_entry_get_text (GTK_ENTRY(us->entry3)));
  show_info("Your food order submitted");
  gtk_entry_set_text(GTK_ENTRY(us->entry1), "");
  gtk_entry_set_text(GTK_ENTRY(us->entry2), "");
  gtk_entry_set_text(GTK_ENTRY(us->entry3), "");
  listUsers(widget);
}