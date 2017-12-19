#include <gtk/gtk.h>
#include <glib.h>
#include <glib/gprintf.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
//***************************************SERVER*************************************************************
int createSocket();
int server_socket;

//***************************************/SERVER************************************************************
//***************************************STRUCTS*************************************************************
typedef struct{
  int ID;
  int userID;
  int tableID;
  char date[32]; //this will be changed
  int status;
} Book;
typedef struct{
  int ID;
  char name[255];
} Category;
typedef struct{
  int ID;
  char name[255];
  int price;
  int categoryID;
  int status;
} Dish;
typedef struct{
  int ID;
  char name[255];
  char login[255];
  char password[255];
  int role; //manager --> 0, waiter -->1
} Employee;
typedef struct{
  int ID;
  int userID;
  int dishID;
  int status;
  char address[255];
  int noOfPportions;
} Order;
typedef struct{
  int ID;
  int noOfPeople;
  int status;
} Table;
typedef struct{
  int ID;
  char name[255];
  char phone[13];
  char email[255];
  char password[255];
} User;
typedef struct{
  bool accepted;
  char comment[255];
} Request_result;
//****************************************GTK ENTRY STRUCTS***************************************************************
typedef struct
{
  GtkWidget *entry1;
  GtkWidget *entry2;
} log;
typedef struct 
{
  GtkWidget *entry1;
  GtkWidget *entry2;
  GtkWidget *entry3;
  int id;
} user;

typedef struct 
{
  GtkWidget *entry1;
  GtkWidget *entry2;
  GtkWidget *entry3;
  GtkWidget *entry4;
  GtkWidget *entry5;
  int id;
} order;

typedef struct 
{
  GtkWidget *entry1;
  GtkWidget *entry2;
  GtkWidget *entry3;
  GtkWidget *entry4;
  int id;
} book;

typedef struct 
{
  GtkWidget *entry1;
  GtkWidget *entry2;
  GtkWidget *entry3;
  GtkWidget *entry4;
  int id;
} menu;
//****************************************STRUCTS***************************************************************

GtkWidget *window; //global window
GtkWidget *grid; // global fixed container

User *manager= NULL;

log lg; 
user us;
menu mn;
order od;
book bk;


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

void login();
void submitLogin();

void mainWind();
void dashboard();
void messages();

void listUsers();
void listOrders();
void listBooks();
void listMenus();


void editUser(GtkWidget *widget, gpointer i);
void updateUser();
void deleteUser(GtkWidget *widget, gpointer i);

void editOrder(GtkWidget *widget, gpointer i);
void updateOrder();
void deleteOrder(GtkWidget *widget, gpointer i);

void editBook(GtkWidget *widget, gpointer i);
void updateBook();
void deleteBook(GtkWidget *widget, gpointer i);

void editMenu(GtkWidget *widget, gpointer i);
void updateMenu();
void deleteMenu(GtkWidget *widget, gpointer i);

