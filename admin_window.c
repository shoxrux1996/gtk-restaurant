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
struct order
{
  GtkWidget *entry1;
  GtkWidget *entry2;
  GtkWidget *entry3;
  GtkWidget *entry4;
};
struct book
{
  GtkWidget *entry1;
  GtkWidget *entry2;
  GtkWidget *entry3;
  GtkWidget *entry4;
};
struct menu
{
  GtkWidget *entry1;
  GtkWidget *entry2;
  GtkWidget *entry3;
  GtkWidget *entry4;
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

void login(GtkWidget *widget);
void submitLogin(GtkWidget *widget, gpointer en);

void mainWind();
void dashboard();
void messages();

void listUsers(GtkWidget *widget);
void listOrders(GtkWidget *widget);
void listBooks(GtkWidget *widget);
void listMenus(GtkWidget *widget);


void editUser(GtkWidget *widget, gpointer i);
void updateUser(GtkWidget *widget, gpointer user);
void deleteUser(GtkWidget *widget, gpointer i);

void editOrder(GtkWidget *widget, gpointer i);
void updateOrder(GtkWidget *widget, gpointer order);
void deleteOrder(GtkWidget *widget, gpointer i);

void editBook(GtkWidget *widget, gpointer i);
void updateBook(GtkWidget *widget, gpointer order);
void deleteBook(GtkWidget *widget, gpointer i);

void editMenu(GtkWidget *widget, gpointer i);
void updateMenu(GtkWidget *widget, gpointer menu);
void deleteMenu(GtkWidget *widget, gpointer i);


int main(int argc, char *argv[]) {
  gtk_init(&argc, &argv);
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "Restaurant Management System");
  gtk_window_set_default_size(GTK_WINDOW(window), 600, 400);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_container_set_border_width(GTK_CONTAINER(window), 15);

  /*---- CSS ------------------*/
  GtkCssProvider *provider;
  GdkDisplay *display;
  GdkScreen *screen;
  
