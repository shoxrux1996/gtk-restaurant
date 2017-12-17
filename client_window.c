#include <gtk/gtk.h>
#include <glib.h>
#include <glib/gprintf.h>
#include <stdio.h>
#include <string.h> 


GtkWidget *window; //global window
GtkWidget *grid; 
char *numb = 0; //number of messages
// global fixed container



struct log
{
  GtkWidget *entry1;
  GtkWidget *entry2;
};
struct regist
{
  GtkWidget *entry1;
  GtkWidget *entry2;
  GtkWidget *entry3;
  GtkWidget *entry4;
};
struct bookSeats 
{
  GtkWidget *entry1;
  GtkWidget *entry2;
  GtkWidget *entry3;
};
struct orderFoods
{
  GtkWidget *entry1;
  GtkWidget *entry2;
  int id;
};

void deleteChildren(){
  GList *children, *iter;
  
  children = gtk_container_get_children(GTK_CONTAINER(grid));

  for(iter = children; iter != NULL; iter = g_list_next(iter))
     {
       gtk_container_remove(GTK_CONTAINER(grid), GTK_WIDGET(iter->data));
     
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


void mainWind();
void registe(GtkWidget *widget);
void login(GtkWidget *widget);
void dashboard();
void messages();
void bookSeat();
void submitLogin(GtkWidget *widget, gpointer en);
void submitRegister(GtkWidget *widget, gpointer shox);
void submitBook(GtkWidget *widget, gpointer book);
void chooseCategory(GtkWidget *widget, gpointer i);
void chooseFood(GtkWidget *widget, gpointer i);
void orderFood(GtkWidget *widget);
void submitOrder(GtkWidget *widget, gpointer order);

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

  mainWind();
  gtk_main();
 

  return 0;
}
void mainWind(){
  deleteChildren();
  GtkWidget *button1;
  GtkWidget *button2;
  button1 = gtk_button_new_with_label("Login");
  button2 = gtk_button_new_with_mnemonic("Register");

  gtk_grid_attach(GTK_GRID(grid), button1, 0, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), button2, 0, 3, 1, 1);
  g_signal_connect(button1, "clicked", G_CALLBACK(login), NULL);  
  g_signal_connect(button2, "clicked", G_CALLBACK(registe), NULL); 
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

  gtk_entry_set_text(GTK_ENTRY(d->entry1), "");
  gtk_entry_set_text(GTK_ENTRY(d->entry2), "");
  numb = "1";

  dashboard();
}
void registe(GtkWidget *widget){
  deleteChildren();
  GtkWidget *label1;
  GtkWidget *label2;
  GtkWidget *label3;
  GtkWidget *label4;
  GtkWidget *entry1;
  GtkWidget *entry2;
  GtkWidget *entry3;
  GtkWidget *entry4;

  GtkWidget *backBut;
  GtkWidget *registerBut;

 
  label1 = gtk_label_new("Name");
  label2 = gtk_label_new("Phone");
  label3 = gtk_label_new("Email");
  label4 = gtk_label_new("Password");

  
  entry1 = gtk_entry_new();
  entry2 = gtk_entry_new();
  entry3 = gtk_entry_new();
  entry4 = gtk_entry_new();
  gtk_entry_set_visibility (GTK_ENTRY(entry4), FALSE);
  registerBut = gtk_button_new_with_label("Register");
  backBut = gtk_button_new_with_label("Back");

  gtk_grid_set_column_spacing(GTK_GRID(grid), 5);
  gtk_grid_set_row_spacing(GTK_GRID(grid), 5);

  gtk_grid_attach(GTK_GRID(grid), label1, 0, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), label2, 0, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), label3, 0, 2, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), label4, 0, 3, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), entry1, 1, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), entry2, 1, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), entry3, 1, 2, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), entry4, 1, 3, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), registerBut, 1, 4, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), backBut, 4, 5, 1, 1);

  struct regist en;
  en.entry1 = entry1;
  en.entry2 = entry2;
  en.entry3 = entry3;
  en.entry4 = entry4;

  g_signal_connect(backBut, "clicked", G_CALLBACK(mainWind), NULL);  
  g_signal_connect(registerBut, "clicked", G_CALLBACK(submitRegister), &en);  
  g_signal_connect(G_OBJECT(window), "destroy",G_CALLBACK(gtk_main_quit), NULL); 

  gtk_widget_show_all(window);
  gtk_main();
}
void submitRegister(GtkWidget *widget, gpointer en) {
  struct regist *d = en;

  printf("Name: %s\n", gtk_entry_get_text (GTK_ENTRY(d->entry1)));
  printf("Phone: %s\n", gtk_entry_get_text (GTK_ENTRY(d->entry2)));
  printf("email: %s\n", gtk_entry_get_text (GTK_ENTRY(d->entry3)));
  printf("Password: %s\n", gtk_entry_get_text (GTK_ENTRY(d->entry4)));
  gtk_entry_set_text(GTK_ENTRY(d->entry1), "");
  gtk_entry_set_text(GTK_ENTRY(d->entry2), "");
  gtk_entry_set_text(GTK_ENTRY(d->entry3), "");
  gtk_entry_set_text(GTK_ENTRY(d->entry4), "");
  login(widget);
}
void dashboard(){
  deleteChildren();
  GtkWidget *button1;
  GtkWidget *button2;
  GtkWidget *button3;

  GtkWidget *logoutBut;
  char msg[20]= "Messages: ";

  strcat(msg, numb);

  printf("%s\n", msg);
  button1 = gtk_button_new_with_label("Booking a seat");
  button2 = gtk_button_new_with_label("Order food");
  button3 = gtk_button_new_with_label(msg);
  
  logoutBut = gtk_button_new_with_label("Log Out");

  gtk_grid_set_column_spacing(GTK_GRID(grid), 5);
  gtk_grid_set_row_spacing(GTK_GRID(grid), 5);

  gtk_grid_attach(GTK_GRID(grid), button1, 1,  1, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), button2, 2, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), button3, 3, 1, 1, 1);

  gtk_grid_attach(GTK_GRID(grid), logoutBut, 4, 1, 1, 1);

  g_signal_connect(G_OBJECT(button1), "clicked", G_CALLBACK(bookSeat), NULL);
  g_signal_connect(G_OBJECT(button2), "clicked", G_CALLBACK(orderFood), NULL);
  g_signal_connect(G_OBJECT(button3), "clicked", G_CALLBACK(messages), NULL);
  g_signal_connect(G_OBJECT(logoutBut), "clicked", G_CALLBACK(login), NULL);
  g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
  gtk_widget_show_all(window);
}
void bookSeat(){
  deleteChildren();
  GtkWidget *entry1;
  GtkWidget *entry2;
  GtkWidget *entry3;

  GtkWidget *submitBut;
  GtkWidget *backBut;

  GtkWidget *label1;
  GtkWidget *label2;
  GtkWidget *label3;

  label1 = gtk_label_new("Number of people:");
  label2 = gtk_label_new("Time of reservation(dd.mm.yyyy, h:m):");
  label3 = gtk_label_new("Comments:");

  entry1 = gtk_entry_new();
  entry2 = gtk_entry_new();
  entry3 = gtk_entry_new();

  backBut = gtk_button_new_with_label("Back");
  submitBut = gtk_button_new_with_label("submit");

  gtk_grid_set_column_spacing(GTK_GRID(grid), 5);
  gtk_grid_set_row_spacing(GTK_GRID(grid), 5);

  gtk_grid_attach(GTK_GRID(grid), label1, 0, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), label2, 0, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), label3, 0, 2, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), entry1, 1, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), entry2, 1, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), entry3, 1, 2, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), backBut, 3, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), submitBut, 1, 3, 1, 1);

  struct bookSeats book;
  book.entry1 = entry1;
  book.entry2 = entry2;
  book.entry3 = entry3;
  
  g_signal_connect(G_OBJECT(submitBut),"clicked", G_CALLBACK(submitBook), &book);
  g_signal_connect(G_OBJECT(backBut), "clicked", G_CALLBACK(dashboard), NULL);
  g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
  gtk_widget_show_all(window);
  gtk_main();
}
void submitBook(GtkWidget *widget, gpointer book){
  struct bookSeats *bk = book;

  printf("Number of people: %s\n", gtk_entry_get_text (GTK_ENTRY(bk->entry1)));
  printf("Time of reservation(dd.mm.yyyy, h:m): %s\n", gtk_entry_get_text (GTK_ENTRY(bk->entry2)));
  printf("Comments: %s\n", gtk_entry_get_text (GTK_ENTRY(bk->entry3)));

  show_info("Your book submitted");
  gtk_entry_set_text(GTK_ENTRY(bk->entry1), "");
  gtk_entry_set_text(GTK_ENTRY(bk->entry2), "");
  gtk_entry_set_text(GTK_ENTRY(bk->entry3), "");

}
void messages(){
  deleteChildren();

  int size = 5;
  for(int i = 0; i < size; i++)
  {
    gchar* index;
    index = g_strdup_printf("%d", i + 1);

    GtkWidget *number = gtk_label_new(index);
    gtk_grid_attach(GTK_GRID(grid), number, 0, i, 1, 1);

    GtkWidget *label = gtk_label_new("Your booking has successfully been accepted");
    gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
    gtk_grid_attach(GTK_GRID(grid), label, 1, i, 1, 1);
  }
  GtkWidget *backBut;
  backBut = gtk_button_new_with_label("Back");
  gtk_grid_attach(GTK_GRID(grid), backBut, 3, 0, 1, 1);

  g_signal_connect(G_OBJECT(backBut), "clicked", G_CALLBACK(dashboard), NULL);
  g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
  gtk_widget_show_all(window);
}
void orderFood(GtkWidget *widget){
  deleteChildren();
  int size = 5;
  for(int i = 0; i < size; i++)
  {
    gchar* index;
    index = g_strdup_printf("%d", i + 1);

    GtkWidget *number = gtk_label_new(index);
    gtk_grid_attach(GTK_GRID(grid), number, 0, i, 1, 1);

    GtkWidget *button = gtk_button_new_with_label("Category");
    gtk_grid_attach(GTK_GRID(grid), button, 1, i, 1, 1);

    g_signal_connect(G_OBJECT(button),"clicked", G_CALLBACK(chooseCategory), ((gpointer) (glong) (i+1)));
  }
  GtkWidget *backBut;
  backBut = gtk_button_new_with_label("Back");
  gtk_grid_attach(GTK_GRID(grid), backBut, 3, 0, 1, 1);

  g_signal_connect(G_OBJECT(backBut), "clicked", G_CALLBACK(dashboard), NULL);
  g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
  
  gtk_widget_show_all(window); 
}
void chooseCategory(GtkWidget *widget, gpointer i){
  deleteChildren();
  int index = ((gint) (glong) (i));
  printf("%d\n", index);
  int size = 5;
  for(int j = 0; j < size; j++)
  {
    gchar* index;
    index = g_strdup_printf("%d", j + 1);

    GtkWidget *number = gtk_label_new(index);
    gtk_grid_attach(GTK_GRID(grid), number, 0, j, 1, 1);

    GtkWidget *button = gtk_button_new_with_label("Plov");
    gtk_grid_attach(GTK_GRID(grid), button, 1, j, 1, 1);

    g_signal_connect(G_OBJECT(button),"clicked", G_CALLBACK(chooseFood), ((gpointer) (glong) (j+1)));
  }
  GtkWidget *backBut;
  backBut = gtk_button_new_with_label("Back");
  gtk_grid_attach(GTK_GRID(grid), backBut, 3, 0, 1, 1);
  
 
  g_signal_connect(G_OBJECT(backBut), "clicked", G_CALLBACK(orderFood), NULL);
  g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
  
  gtk_widget_show_all(window); 
}
void chooseFood(GtkWidget *widget, gpointer i){
 
  deleteChildren();
  int index = ((gint) (glong) (i));
  printf("%d\n", index);

  GtkWidget *backBut, *submitBut;
  GtkWidget *label1;
  GtkWidget *label2;
  GtkWidget *entry1;
  GtkWidget *entry2;

  label1 = gtk_label_new("Number of portions:");
  label2 = gtk_label_new("Address:");
  /* Create an adjustment representing an adjustable bounded value */
  GtkAdjustment *adjustment = gtk_adjustment_new(0,0,10,1,0, 0);

  entry1 = gtk_spin_button_new(GTK_ADJUSTMENT(adjustment), 1, 0);  
  // gtk_spin_button_set_numeric (entry1, TRUE);
  entry2 =  gtk_entry_new();


  gtk_grid_attach(GTK_GRID(grid), label1, 1, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), label2, 1, 2, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), entry1, 2, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), entry2, 2, 2, 1, 1);
  

  backBut = gtk_button_new_with_label("Back");
  submitBut = gtk_button_new_with_label("Submit");
  gtk_grid_attach(GTK_GRID(grid), backBut, 3, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), submitBut, 2, 3, 1, 1);
  gint number_int = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(entry1));
  
  
  struct orderFoods order;
  order.entry1 = entry1;
  order.entry2 = entry2;
  order.id = index;

  gtk_widget_show_all(window); 

  g_signal_connect(G_OBJECT(submitBut), "clicked", G_CALLBACK(submitOrder), &order);
  g_signal_connect(G_OBJECT(backBut), "clicked", G_CALLBACK(orderFood), NULL);
  g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
  gtk_main();
 
}
void submitOrder(GtkWidget *widget, gpointer order){
  struct orderFoods *ord = order;
  // int s= ((gint) (glong) (ord->entry1));
  // printf("Address: %d\n", s);
  printf("Contents of entries:%s\n", gtk_entry_get_text (GTK_ENTRY(ord->entry2)));
  printf("№ portions: %d\n", gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(ord->entry1)));
  printf("id: %d\n", ord->id);
  show_info("Your food order submitted");
  gtk_entry_set_text(GTK_ENTRY(ord->entry2), "");
  orderFood(widget);
  
}