//~~~~~~~~~~~~~~~~~~~~~~~~
Employee *checkForUserExistense(char* name, char *password);
User* getUserByID(int ID);
void saveUser(User *user);
//~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~
Dish* getDishByID(int ID);
void saveDish(Dish *dish, int type);//0 --> create, 1 --> update
Category* getCategoryByID(int ID);
//~~~~~~~~~~~~~~~~~~
Order* getOrderByID(int ID);
// -1 -->Rejected, 0 ---> Not checked, 1 ---->Approved, 2 ----> Delivered
void changeOrdStatus(Order *order, int status);
//**************************************BOOK MANAGEMENT******************************************************
Book *getBookByID(int ID);
Table* getTableByID(int ID);
void changeBkStatus(Book *book, int ID);
//*************************************/BOOK MANAGEMENT******************************************************


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
  
  server_socket = createSocket();
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
void login() {
  deleteChildren();

  GtkWidget *label1;
  GtkWidget *label2;
  GtkWidget *entry1;
  GtkWidget *entry2;
  GtkWidget *loginBut;
  GtkWidget *backBut;

  label1 = gtk_label_new("Login");
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

  lg.entry1 = entry1;
  lg.entry2 = entry2;

  g_signal_connect(backBut, "clicked", G_CALLBACK(mainWind), NULL);  
  g_signal_connect(loginBut, "clicked", G_CALLBACK(submitLogin), NULL);
  g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL); 

  gtk_widget_show_all(window);  
}
void submitLogin() {

  char* name= (char*)gtk_entry_get_text (GTK_ENTRY(lg.entry1));
  char* password = (char*) gtk_entry_get_text (GTK_ENTRY(lg.entry2));

  if(strcmp(name, "") == 0 || strcmp(password, "") == 0){
    show_info("All inputs are required! ");
    
  }else{
    Employee *manager = checkForUserExistense(name, password);
    if(manager != NULL){
      printf(ANSI_COLOR_GREEN "User found!\n" ANSI_COLOR_GREEN);
      dashboard();
    }else{
      printf(ANSI_COLOR_RED "Wrong email or password!\n" ANSI_COLOR_RESET);
      show_info("Wrong email or password!");  
    }
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
void listUsers(){
  deleteChildren();
 
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

  GtkWidget *password = gtk_label_new(NULL);
  gtk_label_set_justify(GTK_LABEL(password), GTK_JUSTIFY_LEFT);
  gtk_label_set_markup(GTK_LABEL(password), "<b>password</b>");
  gtk_grid_attach(GTK_GRID(grid), password, 4, 1, 1, 1);

  GtkWidget *backBut;
  backBut = gtk_button_new_with_label("Back");
  gtk_grid_attach(GTK_GRID(grid), backBut, 7, 1, 1, 1);
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  char query[512] = "SELECT * FROM Users";
  char result[1024];
  
  send(server_socket, query, sizeof(query), 0);
  
  int numOfRows;
  recv(server_socket, &numOfRows, sizeof(int), 0);
  User users[numOfRows];
  
  for(int i = 0; i < numOfRows; i++){
    recv(server_socket, result, sizeof(result), 0);
    sscanf(result, " '%d' '%[^']' '%[^']' '%[^']' '%[^']'", &(users[i].ID), users[i].name, (users[i].phone), users[i].email, users[i].password);
  }

  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

  for(int j = 0; j < numOfRows; j++)
  {
    gchar* index;
    index = g_strdup_printf("%d", users[j].ID);

    GtkWidget *number = gtk_label_new(index);
    gtk_grid_attach(GTK_GRID(grid), number, 0, j+2, 1, 1);

    GtkWidget *name = gtk_label_new(users[j].name);
    gtk_grid_attach(GTK_GRID(grid), name, 1, j+2, 1, 1);

    GtkWidget *phone = gtk_label_new(users[j].phone);
    gtk_grid_attach(GTK_GRID(grid), phone, 2, j+2, 1, 1);

    GtkWidget *email = gtk_label_new(users[j].email);
    gtk_grid_attach(GTK_GRID(grid), email, 3, j+2, 1, 1);

    GtkWidget *password = gtk_label_new(users[j].password);
    gtk_grid_attach(GTK_GRID(grid), password, 4, j+2, 1, 1);

    GtkWidget *edit = gtk_button_new_with_label("edit");
    gtk_grid_attach(GTK_GRID(grid), edit, 5, j+2, 1, 1);

    GtkWidget *delete = gtk_button_new_with_label("delete");
    gtk_grid_attach(GTK_GRID(grid), delete, 6, j+2, 1, 1);

      g_signal_connect(G_OBJECT(delete),"clicked", G_CALLBACK(deleteUser), ((gpointer) (glong) (users[j].ID)));
      g_signal_connect(G_OBJECT(edit),"clicked", G_CALLBACK(editUser), ((gpointer) (glong) (users[j].ID)));
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
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  User *user = getUserByID(index);
  gtk_entry_set_text(GTK_ENTRY(entry1), user->name);
  gtk_entry_set_width_chars (GTK_ENTRY(entry1), 60);
  gtk_entry_set_text(GTK_ENTRY(entry2), user->phone);
  gtk_entry_set_max_length (GTK_ENTRY(entry2),12);
  gtk_entry_set_width_chars (GTK_ENTRY(entry2), 60);
  gtk_entry_set_text(GTK_ENTRY(entry3), user->email);
  gtk_entry_set_width_chars (GTK_ENTRY(entry3), 60);
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

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


  us.entry1 = entry1;
  us.entry2 = entry2;
  us.entry3 = entry3;
  us.id = index;

  gtk_widget_show_all(window);

  g_signal_connect(update, "clicked", G_CALLBACK(updateUser), NULL);  
  g_signal_connect(backBut, "clicked", G_CALLBACK(listUsers), NULL);  
  g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
}
void updateUser(){
  User *user = malloc(sizeof(User));
  sprintf(user->name,"%s",gtk_entry_get_text (GTK_ENTRY(us.entry1)));
  sprintf(user->phone,"%s",gtk_entry_get_text (GTK_ENTRY(us.entry2)));
  sprintf(user->email,"%s",gtk_entry_get_text (GTK_ENTRY(us.entry3)));
  user->ID = us.id;
  saveUser(user);
  free(user);
  listUsers();  
}
void deleteUser(GtkWidget *widget, gpointer i){
  deleteChildren();
  int ID = ((gint) (glong) (i));

  int numOfRows;
  Request_result reg;
  char request_result_string[256];
  User *user = getUserByID(ID);
  if(user != NULL){
    char query[512];
    sprintf(query, "DELETE FROM Users WHERE ID = %d", user->ID);

    send(server_socket, query, sizeof(query), 0);
    recv(server_socket, &numOfRows, sizeof(int), 0);

    recv(server_socket, request_result_string, sizeof(request_result_string), 0);
    sscanf(request_result_string, "%d `%[^`]", (int *)&reg.accepted, reg.comment);

    if(reg.accepted){
      show_info("User deleted successfully");
      printf("User Successfully deleted\n");
    }else{
      show_info("Error occured during deletion");
      printf("Error occured during deletion\n");
    }
    free(user);
  }
 
  listUsers();
}

void listOrders(){
  deleteChildren();

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

  GtkWidget *numOfPortions = gtk_label_new(NULL);
  gtk_label_set_markup(GTK_LABEL(numOfPortions), "<b>№ portions</b>");
  gtk_grid_attach(GTK_GRID(grid), numOfPortions, 5, 1, 1, 1);

  GtkWidget *backBut;
  backBut = gtk_button_new_with_label("Back");
  gtk_grid_attach(GTK_GRID(grid), backBut, 7, 1, 1, 1);

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  char query[512] = "SELECT * FROM Orders";
  char result[1024];
  send(server_socket, query, sizeof(query), 0);
  int numOfRows;
  recv(server_socket, &numOfRows, sizeof(int), 0);
  if(numOfRows > 0){
    Order orders[numOfRows];
    for(int i = 0; i < numOfRows; i++){
      recv(server_socket, result, sizeof(result), 0);
      sscanf(result, " '%d' '%d' '%d' '%d' '%[^']' '%d'", &(orders[i].ID), &(orders[i].userID), &(orders[i].dishID), &(orders[i].status), orders[i].address, &(orders[i].noOfPportions));
    }
    for(int i = 0; i < numOfRows; i++){
      
      User *user = getUserByID(orders[i].userID);
      Dish *dish = getDishByID(orders[i].dishID);
      char status[32];
      if(orders[i].status == 0)
        strcpy(status, "Not checked");
      if(orders[i].status == -1)
        strcpy(status, "Rejected");
      if(orders[i].status == 1)
        strcpy(status, "Approved");
      if(orders[i].status == 2)
        strcpy(status, "Served");

      gchar* index;
      index = g_strdup_printf("%d", orders[i].ID);

      GtkWidget *number = gtk_label_new(index);
      gtk_grid_attach(GTK_GRID(grid), number, 0, i+2, 1, 1);

      GtkWidget *name = gtk_label_new(user->name);
      gtk_grid_attach(GTK_GRID(grid), name, 1, i+2, 1, 1);

      GtkWidget *phone = gtk_label_new(dish->name);
      gtk_grid_attach(GTK_GRID(grid), phone, 2, i+2, 1, 1);

      GtkWidget *email = gtk_label_new(status);
      gtk_grid_attach(GTK_GRID(grid), email, 3, i+2, 1, 1);

      GtkWidget *address = gtk_label_new(orders[i].address);
      gtk_grid_attach(GTK_GRID(grid), address, 4, i+2, 1, 1);

      index = g_strdup_printf("%d", orders[i].noOfPportions);
      GtkWidget *numOfPortions = gtk_label_new(index);
      gtk_grid_attach(GTK_GRID(grid), numOfPortions, 5, i+2, 1, 1);

      GtkWidget *edit = gtk_button_new_with_label("edit");
      gtk_grid_attach(GTK_GRID(grid), edit, 6, i+2, 1, 1);

      GtkWidget *delete = gtk_button_new_with_label("delete");
      gtk_grid_attach(GTK_GRID(grid), delete, 7, i+2, 1, 1);

      g_signal_connect(G_OBJECT(delete),"clicked", G_CALLBACK(deleteOrder), ((gpointer) (glong) (orders[i].ID)));
      g_signal_connect(G_OBJECT(edit),"clicked", G_CALLBACK(editOrder), ((gpointer) (glong) (orders[i].ID)));

      free(user);
      free(dish);
    }
  }
  
  g_signal_connect(G_OBJECT(backBut), "clicked", G_CALLBACK(dashboard), NULL);
  g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
  
  gtk_widget_show_all(window); 
}
void editOrder(GtkWidget *widget, gpointer i){
  deleteChildren();
  GtkWidget *label1;
  GtkWidget *label2;
  GtkWidget *label3;
  GtkWidget *label4;
  GtkWidget *label5;

  label1 = gtk_label_new("User:");
  label2 = gtk_label_new("Dish:");
  label3 = gtk_label_new("Status:");
  label4 = gtk_label_new("Address:");
  label5 = gtk_label_new("Number of portions:");
  GtkWidget *entry1;
  GtkWidget *entry2;
  GtkWidget *entry3;
  GtkWidget *entry4;
  GtkWidget *entry5;

  entry1 = gtk_entry_new();
  entry2 = gtk_entry_new();
  entry3 = gtk_combo_box_text_new();
  entry4 = gtk_entry_new();
  GtkAdjustment *adjustment = gtk_adjustment_new(0,0,10,1,0, 0);
  entry5 = gtk_spin_button_new(GTK_ADJUSTMENT(adjustment), 1, 0);


  gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (entry3), "not checked");
  gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (entry3), "rejected");
  gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (entry3), "approved");
  gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (entry3), "delivered");

  int ID = ((gint) (glong) (i));
  Order *order = getOrderByID(ID);
  User *user = getUserByID(order->userID);
  Dish *dish  = getDishByID(order->dishID);
  
  gtk_editable_set_editable(GTK_EDITABLE(entry1), FALSE);
  gtk_entry_set_width_chars (GTK_ENTRY(entry1), 60);
  gtk_entry_set_text(GTK_ENTRY(entry1), user->name);

  gtk_editable_set_editable(GTK_EDITABLE(entry2), FALSE);
  gtk_entry_set_width_chars (GTK_ENTRY(entry2), 60);
  gtk_entry_set_text(GTK_ENTRY(entry2), dish->name);

  /* Choose to set the first row as the active one by default, from the beginning */
  // -1 -->Rejected, 0 ---> Not checked, 1 ---->Approved, 2 ----> Delivered
  int status_default=0; 
  if(order->status == -1)
    status_default = 1;
  if(order->status == 1)
    status_default = 2;
  if(order->status == 2)
    status_default = 3;
  gtk_combo_box_set_active (GTK_COMBO_BOX (entry3), status_default);
  gtk_combo_box_set_wrap_width (GTK_COMBO_BOX(entry3), 60);

  gtk_editable_set_editable(GTK_EDITABLE(entry4), FALSE);
  gtk_entry_set_text(GTK_ENTRY(entry4), order->address);
  gtk_entry_set_width_chars (GTK_ENTRY(entry4), 60);

  gtk_editable_set_editable(GTK_EDITABLE(entry5), FALSE);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(entry5), order->noOfPportions);
  // gtk_entry_set_width_chars (GTK_ENTRY(entry5), 60);


  GtkWidget *backBut;
  backBut = gtk_button_new_with_label("back");
  GtkWidget *update;
  update = gtk_button_new_with_label("update");

  gtk_grid_attach(GTK_GRID(grid), label1, 1, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), label2, 1, 2, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), label3, 1, 3, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), label4, 1, 4, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), label5, 1, 5, 1, 1);

  gtk_grid_attach(GTK_GRID(grid), entry1, 2, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), entry2, 2, 2, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), entry3, 2, 3, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), entry4, 2, 4, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), entry5, 2, 5, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), backBut,3, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), update, 2, 6, 1, 1);

  od.entry1 = entry1;
  od.entry2 = entry2;
  od.entry3 = entry3;
  od.entry4 = entry4;
  od.entry5 = entry5;
  od.id = ID;

  g_signal_connect(update, "clicked", G_CALLBACK(updateOrder), NULL);  
  g_signal_connect(backBut, "clicked", G_CALLBACK(listOrders), NULL);  
  g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
  gtk_widget_show_all(window);

  free(dish);
  free(user);
  free(order);
}
void updateOrder(){

  // printf("Dish:%s\n", gtk_entry_get_text (GTK_ENTRY(od.entry2)));
  // printf("Dish:%s\n", gtk_entry_get_text (GTK_ENTRY(od.entry4)));
  int status_default = 0;
  int status = 0;
  status_default = (int)gtk_combo_box_get_active(GTK_COMBO_BOX(od.entry3));
  if(status_default == 1)
    status = -1;//-1 -->Rejected
  if(status_default == 2)
    status = 1;//1 ---->Approved
  if(status_default == 3)
    status = 2;//2 ---->Delivered

  Order *order = getOrderByID(od.id);
  changeOrdStatus(order, status);
  free(order);
  listOrders();
}
void deleteOrder(GtkWidget *widget, gpointer i){
  int ID = ((gint) (glong) (i));
  int numOfRows;
  Request_result reg;
  char request_result_string[256];
  char query[512];
  sprintf(query, "DELETE FROM Orders WHERE ID = %d", ID);
  send(server_socket, query, sizeof(query), 0);
  recv(server_socket, &numOfRows, sizeof(int), 0);
  recv(server_socket, request_result_string, sizeof(request_result_string), 0);
  sscanf(request_result_string, "%d `%[^`]", (int *)&reg.accepted, reg.comment);
  if(reg.accepted){
    show_info("Order deleted successfuly");
    printf("%s\n", reg.comment);
  }else{
    printf("%s\n", reg.comment);
  }

  listOrders();
}
void listBooks(){
  deleteChildren();

  gtk_grid_set_column_spacing(GTK_GRID(grid), 5);
  gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
  
  GtkWidget *number = gtk_label_new(NULL);  
  gtk_label_set_markup(GTK_LABEL(number), "<b>#</b>");
  gtk_grid_attach(GTK_GRID(grid), number, 0, 1, 1, 1);

  GtkWidget *name = gtk_label_new(NULL);  
  gtk_label_set_markup(GTK_LABEL(name), "<b>User</b>");
  gtk_grid_attach(GTK_GRID(grid), name, 1, 1, 1, 1);

  GtkWidget *table_label = gtk_label_new(NULL);
  gtk_label_set_markup(GTK_LABEL(table_label), "<b>Table</b>");
  gtk_grid_attach(GTK_GRID(grid), table_label, 2, 1, 1, 1);

  GtkWidget *time = gtk_label_new(NULL);
  gtk_label_set_markup(GTK_LABEL(time), "<b>Time</b>");
  gtk_grid_attach(GTK_GRID(grid), time, 3, 1, 1, 1);

  GtkWidget *stat = gtk_label_new(NULL);
  gtk_label_set_markup(GTK_LABEL(stat), "<b>Status</b>");
  gtk_grid_attach(GTK_GRID(grid), stat, 4, 1, 1, 1);

  GtkWidget *backBut;
  backBut = gtk_button_new_with_label("Back");
  gtk_grid_attach(GTK_GRID(grid), backBut, 6, 1, 1, 1);

  char query[512] = "SELECT * FROM Bookings";
  char result[1024];
  send(server_socket, query, sizeof(query), 0);
  int numOfRows;
  recv(server_socket, &numOfRows, sizeof(int), 0);
  if(numOfRows > 0){
    Book books[numOfRows];
    for(int i = 0; i < numOfRows; i++){
      recv(server_socket, result, sizeof(result), 0);
      sscanf(result, " '%d' '%d' '%d' '%[^']' '%d'", &(books[i].ID), &(books[i].userID), &(books[i].tableID), books[i].date, &(books[i].status));
    }
    for(int i = 0; i < numOfRows; i++){
      User *user = getUserByID(books[i].userID);
      Table *table = getTableByID(books[i].tableID);
      char status[32];
      if(books[i].status == 0)
      strcpy(status, "Not checked");
      if(books[i].status == -1)
      strcpy(status, "Rejected");
      if(books[i].status == 1)
      strcpy(status, "Approved");
      if(books[i].status == 2)
      strcpy(status, "Served");

      gchar* index;
      index = g_strdup_printf("%d", books[i].ID);

      GtkWidget *number = gtk_label_new(index);
      gtk_grid_attach(GTK_GRID(grid), number, 0, i+2, 1, 1);

      GtkWidget *name = gtk_label_new(user->name);
      gtk_grid_attach(GTK_GRID(grid), name, 1, i+2, 1, 1);
      
      index = g_strdup_printf("%d", table->ID);
      GtkWidget *table_label = gtk_label_new(index);
      gtk_grid_attach(GTK_GRID(grid), table_label, 2, i+2, 1, 1);

      GtkWidget *time = gtk_label_new(books[i].date);
      gtk_grid_attach(GTK_GRID(grid), time, 3, i+2, 1, 1);

      GtkWidget *stat = gtk_label_new(status);
      gtk_grid_attach(GTK_GRID(grid), stat, 4, i+2, 1, 1);

      GtkWidget *edit = gtk_button_new_with_label("edit");
      gtk_grid_attach(GTK_GRID(grid), edit, 5, i+2, 1, 1);

      GtkWidget *delete = gtk_button_new_with_label("delete");
      gtk_grid_attach(GTK_GRID(grid), delete, 6, i+2, 1, 1);

      g_signal_connect(G_OBJECT(delete),"clicked", G_CALLBACK(deleteBook), ((gpointer) (glong) (books[i].ID)));
      g_signal_connect(G_OBJECT(edit),"clicked", G_CALLBACK(editBook), ((gpointer) (glong) (books[i].ID)));
      
      free(table);
      free(user);
    }
  }

  g_signal_connect(G_OBJECT(backBut), "clicked", G_CALLBACK(dashboard), NULL);
  g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
  
  gtk_widget_show_all(window); 
}
void editBook(GtkWidget *widget, gpointer i){
  deleteChildren();
  
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
  entry2 = gtk_entry_new();
  entry3 = gtk_entry_new();
  entry4 = gtk_combo_box_text_new();
  
  int ID = ((gint) (glong) (i));
  Book *book = getBookByID(ID);
  User *user = getUserByID(book->userID);

  gtk_editable_set_editable(GTK_EDITABLE(entry1), FALSE);
  gtk_entry_set_width_chars (GTK_ENTRY(entry1), 60);
  gtk_entry_set_text(GTK_ENTRY(entry1), user->name);
  gtk_editable_set_editable(GTK_EDITABLE(entry2), FALSE);

  gtk_entry_set_width_chars (GTK_ENTRY(entry2), 60);
  gtk_entry_set_text(GTK_ENTRY(entry2), g_strdup_printf("%d", book->tableID));

  gtk_editable_set_editable(GTK_EDITABLE(entry3), FALSE);
  gtk_entry_set_width_chars (GTK_ENTRY(entry3), 60);
  gtk_entry_set_text(GTK_ENTRY(entry3), book->date);

  gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (entry4), "not checked");
  gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (entry4), "rejected");
  gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (entry4), "approved");
  gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (entry4), "served");

  /* Choose to set the first row as the active one by default, from the beginning */
  // -1 -->Rejected, 0 ---> Not checked, 1 ---->Approved, 2 ----> Delivered
  int status_default=0; 
  if(book->status == -1)
    status_default = 1;
  if(book->status == 1)
    status_default = 2;
  if(book->status == 2)
    status_default = 3;
  gtk_combo_box_set_active (GTK_COMBO_BOX (entry4), status_default);
  gtk_combo_box_set_wrap_width (GTK_COMBO_BOX(entry4), 60);

  free(user);
  free(book);
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

  bk.entry1 = entry1;
  bk.entry2 = entry2;
  bk.entry3 = entry3;
  bk.entry4 = entry4;
  bk.id = ID;

  gtk_widget_show_all(window);

  g_signal_connect(update, "clicked", G_CALLBACK(updateBook), NULL);  
  g_signal_connect(backBut, "clicked", G_CALLBACK(listBooks), NULL);  
  g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
 
}
void updateBook(){
  // printf("Dish:%s\n", gtk_entry_get_text (GTK_ENTRY(od.entry2)));
  // printf("Dish:%s\n", gtk_entry_get_text (GTK_ENTRY(od.entry4)));
  int status_default = 0;
  int status = 0;
  status_default = (int)gtk_combo_box_get_active(GTK_COMBO_BOX(bk.entry4));
  if(status_default == 1)
    status = -1;//-1 -->Rejected
  if(status_default == 2)
    status = 1;//1 ---->Approved
  if(status_default == 3)
    status = 2;//2 ---->Delivered

  Book *book = getBookByID(bk.id);
  changeBkStatus(book, status);
  free(book);
  listBooks();
}
void deleteBook(GtkWidget *widget, gpointer i){
  int ID = ((gint) (glong) (i));
  Book *book = getBookByID(ID);
  int numOfRows;
  Request_result reg;
  char request_result_string[256];
  char query[512];
  sprintf(query, "DELETE FROM Bookings WHERE ID = %d", book->ID);
  send(server_socket, query, sizeof(query), 0);
  recv(server_socket, &numOfRows, sizeof(int), 0);
  recv(server_socket, request_result_string, sizeof(request_result_string), 0);
  sscanf(request_result_string, "%d `%[^`]", (int *)&reg.accepted, reg.comment);

  free(book);
  show_info("Book deleted successfuly");
  listBooks();
}
void listMenus(){
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
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  char query[512] = "SELECT * FROM Dishes";
  char result[1024];
  send(server_socket, query, sizeof(query), 0);
  int numOfRows;
  recv(server_socket, &numOfRows, sizeof(int), 0);
  if(numOfRows > 0){
    Dish dishes[numOfRows];
    for(int i = 0; i < numOfRows; i++){
      recv(server_socket, result, sizeof(result), 0);
      sscanf(result, " '%d' '%[^']' '%d' '%d' '%d'", &(dishes[i].ID), dishes[i].name, &(dishes[i].price), &(dishes[i].categoryID), &(dishes[i].status));
    }
    for(int i = 0; i < numOfRows; i++){
      Category *category = getCategoryByID(dishes[i].categoryID);

      gchar* index;
      index = g_strdup_printf("%d", dishes[i].ID);

      GtkWidget *number = gtk_label_new(index);
      gtk_grid_attach(GTK_GRID(grid), number, 0, i+2, 1, 1);

      GtkWidget *name = gtk_label_new(dishes[i].name);
      gtk_grid_attach(GTK_GRID(grid), name, 1, i+2, 1, 1);

      index = g_strdup_printf("%d", dishes[i].price);
      GtkWidget *price = gtk_label_new(index);
      gtk_grid_attach(GTK_GRID(grid), price, 2, i+2, 1, 1);

      GtkWidget *categ = gtk_label_new(category->name);
      gtk_grid_attach(GTK_GRID(grid), categ, 3, i+2, 1, 1);

      char status[32];//status of the order
      if(dishes[i].status == 0)
        strcpy(status, "Not available");
      if(dishes[i].status == 1)
        strcpy(status, "Available");  
      GtkWidget *stat = gtk_label_new(status);
      gtk_grid_attach(GTK_GRID(grid), stat, 4, i+2, 1, 1);

      GtkWidget *edit = gtk_button_new_with_label("edit");
      gtk_grid_attach(GTK_GRID(grid), edit, 5, i+2, 1, 1);

      GtkWidget *delete = gtk_button_new_with_label("delete");  
      gtk_grid_attach(GTK_GRID(grid), delete, 6, i+2, 1, 1);

      g_signal_connect(G_OBJECT(delete),"clicked", G_CALLBACK(deleteMenu), ((gpointer) (glong) (dishes[i].ID)));
      g_signal_connect(G_OBJECT(edit),"clicked", G_CALLBACK(editMenu), ((gpointer) (glong) (dishes[i].ID)));
      free(category);
    }
  }
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  g_signal_connect(G_OBJECT(backBut), "clicked", G_CALLBACK(dashboard), NULL);
  g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
  
  gtk_widget_show_all(window); 
}
void editMenu(GtkWidget *widget, gpointer i){
  deleteChildren();
  GtkWidget *label1;
  GtkWidget *label2;
  GtkWidget *label3;
  GtkWidget *label4;

  label1 = gtk_label_new("Dish:");
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

  gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (entry3), "First meal");
  gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (entry3), "Second meal");
  gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (entry3), "Drinks");
  gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (entry3), "Salad");


  gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (entry4), "Not available");
  gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (entry4), "Available");

  int ID = ((gint) (glong) (i));
  Dish *dish = getDishByID(ID);
  Category *category = getCategoryByID(dish->categoryID);

  gtk_entry_set_width_chars (GTK_ENTRY(entry1), 60);
  gtk_entry_set_text(GTK_ENTRY(entry1), dish->name);

  gtk_entry_set_width_chars (GTK_ENTRY(entry2), 60);
  gtk_spin_button_set_value (GTK_SPIN_BUTTON(entry2), dish->price);

  /* Choose to set the first row as the active one by default, from the beginning */
  gtk_combo_box_set_wrap_width (GTK_COMBO_BOX(entry3), 60);
  gtk_combo_box_set_active (GTK_COMBO_BOX (entry3), dish->categoryID-1);

    /* Choose to set the first row as the active one by default, from the beginning */
  gtk_combo_box_set_wrap_width (GTK_COMBO_BOX(entry4), 60);
  gtk_combo_box_set_active (GTK_COMBO_BOX (entry4), dish->status);

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

 
  mn.entry1 = entry1;
  mn.entry2 = entry2;
  mn.entry3 = entry3;
  mn.entry4 = entry4;
  mn.id = ID;

  gtk_widget_show_all(window);

  g_signal_connect(update, "clicked", G_CALLBACK(updateMenu), NULL);  
  g_signal_connect(backBut, "clicked", G_CALLBACK(listMenus), NULL);  
  g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
  
}
void updateMenu(){
  Dish *dish = malloc(sizeof(Dish));  
  sprintf(dish->name,"%s",gtk_entry_get_text(GTK_ENTRY(mn.entry1)));
  dish->price =gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(mn.entry2));
  dish->categoryID = gtk_combo_box_get_active(GTK_COMBO_BOX(mn.entry3))+1;
  dish->status =gtk_combo_box_get_active(GTK_COMBO_BOX(mn.entry4));
  dish->ID = mn.id;
  saveDish(dish, 1);
  free(dish);
 
  listMenus();
}
void deleteMenu(GtkWidget *widget, gpointer i){
  int ID = ((gint) (glong) (i));
  Dish *dish = getDishByID(ID);
  if(dish != NULL){
    int numOfRows;
    Request_result reg;
    char request_result_string[256];
    char query[512];
    sprintf(query, "DELETE FROM Dishes WHERE ID = %d", dish->ID);
    send(server_socket, query, sizeof(query), 0);
    recv(server_socket, &numOfRows, sizeof(int), 0);
    recv(server_socket, request_result_string, sizeof(request_result_string), 0);
    sscanf(request_result_string, "%d `%[^`]", (int *)&reg.accepted, reg.comment);
    if(reg.accepted){
      show_info("Dish Successfully deleted");
      printf("Dish Successfully deleted\n");
    }else{
      show_info(reg.comment);
      printf("%s\n", reg.comment);
    }
  }
  free(dish);
  listMenus();
}


