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

int main(int argc, char *argv[]) {

  server_socket = createSocket();
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
  
  struct log en; 
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
  char *email= (char*)gtk_entry_get_text (GTK_ENTRY(d->entry1));
  char *password = (char*)gtk_entry_get_text (GTK_ENTRY(d->entry2));

  printf("email: %s\n", email);
  printf("Password: %s\n", password);
  User *user = authenticate(email, password);


  if(user != NULL)
  {
    printf(ANSI_COLOR_GREEN "User found!\n" ANSI_COLOR_RESET);
    sleep(1);

    dashboard_menu(user);
   

  } 
  else
  {
    printf(ANSI_COLOR_RED "Wrong email or password!\n" ANSI_COLOR_RESET);
    sleep(2);
    show_info("Invalid credantials");
    login(widget);
  }
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
  char *name =(char *) gtk_entry_get_text (GTK_ENTRY(d->entry1));
  char *phone =(char *) gtk_entry_get_text (GTK_ENTRY(d->entry2));
  char *email =(char *) gtk_entry_get_text (GTK_ENTRY(d->entry3));
  char *password =(char *) gtk_entry_get_text (GTK_ENTRY(d->entry4));
  printf("%d\n", (int)strlen(name));
  if(strcmp(name, "") == 0 || strcmp(email, "") == 0 || strcmp(password, "") == 0 || strcmp(phone, "") == 0){
      show_info("All inputs are required! ");
      registe(widget);
  }

  Request_result reg;
  reg = request_register(name, phone, email, password);
  if(reg.accepted)
    printf( ANSI_COLOR_GREEN "%s\n" ANSI_COLOR_RESET, reg.comment);
  else
    printf( ANSI_COLOR_RED "%s\n" ANSI_COLOR_RESET, reg.comment);
    
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
  orderFood(widget);
  
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

void dashboard_menu(User *user){
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
  dashboard();
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