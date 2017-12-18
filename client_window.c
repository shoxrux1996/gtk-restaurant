#include <gtk/gtk.h>
#include <glib.h>
#include <glib/gprintf.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h> 


#define PORT 8080
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define ANSI_COLOR_BLUE    "\x1b[34m"

GtkWidget *window; //global window
GtkWidget *grid; 
char numb[10] = "0"; //number of messages
// global fixed container

typedef struct 
{
  GtkWidget *entry1;
  GtkWidget *entry2;
} log;
log lg;

typedef struct 
{
  GtkWidget *entry1;
  GtkWidget *entry2;
  GtkWidget *entry3;
  GtkWidget *entry4;
} regist;
regist rg;

typedef struct  
{
  GtkWidget *entry1;
  GtkWidget *entry2;
  GtkWidget *entry3;
} bookSeats;
bookSeats bs;
typedef struct 
{
  GtkWidget *entry1;
  GtkWidget *entry2;
  int id;
}orderFoods;
orderFoods of;

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
void registe();
void login();
void dashboard();
void messages();
void bookSeat();
void submitLogin();
void submitRegister();
void submitBook();
void chooseCategory(GtkWidget *widget, gpointer i);
void chooseFood(GtkWidget *widget, gpointer i);
void orderFood();
void submitOrder();

typedef struct{
  int ID;
  char name[255];
  char phone[12];
  char email[255];
  char password[255];
} User;

typedef struct{
  int day;
  int month;
  int year;

  int hour;
  int minute;

  char format[20];
} Time;

typedef struct{
  int ID;
  int price;
  char name[255];
} Meal;

typedef struct{
  bool accepted;
  char comment[255]; 
} Request_result;

void menu();
void login_menu();
void register_menu();
void dashboard_menu(User *user);
User* authenticate(char *email, char *password);
void booking_menu(User *user);
void ordering_food_menu(User *user);
int choose_meal(User *user, int);
void messages_menu(User *user);
Request_result request_booking(User *, int, Time, char *);
Request_result request_order(User *, int, int, char *);
Request_result request_register(char *, char *, char *, char *);
int createSocket();
int server_socket;
User *user= NULL;
 