Employee *checkForUserExistense(char* name, char *password){
  Employee *manager = NULL;
  char query[512];
  char temp[32];
  char result[1024];
  int numOfRows;

  sprintf(query, "SELECT * FROM Employees WHERE login = '%s' AND password = '%s'", name, password);
  send(server_socket, query, sizeof(query), 0);
  recv(server_socket, &numOfRows, sizeof(int), 0);
  if(numOfRows > 0){
    manager = malloc(sizeof(Employee));
    recv(server_socket, result, sizeof(result), 0);
    sscanf(result, " '%d' '%[^']' '%[^']' '%[^']' '%d'", &(manager->ID), manager->name, manager->login, manager->password, &(manager->role));
  }
  return manager;
}
User* getUserByID(int ID){
  User *user = NULL;
  char query[512];
  char result[1024];
  sprintf(query, "SELECT * FROM Users WHERE ID = %d", ID);
  int count;
  send(server_socket, query, sizeof(query), 0);
  recv(server_socket, &count, sizeof(int), 0);
  if(count == 1){
    user = malloc(sizeof(User));
    recv(server_socket, result, sizeof(result), 0);
    sscanf(result, " '%d' '%[^']' '%[^']' '%[^']' '%[^']'", &(user->ID), user->name, user->phone, user->email, user->password);
  }
  return user;
}
Dish *getDishByID(int ID){
  char query[512];
  char result[1024];
  Dish *dish = NULL;
  sprintf(query, "SELECT * FROM Dishes WHERE ID = %d", ID);
  int count;
  send(server_socket, query, sizeof(query), 0);
  recv(server_socket, &count, sizeof(int), 0);
  if(count == 1){
    dish = malloc(sizeof(Dish));
    recv(server_socket, result, sizeof(result), 0);
    sscanf(result, " '%d' '%[^']' '%d' '%d' '%d'", &(dish->ID), dish->name, &(dish->price), &(dish->categoryID), &(dish->status));
  }
  return dish;
}
Category *getCategoryByID(int ID){
  char query[512];
  char result[1024];
  Category *category = NULL;
  sprintf(query, "SELECT * FROM Categories WHERE ID = %d", ID);
  int count;
  send(server_socket, query, sizeof(query), 0);
  recv(server_socket, &count, sizeof(int), 0);
  if(count == 1){
    category = malloc(sizeof(Category));
    recv(server_socket, result, sizeof(result), 0);
    sscanf(result, " '%d' '%[^']'", &(category->ID), category->name);
  }
  return category;
}
Order* getOrderByID(int ID){
  char query[512];
  char result[1024];
  Order *order = NULL;
  sprintf(query, "SELECT * FROM Orders WHERE ID = %d", ID);
  int count;
  send(server_socket, query, sizeof(query), 0);
  recv(server_socket, &count, sizeof(int), 0);
  if(count == 1){
     order = malloc(sizeof(Order));
    recv(server_socket, result, sizeof(result), 0);
    sscanf(result, " '%d' '%d' '%d' '%d' '%[^']' '%d'", &(order->ID), &(order->userID), &(order->dishID), &(order->status), order->address, &(order->noOfPportions));
  }
  return order;
}
Book *getBookByID(int ID){
  char query[512];
  char result[1024];
  Book *book = NULL;
  sprintf(query, "SELECT * FROM Bookings WHERE ID = %d", ID);
  int count;
  send(server_socket, query, sizeof(query), 0);
  recv(server_socket, &count, sizeof(int), 0);
  if(count == 1){
    book = malloc(sizeof(Book));
    recv(server_socket, result, sizeof(result), 0);
    sscanf(result, " '%d' '%d' '%d' '%[^']' '%d'", &(book->ID), &(book->userID), &(book->tableID), book->date, &(book->status));
  }
  return book;
}
Table *getTableByID(int ID){
  char query[512];
  char result[1024];
  Table *table = NULL;
  sprintf(query, "SELECT * FROM Tables WHERE ID = %d", ID);
  int count;
  send(server_socket, query, sizeof(query), 0);
  recv(server_socket, &count, sizeof(int), 0);
  if(count == 1){
    table = malloc(sizeof(Table));
    recv(server_socket, result, sizeof(result), 0);
    sscanf(result, " '%d' '%d' '%d'", &(table->ID), &(table->noOfPeople), &(table->status));
  }
  return table;
}
void changeBkStatus(Book *book, int status){
  Request_result reg;
  char request_result_string[256];
  char query[512];
  int numOfRows;
  sprintf(query, "UPDATE Bookings SET Status = %d WHERE ID = %d", status, book->ID);
  send(server_socket, query, sizeof(query), 0);
  recv(server_socket, &numOfRows, sizeof(int), 0);

  recv(server_socket, request_result_string, sizeof(request_result_string), 0);
  sscanf(request_result_string, "%d `%[^`]", (int *)&reg.accepted, reg.comment);
  if(reg.accepted){
    printf("Status Successfully changed\n");
    show_info("Status Successfully changed");
  }
  else{
    printf("%s",reg.comment);
    show_info(reg.comment);
  }
}
void saveUser(User *user){
  Request_result reg;
  char request_result_string[256];
  char query[512];
  int numOfRows;
  sprintf(query, "UPDATE Users SET Name = '%s', Phone = %s, Email = '%s' WHERE ID = %d", user->name, user->phone, user->email, user->ID);
  send(server_socket, query, sizeof(query), 0);
  recv(server_socket, &numOfRows, sizeof(int), 0);

  recv(server_socket, request_result_string, sizeof(request_result_string), 0);
  sscanf(request_result_string, "%d `%[^`]", (int *)&reg.accepted, reg.comment);
  if(reg.accepted){
    show_info("User updated successfully");
    printf("%s", reg.comment);
  }else{
    show_info(reg.comment);
    printf("%s", reg.comment);
  }
}