  provider = gtk_css_provider_new ();
  display = gdk_display_get_default ();
  screen = gdk_display_get_default_screen (display);
  gtk_style_context_add_provider_for_screen (screen, GTK_STYLE_PROVIDER (provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
 
  const gchar* style = "style.css";
  GError *error = 0;
  gtk_css_provider_load_from_file(provider, g_file_new_for_path(style), &error);
  g_object_unref (provider);
  /*---------------------------*/
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
    dashboard();
  }else{
    
    show_info("Entered wrong credentials");
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
  GtkWidget *button4;


  GtkWidget *logoutBut;
  

  button1 = gtk_button_new_with_label("Managing users");
  button2 = gtk_button_new_with_label("Managing orders");
  button3 = gtk_button_new_with_label("Managing books");
  button4 = gtk_button_new_with_label("Managing menu");
  
  logoutBut = gtk_button_new_with_label("Log Out");

  gtk_grid_attach(GTK_GRID(grid), button1, 1,  1, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), button2, 2, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), button3, 1, 2, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), button4, 2, 2, 1, 1);

  gtk_grid_attach(GTK_GRID(grid), logoutBut, 3, 1, 1, 1);


  g_signal_connect(G_OBJECT(button1), "clicked", G_CALLBACK(listUsers), NULL);
  g_signal_connect(G_OBJECT(button2), "clicked", G_CALLBACK(listOrders), NULL);
  g_signal_connect(G_OBJECT(button3), "clicked", G_CALLBACK(listBooks), NULL);
  g_signal_connect(G_OBJECT(button4), "clicked", G_CALLBACK(listMenus), NULL);
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
  gtk_grid_attach(GTK_GRID(grid), backBut, 5, 1, 1, 1);

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

    g_signal_connect(G_OBJECT(delete),"clicked", G_CALLBACK(deleteUser), ((gpointer) (glong) (j+1)));
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
  
  listUsers(widget);
}
void deleteUser(GtkWidget *widget, gpointer i){
  show_info("User deleted successfuly");
  deleteChildren();
  int index = ((gint) (glong) (i));
  printf("%d\n", index);
  listUsers(widget);
}
void listOrders(GtkWidget *widget){
  deleteChildren();

  int size = 5;
 
  gtk_grid_set_column_spacing(GTK_GRID(grid), 5);
  gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
  
  GtkWidget *number = gtk_label_new(NULL);  
  gtk_label_set_markup(GTK_LABEL(number), "<b>#</b>");
  gtk_grid_attach(GTK_GRID(grid), number, 0, 1, 1, 1);

  GtkWidget *name = gtk_label_new(NULL);  
  gtk_label_set_markup(GTK_LABEL(name), "<b>User</b>");
  gtk_grid_attach(GTK_GRID(grid), name, 1, 1, 1, 1);

  GtkWidget *dish = gtk_label_new(NULL);
  gtk_label_set_markup(GTK_LABEL(dish), "<b>Dish</b>");
  gtk_grid_attach(GTK_GRID(grid), dish, 2, 1, 1, 1);

  GtkWidget *status = gtk_label_new(NULL);
  gtk_label_set_markup(GTK_LABEL(status), "<b>Status</b>");
  gtk_grid_attach(GTK_GRID(grid), status, 3, 1, 1, 1);

  GtkWidget *address = gtk_label_new(NULL);
  gtk_label_set_markup(GTK_LABEL(address), "<b>Address</b>");
  gtk_grid_attach(GTK_GRID(grid), address, 4, 1, 1, 1);

  GtkWidget *backBut;
  backBut = gtk_button_new_with_label("Back");
  gtk_grid_attach(GTK_GRID(grid), backBut, 6, 1, 1, 1);

  for(int j = 2; j <= size; j++)
  {
    gchar* index;
    index = g_strdup_printf("%d", j + 1);

    GtkWidget *number = gtk_label_new(index);
    gtk_grid_attach(GTK_GRID(grid), number, 0, j, 1, 1);

    GtkWidget *name = gtk_label_new("Alexandrer Stokes");
    gtk_grid_attach(GTK_GRID(grid), name, 1, j, 1, 1);

    GtkWidget *phone = gtk_label_new("Palov");
    gtk_grid_attach(GTK_GRID(grid), phone, 2, j, 1, 1);

    int status = 2;//status of the order
    char status_text[10]="unchacked";
    switch(status){
      case 0:
        strcpy(status_text, "unchacked");
        break;
      case 1:
        strcpy(status_text, "rejected");
        break;
      case 2:
        strcpy(status_text, "delivered");
        break;
      default:  
        strcpy(status_text, "approved");
    }
    GtkWidget *email = gtk_label_new(status_text);
    gtk_grid_attach(GTK_GRID(grid), email, 3, j, 1, 1);

    GtkWidget *address = gtk_label_new("Ko'kcha adsdas dkasdaskdkasdk kdksa");
    gtk_grid_attach(GTK_GRID(grid), address, 4, j, 1, 1);

    GtkWidget *edit = gtk_button_new_with_label("edit");
    gtk_grid_attach(GTK_GRID(grid), edit, 5, j, 1, 1);

    GtkWidget *delete = gtk_button_new_with_label("delete");
    gtk_grid_attach(GTK_GRID(grid), delete, 6, j, 1, 1);

    g_signal_connect(G_OBJECT(delete),"clicked", G_CALLBACK(deleteOrder), ((gpointer) (glong) (j+1)));
    g_signal_connect(G_OBJECT(edit),"clicked", G_CALLBACK(editOrder), ((gpointer) (glong) (j+1)));
  }
 
  g_signal_connect(G_OBJECT(backBut), "clicked", G_CALLBACK(dashboard), NULL);
  g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
  
  gtk_widget_show_all(window); 
}
void editOrder(GtkWidget *widget, gpointer i){
  deleteChildren();
  int index = ((gint) (glong) (i));
  GtkWidget *label1;
  GtkWidget *label2;
  GtkWidget *label3;
  GtkWidget *label4;

  label1 = gtk_label_new("User:");
  label2 = gtk_label_new("Dish:");
  label3 = gtk_label_new("Status:");
  label4 = gtk_label_new("Address:");
  GtkWidget *entry1;
  GtkWidget *entry2;
  GtkWidget *entry3;
  GtkWidget *entry4;

  entry1 = gtk_entry_new();
  entry2 = gtk_entry_new();
  entry3 = gtk_combo_box_text_new();
  entry4 = gtk_entry_new();
  gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (entry3), "unchacked");
  gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (entry3), "rejected");
  gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (entry3), "delivered");
  gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (entry3), "approved");


  gtk_entry_set_width_chars (GTK_ENTRY(entry1), 60);
  gtk_entry_set_text(GTK_ENTRY(entry1), "Shokhrukh");
  gtk_entry_set_width_chars (GTK_ENTRY(entry2), 60);
  gtk_entry_set_text(GTK_ENTRY(entry2), "Palov");
  /* Choose to set the first row as the active one by default, from the beginning */
  gtk_combo_box_set_wrap_width (GTK_COMBO_BOX(entry3), 60);
  gtk_combo_box_set_active (GTK_COMBO_BOX (entry3), 2);
  gtk_entry_set_width_chars (GTK_ENTRY(entry4), 60);
  gtk_entry_set_text(GTK_ENTRY(entry4), "Toshkent, Ziyolilar 9");

  GtkWidget *backBut;
  backBut = gtk_button_new_with_label("back");
  GtkWidget *update;
  update = gtk_button_new_with_label("update");

  gtk_grid_attach(GTK_GRID(grid), label1, 1, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), label2, 1, 2, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), label3, 1, 3, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), label4, 1, 4, 1, 1);

  gtk_grid_attach(GTK_GRID(grid), entry1, 2, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), entry2, 2, 2, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), entry3, 2, 3, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), entry4, 2, 4, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), backBut,3, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), update, 2, 5, 1, 1);

  struct order or;
  or.entry1 = entry1;
  or.entry2 = entry2;
  or.entry3 = entry3;
  or.entry4 = entry4;

  gtk_widget_show_all(window);

  g_signal_connect(update, "clicked", G_CALLBACK(updateOrder), &or);  
  g_signal_connect(backBut, "clicked", G_CALLBACK(listOrders), NULL);  
  g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
  gtk_main();
}
void updateOrder(GtkWidget *widget, gpointer order){
  struct order *or = order;

  printf("Name:%s\n", gtk_entry_get_text (GTK_ENTRY(or->entry1)));
  printf("Dish:%s\n", gtk_entry_get_text (GTK_ENTRY(or->entry2)));
  printf("Selected index:%s\n", gtk_combo_box_text_get_active_text (GTK_COMBO_BOX_TEXT(or->entry3)));
  printf("Dish:%s\n", gtk_entry_get_text (GTK_ENTRY(or->entry4)));
  show_info("Order updated successfully");

  listOrders(widget);
}
void deleteOrder(GtkWidget *widget, gpointer i){
  show_info("Order deleted successfuly");

  int index = ((gint) (glong) (i));
  printf("%d\n", index);
  listOrders(widget);
}
void listBooks(GtkWidget *widget){
  deleteChildren();

  int size = 5;
 
  gtk_grid_set_column_spacing(GTK_GRID(grid), 5);
  gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
  
  GtkWidget *number = gtk_label_new(NULL);  
  gtk_label_set_markup(GTK_LABEL(number), "<b>#</b>");
  gtk_grid_attach(GTK_GRID(grid), number, 0, 1, 1, 1);

  GtkWidget *name = gtk_label_new(NULL);  
  gtk_label_set_markup(GTK_LABEL(name), "<b>User</b>");
  gtk_grid_attach(GTK_GRID(grid), name, 1, 1, 1, 1);

  GtkWidget *table = gtk_label_new(NULL);
  gtk_label_set_markup(GTK_LABEL(table), "<b>Table</b>");
  gtk_grid_attach(GTK_GRID(grid), table, 2, 1, 1, 1);

  GtkWidget *time = gtk_label_new(NULL);
  gtk_label_set_markup(GTK_LABEL(time), "<b>Time</b>");
  gtk_grid_attach(GTK_GRID(grid), time, 3, 1, 1, 1);

  GtkWidget *stat = gtk_label_new(NULL);
  gtk_label_set_markup(GTK_LABEL(stat), "<b>Status</b>");
  gtk_grid_attach(GTK_GRID(grid), stat, 4, 1, 1, 1);


  GtkWidget *backBut;
  backBut = gtk_button_new_with_label("Back");
  gtk_grid_attach(GTK_GRID(grid), backBut, 6, 1, 1, 1);

  for(int j = 2; j <= size; j++)
  {
    gchar* index;
    index = g_strdup_printf("%d", j + 1);

    GtkWidget *number = gtk_label_new(index);
    gtk_grid_attach(GTK_GRID(grid), number, 0, j, 1, 1);

    GtkWidget *name = gtk_label_new("Alexandrer Stokes");
    gtk_grid_attach(GTK_GRID(grid), name, 1, j, 1, 1);

    GtkWidget *table = gtk_label_new("1");
    gtk_grid_attach(GTK_GRID(grid), table, 2, j, 1, 1);

    GtkWidget *time = gtk_label_new("12.12.2017 00:00");
    gtk_grid_attach(GTK_GRID(grid), time, 3, j, 1, 1);


    int status = 2;//status of the order
    char status_text[10]="unchacked";
    switch(status){
      case 0:
        strcpy(status_text, "unchacked");
        break;
      case 1:
        strcpy(status_text, "rejected");
        break;
      case 2:
        strcpy(status_text, "delivered");
        break;
      default:  
        strcpy(status_text, "approved");
    }

    GtkWidget *stat = gtk_label_new(status_text);
    gtk_grid_attach(GTK_GRID(grid), stat, 4, j, 1, 1);

    GtkWidget *edit = gtk_button_new_with_label("edit");
    gtk_grid_attach(GTK_GRID(grid), edit, 5, j, 1, 1);

    GtkWidget *delete = gtk_button_new_with_label("delete");
    gtk_grid_attach(GTK_GRID(grid), delete, 6, j, 1, 1);

    g_signal_connect(G_OBJECT(delete),"clicked", G_CALLBACK(deleteBook), ((gpointer) (glong) (j+1)));
    g_signal_connect(G_OBJECT(edit),"clicked", G_CALLBACK(editBook), ((gpointer) (glong) (j+1)));
  }
 
  g_signal_connect(G_OBJECT(backBut), "clicked", G_CALLBACK(dashboard), NULL);
  g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
  
  gtk_widget_show_all(window); 
}
void editBook(GtkWidget *widget, gpointer i){
  deleteChildren();
  int index = ((gint) (glong) (i));
  GtkWidget *label1;
  GtkWidget *label2;
  GtkWidget *label3;
  GtkWidget *label4;

  label1 = gtk_label_new("User:");
  label2 = gtk_label_new("Table:");
  label3 = gtk_label_new("Time:");
  label4 = gtk_label_new("Status:");
  GtkWidget *entry1;
  GtkWidget *entry2;
  GtkWidget *entry3;
  GtkWidget *entry4;

  entry1 = gtk_entry_new();
  entry2 = gtk_combo_box_text_new();
  entry3 = gtk_entry_new();
  entry4 = gtk_combo_box_text_new();

  for (int i = 1; i < 5; i++){
    gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (entry2), g_strdup_printf("%d", i));
  }

  gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (entry4), "unchacked");
  gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (entry4), "rejected");
  gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (entry4), "delivered");
  gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (entry4), "approved");


  gtk_entry_set_width_chars (GTK_ENTRY(entry1), 60);
  gtk_entry_set_text(GTK_ENTRY(entry1), "Shokhrukh");
  /* Choose to set the first row as the active one by default, from the beginning */
  gtk_combo_box_set_wrap_width (GTK_COMBO_BOX(entry2), 60);
  gtk_combo_box_set_active (GTK_COMBO_BOX (entry2), 1);
  gtk_entry_set_width_chars (GTK_ENTRY(entry3), 60);
  gtk_entry_set_text(GTK_ENTRY(entry3), "12.12.2017 00:00");
    /* Choose to set the first row as the active one by default, from the beginning */
  gtk_combo_box_set_wrap_width (GTK_COMBO_BOX(entry4), 60);
  gtk_combo_box_set_active (GTK_COMBO_BOX (entry4), 2);

  GtkWidget *backBut;
  backBut = gtk_button_new_with_label("back");
  GtkWidget *update;
  update = gtk_button_new_with_label("update");

  gtk_grid_attach(GTK_GRID(grid), label1, 1, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), label2, 1, 2, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), label3, 1, 3, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), label4, 1, 4, 1, 1);

  gtk_grid_attach(GTK_GRID(grid), entry1, 2, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), entry2, 2, 2, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), entry3, 2, 3, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), entry4, 2, 4, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), backBut,3, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), update, 2, 5, 1, 1);

  struct book or;
  or.entry1 = entry1;
  or.entry2 = entry2;
  or.entry3 = entry3;
  or.entry4 = entry4;

  gtk_widget_show_all(window);

  g_signal_connect(update, "clicked", G_CALLBACK(updateBook), &or);  
  g_signal_connect(backBut, "clicked", G_CALLBACK(listBooks), NULL);  
  g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
  gtk_main();
}
void updateBook(GtkWidget *widget, gpointer book){
  struct book *or = book;

  printf("Name:%s\n", gtk_entry_get_text (GTK_ENTRY(or->entry1)));
  printf("Table:%s\n", gtk_combo_box_text_get_active_text (GTK_COMBO_BOX_TEXT(or->entry2)));
  printf("Time:%s\n", gtk_entry_get_text (GTK_ENTRY(or->entry3)));
  printf("Status:%s\n", gtk_combo_box_text_get_active_text (GTK_COMBO_BOX_TEXT(or->entry4)));

  show_info("Book updated successfully");
  listBooks(widget);
}
void deleteBook(GtkWidget *widget, gpointer i){
  show_info("Book deleted successfuly");
  int index = ((gint) (glong) (i));
  printf("%d\n", index);
  listBooks(widget);
}
void listMenus(GtkWidget *widget){
  deleteChildren();

  int size = 5;
  char status_text[10]="unchacked";
 
  gtk_grid_set_column_spacing(GTK_GRID(grid), 5);
  gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
  
  GtkWidget *number = gtk_label_new(NULL);  
  gtk_label_set_markup(GTK_LABEL(number), "<b>#</b>");
  gtk_grid_attach(GTK_GRID(grid), number, 0, 1, 1, 1);

  GtkWidget *name = gtk_label_new(NULL);  
  gtk_label_set_markup(GTK_LABEL(name), "<b>User</b>");
  gtk_grid_attach(GTK_GRID(grid), name, 1, 1, 1, 1);

  GtkWidget *price = gtk_label_new(NULL);
  gtk_label_set_markup(GTK_LABEL(price), "<b>Price</b>");
  gtk_grid_attach(GTK_GRID(grid), price, 2, 1, 1, 1);

  GtkWidget *category = gtk_label_new(NULL);
  gtk_label_set_markup(GTK_LABEL(category), "<b>Category</b>");
  gtk_grid_attach(GTK_GRID(grid), category, 3, 1, 1, 1);

  GtkWidget *stat = gtk_label_new(NULL);
  gtk_label_set_markup(GTK_LABEL(stat), "<b>Status</b>");
  gtk_grid_attach(GTK_GRID(grid), stat, 4, 1, 1, 1);


  GtkWidget *backBut;
  backBut = gtk_button_new_with_label("Back");
  gtk_grid_attach(GTK_GRID(grid), backBut, 6, 1, 1, 1);

  for(int j = 2; j <= size; j++)
  {
    gchar* index;
    index = g_strdup_printf("%d", j + 1);

    GtkWidget *number = gtk_label_new(index);
    gtk_grid_attach(GTK_GRID(grid), number, 0, j, 1, 1);

    GtkWidget *name = gtk_label_new("Alexandrer Stokes");
    gtk_grid_attach(GTK_GRID(grid), name, 1, j, 1, 1);


    GtkWidget *price = gtk_label_new("1000");
    gtk_grid_attach(GTK_GRID(grid), price, 2, j, 1, 1);

    GtkWidget *category = gtk_label_new(index);
    gtk_grid_attach(GTK_GRID(grid), category, 3, j, 1, 1);

    int status = 2;//status of the order
    switch(status){
      case 0:
        strcpy(status_text, "unchacked");
        break;
      case 1:
        strcpy(status_text, "rejected");
        break;
      case 2:
        strcpy(status_text, "delivered");
        break;
      default:  
        strcpy(status_text, "approved");
    }

    GtkWidget *stat = gtk_label_new(status_text);
    gtk_grid_attach(GTK_GRID(grid), stat, 4, j, 1, 1);

    GtkWidget *edit = gtk_button_new_with_label("edit");
    gtk_grid_attach(GTK_GRID(grid), edit, 5, j, 1, 1);

    GtkWidget *delete = gtk_button_new_with_label("delete");  
    gtk_grid_attach(GTK_GRID(grid), delete, 6, j, 1, 1);

    g_signal_connect(G_OBJECT(delete),"clicked", G_CALLBACK(deleteMenu), ((gpointer) (glong) (j+1)));
    g_signal_connect(G_OBJECT(edit),"clicked", G_CALLBACK(editMenu), ((gpointer) (glong) (j+1)));
  }
 
  g_signal_connect(G_OBJECT(backBut), "clicked", G_CALLBACK(dashboard), NULL);
  g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
  
  gtk_widget_show_all(window); 
}
void editMenu(GtkWidget *widget, gpointer i){
  deleteChildren();
  int index = ((gint) (glong) (i));
  GtkWidget *label1;
  GtkWidget *label2;
  GtkWidget *label3;
  GtkWidget *label4;

  label1 = gtk_label_new("User:");
  label2 = gtk_label_new("Price:");
  label3 = gtk_label_new("Category:");
  label4 = gtk_label_new("Status:");
  GtkWidget *entry1;
  GtkWidget *entry2;
  GtkWidget *entry3;
  GtkWidget *entry4;

  entry1 = gtk_entry_new();
  GtkAdjustment *adjustment = gtk_adjustment_new(0,0,10000000,1,0, 0);
  entry2 = gtk_spin_button_new(GTK_ADJUSTMENT(adjustment), 1, 0);
  entry3 = gtk_combo_box_text_new();
  entry4 = gtk_combo_box_text_new();

  for (int i = 1; i < 5; i++){
    gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (entry3), g_strdup_printf("%d", i));
  }

  gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (entry4), "unchacked");
  gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (entry4), "rejected");
  gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (entry4), "delivered");
  gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (entry4), "approved");


  gtk_entry_set_width_chars (GTK_ENTRY(entry1), 60);
  gtk_entry_set_text(GTK_ENTRY(entry1), "Shokhrukh");

  gtk_entry_set_width_chars (GTK_ENTRY(entry2), 60);
  gtk_spin_button_set_value (GTK_SPIN_BUTTON(entry2), 2600);

  /* Choose to set the first row as the active one by default, from the beginning */
  gtk_combo_box_set_wrap_width (GTK_COMBO_BOX(entry3), 60);
  gtk_combo_box_set_active (GTK_COMBO_BOX (entry3), 1);

    /* Choose to set the first row as the active one by default, from the beginning */
  gtk_combo_box_set_wrap_width (GTK_COMBO_BOX(entry4), 60);
  gtk_combo_box_set_active (GTK_COMBO_BOX (entry4), 2);

  GtkWidget *backBut;
  backBut = gtk_button_new_with_label("back");
  GtkWidget *update;
  update = gtk_button_new_with_label("update");

  gtk_grid_attach(GTK_GRID(grid), label1, 1, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), label2, 1, 2, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), label3, 1, 3, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), label4, 1, 4, 1, 1);

  gtk_grid_attach(GTK_GRID(grid), entry1, 2, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), entry2, 2, 2, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), entry3, 2, 3, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), entry4, 2, 4, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), backBut,3, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), update, 2, 5, 1, 1);

  struct menu or;
  or.entry1 = entry1;
  or.entry2 = entry2;
  or.entry3 = entry3;
  or.entry4 = entry4;

  gtk_widget_show_all(window);

  g_signal_connect(update, "clicked", G_CALLBACK(updateMenu), &or);  
  g_signal_connect(backBut, "clicked", G_CALLBACK(listMenus), NULL);  
  g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
  gtk_main();
}
void updateMenu(GtkWidget *widget, gpointer menu){
  struct menu *or = menu;
  printf("Name:%s\n", gtk_entry_get_text (GTK_ENTRY(or->entry1)));
  printf("Price:%d\n", gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(or->entry2)));
  printf("Category:%s\n", gtk_combo_box_text_get_active_text (GTK_COMBO_BOX_TEXT(or->entry3)));
  printf("Status:%s\n", gtk_combo_box_text_get_active_text (GTK_COMBO_BOX_TEXT(or->entry4)));
  show_info("Menu updated successfully");
  listMenus(widget);
}
void deleteMenu(GtkWidget *widget, gpointer i){
  show_info("Menu deleted successfuly");
  int index = ((gint) (glong) (i));
  printf("%d\n", index);
  listMenus(widget);
}