int main(int argc, char *argv[]) {

  server_socket = createSocket();
  gtk_init(&argc, &argv);
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  g_object_ref_sink(window);
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
void login() {
  deleteChildren();
  user = NULL;
  GtkWidget *label1;
  GtkWidget *label2;
  GtkWidget *entry1;
  GtkWidget *entry2;
  GtkWidget *loginBut;
  GtkWidget *backBut;

  label1 = gtk_label_new("Email");
  label2 = gtk_label_new("Password");

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
  
  


  gtk_widget_show_all(window);

  lg.entry1 = entry1;
  lg.entry2 = entry2;
  g_signal_connect(backBut, "clicked", G_CALLBACK(mainWind), NULL);  
  g_signal_connect(loginBut, "clicked", G_CALLBACK(submitLogin), NULL);
  g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL); 
  

}
void submitLogin() {
  
  char *email = (char*)gtk_entry_get_text (GTK_ENTRY(lg.entry1));
  char *password = (char*)gtk_entry_get_text (GTK_ENTRY(lg.entry2));
 

  if(strcmp(email, "") == 0 || strcmp(password, "") == 0){
    show_info("All inputs are required! ");
    login();
  }
  
  user = authenticate(email, password);


  if(user != NULL)
  {
    printf(ANSI_COLOR_GREEN "User found!\n" ANSI_COLOR_RESET);
    sleep(1);
    dashboard();
  } 
  else
  {
    printf(ANSI_COLOR_RED "Wrong email or password!\n" ANSI_COLOR_RESET);
    sleep(2);
    show_info("Wrong email or password!");
    login();
  }
}
void registe(){
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


  rg.entry1 = entry1;
  rg.entry2 = entry2;
  rg.entry3 = entry3;
  rg.entry4 = entry4;

  g_signal_connect(backBut, "clicked", G_CALLBACK(mainWind), NULL);  
  g_signal_connect(registerBut, "clicked", G_CALLBACK(submitRegister), NULL);  
  g_signal_connect(G_OBJECT(window), "destroy",G_CALLBACK(gtk_main_quit), NULL); 

  gtk_widget_show_all(window);

  
}
void submitRegister() {
  
  char *name =(char *) gtk_entry_get_text (GTK_ENTRY(rg.entry1));
  char *phone =(char *) gtk_entry_get_text (GTK_ENTRY(rg.entry2));
  char *email =(char *) gtk_entry_get_text (GTK_ENTRY(rg.entry3));
  char *password =(char *) gtk_entry_get_text (GTK_ENTRY(rg.entry4));

  if(strcmp(name, "") == 0 || strcmp(email, "") == 0 || strcmp(password, "") == 0 || strcmp(phone, "") == 0){
      show_info("All inputs are required! ");
      registe();
  }

  Request_result reg;
  reg = request_register(name, phone, email, password);
  if(reg.accepted)
  {
    printf( ANSI_COLOR_GREEN "%s\n" ANSI_COLOR_RESET, reg.comment);
    show_info(reg.comment);
    login();
    
  }
  else{
    printf( ANSI_COLOR_RED "%s\n" ANSI_COLOR_RESET, reg.comment);
    show_info(reg.comment);
  }

}
void dashboard(){
  char query[512];
  int num_rows;
  char row_string[1024];
  int new_messages;  
  int choice; 
 
  sprintf(query, "SELECT COUNT(*) FROM Messages WHERE User_id = %d AND Status = 0", user->ID);
  send(server_socket, query, sizeof(query), 0);
  recv(server_socket, &num_rows, sizeof(int), 0);

  recv(server_socket, row_string, sizeof(row_string), 0);
  sscanf(row_string, " '%d'", &new_messages);
  sprintf(numb, "%d",new_messages);

  deleteChildren();
  GtkWidget *button1;
  GtkWidget *button2;
  GtkWidget *button3;

  GtkWidget *logoutBut;
  char msg[20]= "Messages: ";
  strcat(msg, numb);

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

  GtkAdjustment *adjustment = gtk_adjustment_new(0,0,10000000,1,0, 0);
  entry1 = gtk_spin_button_new(GTK_ADJUSTMENT(adjustment), 1, 0);
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

  
  bs.entry1 = entry1;
  bs.entry2 = entry2;
  bs.entry3 = entry3;
  
  g_signal_connect(G_OBJECT(submitBut),"clicked", G_CALLBACK(submitBook), NULL);
  g_signal_connect(G_OBJECT(backBut), "clicked", G_CALLBACK(dashboard), NULL);
  g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
  gtk_widget_show_all(window);
 
}
void submitBook(GtkWidget *widget){
  int num_of_people;
  Time *time = malloc(sizeof(Time));
  char *comments;
  char *time_reserve;


  num_of_people = (int)gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(bs.entry1));
  comments = (char*) gtk_entry_get_text (GTK_ENTRY(bs.entry3));
  time_reserve = (char *)gtk_entry_get_text (GTK_ENTRY(bs.entry2));
  sscanf(time_reserve,"%d.%d.%d, %d:%d", &time->day, &time->month, &time->year, &time->hour, &time->minute);

  sprintf(time->format, "%04d-%02d-%02d %02d:%02d:00", time->year, time->month, time->day, time->hour, time->minute);

  Request_result booking = request_booking(user, num_of_people, *time, comments);
  free(time);
  if(booking.accepted){
    show_info(booking.comment);
    printf( ANSI_COLOR_GREEN "%s\n" ANSI_COLOR_RESET, booking.comment);
  }
  else{
    show_info(booking.comment);
    printf( ANSI_COLOR_RED "%s\n" ANSI_COLOR_RESET, booking.comment);
  }
 
}
void messages(){
  deleteChildren();
  char query[512];
  char row_string[1024];
  char message[512];
  int status;
  int num_rows;
  int choice;

  sprintf(query, "SELECT Text, Status FROM Messages WHERE User_id = %d ORDER BY ID DESC", user->ID);
  send(server_socket, query, sizeof(query), 0);
  recv(server_socket, &num_rows, sizeof(num_rows), 0);

  
  for(int i = 0; i < num_rows; ++i)
  {
    recv(server_socket, row_string, sizeof(row_string), 0);
    sscanf(row_string, " '%[^']' '%d'", message, &status);

    gchar* index;
    index = g_strdup_printf("%d", i + 1);

    GtkWidget *number = gtk_label_new(index);
    gtk_grid_attach(GTK_GRID(grid), number, 0, i, 1, 1);

    GtkWidget *label = gtk_label_new(message);
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
void orderFood(){
  deleteChildren();
  int size = 4;
  for(int i = 0; i < size; i++)
  {
    gchar* index;
    index = g_strdup_printf("%d", i+1);

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
  int category_id = ((gint) (glong) (i));
 

  char query[512];
  char row_string[1024];
  int num_rows;

  sprintf(query, "SELECT ID, Name, Price FROM Dishes WHERE category_id = %d AND Status = 1", category_id);
  send(server_socket, query, sizeof(query), 0);
  recv(server_socket, &num_rows, sizeof(int), 0);
  Meal meals[num_rows];

  int choice;

  for (int i = 0; i < num_rows; ++i)
  {
    recv(server_socket, row_string, sizeof(row_string), 0);
    sscanf(row_string, " '%d' '%[^']' '%d'", &meals[i].ID, meals[i].name, &meals[i].price);
    //printf("%-15d%-15s%-15d\n", meals[i].ID, meals[i].name, meals[i].price);
  }

  GtkWidget *number = gtk_label_new(NULL);  
  gtk_label_set_markup(GTK_LABEL(number), "<b>#</b>");
  gtk_grid_attach(GTK_GRID(grid), number, 0, 0, 1, 1);

  GtkWidget *price = gtk_label_new(NULL);  
  gtk_label_set_markup(GTK_LABEL(price), "<b>Price</b>");
  gtk_label_set_width_chars (GTK_LABEL(price), 20);
  gtk_grid_attach(GTK_GRID(grid), price, 1, 0, 1, 1);
  
  GtkWidget *category = gtk_label_new(NULL);  
  gtk_label_set_markup(GTK_LABEL(category), "<b>Dish</b>");
  gtk_label_set_width_chars (GTK_LABEL(category), 20);
  gtk_grid_attach(GTK_GRID(grid), category, 2, 0, 1, 1);
  
  GtkWidget *backBut;
  backBut = gtk_button_new_with_label("Back");
  gtk_grid_attach(GTK_GRID(grid), backBut, 3, 0, 1, 1);

  for(int j = 0; j < num_rows; ++j)
  {
    gchar* index;
    index = g_strdup_printf("%d", meals[j].ID);
    gchar* meal_price;
    meal_price = g_strdup_printf("%d", meals[j].price);

    GtkWidget *number = gtk_label_new(index);
    gtk_grid_attach(GTK_GRID(grid), number, 0, j+1, 1, 1);

    GtkWidget *price = gtk_label_new(meal_price);
    gtk_grid_attach(GTK_GRID(grid), price, 1, j+1, 1, 1);

    GtkWidget *button = gtk_button_new_with_label(meals[j].name);
    gtk_grid_attach(GTK_GRID(grid), button, 2, j+1, 1, 1);


    g_signal_connect(G_OBJECT(button),"clicked", G_CALLBACK(chooseFood), ((gpointer) (glong) (meals[j].ID)));
  }

  
 
  g_signal_connect(G_OBJECT(backBut), "clicked", G_CALLBACK(orderFood), NULL);
  g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
  
  gtk_widget_show_all(window); 
}
void chooseFood(GtkWidget *widget, gpointer i){
 
  deleteChildren();
  int index = ((gint) (glong) (i));

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
  
  
  of.entry1 = entry1;
  of.entry2 = entry2;
  of.id = index;

  gtk_widget_show_all(window); 

  g_signal_connect(G_OBJECT(submitBut), "clicked", G_CALLBACK(submitOrder), NULL);
  g_signal_connect(G_OBJECT(backBut), "clicked", G_CALLBACK(orderFood), NULL);
  g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
  
 
}
void submitOrder(){

  // int s= ((gint) (glong) (ord->entry1));
  // printf("Address: %d\n", s);
  int portions= (int)gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(of.entry1));
  char *address =(char *) gtk_entry_get_text (GTK_ENTRY(of.entry2));

 
  Request_result ordering = request_order(user, of.id, portions, address);

  if(ordering.accepted){
    show_info(ordering.comment);
    printf( ANSI_COLOR_GREEN "%s\n" ANSI_COLOR_RESET, ordering.comment);
  }
  else{
    show_info(ordering.comment);
    printf( ANSI_COLOR_RED "%s\n" ANSI_COLOR_RESET, ordering.comment);
  }

  sleep(2);
  orderFood();
  
}

User* authenticate(char *email, char *password){//return user struct on success, NULL otherwise
  User *user = NULL;
  
  char query[512];
  int num_rows;
  char row_string[1024];

  sprintf(query, "SELECT ID, name, phone, email, password FROM Users WHERE email = '%s' AND password = '%s'", email, password);

  send(server_socket, query, sizeof(query), 0);
  recv(server_socket, &num_rows, sizeof(int), 0);
  //printf("Size of int: %d\n", (int)sizeof(int));
  //sleep(5);
  if(num_rows>0)
  {
    user = malloc(sizeof(User));
    recv(server_socket, row_string, sizeof(row_string), 0);
    sscanf(row_string, " '%d' '%[^']' '%[^']' '%[^']' '%[^']'", &user->ID, user->name, user->phone, user->email, user->password);
  
  }
  
  return user;
}
Request_result request_register(char *name, char *phone, char *email, char *password){

  char query[512];
  int num_rows;
  char request_result_string[256];
  Request_result reg;

  sprintf(query, "INSERT INTO Users (Name, Phone, Email, Password) VALUES ('%s', '%s', '%s', '%s')", name, phone, email, password);

  send(server_socket, query, sizeof(query), 0);
  recv(server_socket, &num_rows, sizeof(int), 0);


  recv(server_socket, request_result_string, sizeof(request_result_string), 0);
  
  sscanf(request_result_string, "%d `%[^`]", (int *)&reg.accepted, reg.comment);

  return reg;
}
Request_result request_booking(User *user, int num_of_people, Time time, char *comments){// return Booking_req struct object 

  char query[512];
  Time post_time;
  Time pre_time;
  int num_rows;
  char request_result_string[256];
  Request_result booking;

  sprintf(query, "INSERT INTO Bookings (User_id, Table_id, Time, Status) SELECT %d, ID, '%s', 0 FROM Tables WHERE ID NOT IN (SELECT Table_id FROM Bookings WHERE Time BETWEEN ('%s' - INTERVAL '3' HOUR) AND ('%s' + INTERVAL '3' HOUR)) AND '%s' > NOW() + INTERVAL '20' MINUTE AND No_of_people > %d AND status = true ORDER BY No_of_people LIMIT 1", user->ID, time.format, time.format, time.format, time.format, num_of_people);
  send(server_socket, query, sizeof(query), 0);
  recv(server_socket, &num_rows, sizeof(int), 0);
  
  recv(server_socket, request_result_string, sizeof(request_result_string), 0);
  sscanf(request_result_string, "%d `%[^`]", (int *)&booking.accepted, booking.comment);

  if(booking.accepted)
    strcpy(booking.comment, "Your seat request has successfully been booked!");
  else
    strcpy(booking.comment, "Sorry, there are no available seats!");

  return booking;
}
Request_result request_order(User *user, int meal_id, int num_of_portions, char *address){

  char query[512];
  int num_rows;
  char request_result_string[256];
  Request_result order;

  sprintf(query, "INSERT INTO Orders (User_id, Dish_id, Status, Address, No_of_portions) VALUES (%d, %d, 0, '%s', %d)", user->ID, meal_id, address, num_of_portions);
  send(server_socket, query, sizeof(query), 0);
  recv(server_socket, &num_rows, sizeof(int), 0);

  recv(server_socket, request_result_string, sizeof(request_result_string), 0);
  sscanf(request_result_string, "%d `%[^`]", (int *)&order.accepted, order.comment);
  
  if(order.accepted)
    strcpy(order.comment, "Your request for ordering has been arranged for consideration. Please, check your message box for further confirmations.");
  else
    strcpy(order.comment, "Sorry, your order request has been cancelled. Please try again!");

  return order;
}



int createSocket(){

  int sock = 0;
  struct sockaddr_in serv_addr;
  
  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    printf("\n Socket creation error \n");
    return -1;
  }

  memset(&serv_addr, '0', sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);
  // Convert IPv4 and IPv6 addresses from text to binary form
  if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
  {
    printf("\nInvalid address/ Address not supported \n");
    return -1;
  }

  if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
  {
    printf("\nConnection Failed \n");
    return -1;
  }

  return sock;
}