//0 --> create, 1 --> update
void saveDish(Dish *dish, int type){
  Request_result reg;
  char request_result_string[256];
  char query[512];
  if(type == 0){
    sprintf(query, "INSERT INTO Dishes(Name, Price, Category_id, Status) VALUES ('%s', %d, %d, %d)", dish->name, dish->price, dish->categoryID, dish->status);
  }else{
    sprintf(query, "UPDATE Dishes SET Name = '%s', Price = %d, Category_id = %d, Status = %d WHERE ID = %d", dish->name, dish->price, dish->categoryID, dish->status, dish->ID);
  }
  send(server_socket, query, strlen(query), 0);
  int numOfRows;
  recv(server_socket, &numOfRows, sizeof(int), 0);
  recv(server_socket, request_result_string, sizeof(request_result_string), 0);
  sscanf(request_result_string, "%d `%[^`]", (int *)&reg.accepted, reg.comment);
  if(reg.accepted){
    show_info("Dish saved Successfully");
    printf("Dish saved Successfully\n");
  }else{
    show_info(reg.comment);
    printf("%s",reg.comment);
  }
}

void changeOrdStatus(Order *order, int status){
  Request_result reg;
  char request_result_string[256];
  char query[512];
  int numOfRows;
  sprintf(query, "UPDATE Orders SET Status = %d WHERE ID = %d", status, order->ID);
  send(server_socket, query, sizeof(query), 0);
  recv(server_socket, &numOfRows, sizeof(int), 0);

  recv(server_socket, request_result_string, sizeof(request_result_string), 0);
  sscanf(request_result_string, "%d `%[^`]", (int *)&reg.accepted, reg.comment);
  if(reg.accepted){
    show_info("Order status Successfully changed");
    printf("Order status Successfully changed\n");
  }
  else{
    printf("%s",reg.comment);
    show_info(reg.comment);
  }
}


//***************************************SERVER************************************************************
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
//***************************************/SERVER***********************************************************
