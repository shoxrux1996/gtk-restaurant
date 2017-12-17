#include <mysql/mysql.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <stdbool.h>

MYSQL *conn;
MYSQL_RES *res;
MYSQL_ROW row;
MYSQL_FIELD *fields;

typedef struct{
  char name[255];
  char phone[9];
  char email[255];
  char password[255];
} User;

typedef struct{
  bool accepted;
  char comment[255]; 
} Request_result;

User* authenticate(char *email, char *password);
void connectToDatabase();
void closeDatabaseConnection();
MYSQL_RES* executeQuery(char* query);
MYSQL_RES* getDishes(char *res);
MYSQL_RES* getUser(char *, char *);
void sendDishes(MYSQL_RES* res, int socket);

void createSocket();
void* connection_handler(void *);



int main()
{
  connectToDatabase();
  //executeQuery("INSERT INT Dishes(Name, Price, Category_id, Status) Values('French Salad', 4000, 4, 1)");
  createSocket();
  

  //res = getDishes(NULL);

  //sendDishes(res, new_socket);

  /*while((row = mysql_fetch_row(res)) != NULL)
  {
    for(int i=0; i<mysql_num_fields(res); i++)
      printf("%-15s", row[i]);

    printf("\n");
    
  }*/
  

  /*
  int valread;
  char ser[1024];
  char cli[1024];
  //send(new_socket, r, 1024, 0);

  int pid = fork();

  if(pid == 0)
  {
    while(1)
    {

    memset(ser, 0, sizeof(ser));
    printf("Server : ");
    fgets (ser, sizeof(ser), stdin);
    send(new_socket , ser , strlen(ser) , 0 );
    ser[strlen(ser)] = '\0';
    
  }
  }else{
    while(1){
      memset(cli, 0, sizeof(cli));
      valread = read( new_socket , cli, 1024);
      printf("Client: %s\n",cli );
    }
  }*/


  //executeQuery("SELECT d.ID, d.Name, d.Price, c.Name  FROM Dishes as d JOIN Categories as c ON d.Category_id=c.ID");
	
  
  /*printf("TABLES:\n");

  fields = mysql_fetch_fields(res);
  for(int i=0; i<mysql_num_fields(res); i++)
  {
    printf("%-15s", fields[i].name);
  }
  printf("\n");
  while((row = mysql_fetch_row(res)) != NULL)
  {
    for(int i=0; i<mysql_num_fields(res); i++)
      printf("%-15s", row[i]);

    printf("\n");
    
  }*/

  mysql_free_result(res);
  closeDatabaseConnection();

  return 0;
}

/*void sendDishes(MYSQL_RES *res, int socket){
  char dish_row[1024];
  int num_rows = mysql_num_rows(res);

  printf("Rows: %d\n", num_rows);
  send(socket, &num_rows, sizeof(int), 0);

  while((row = mysql_fetch_row(res)) != NULL)
  {
    //int num_fields = mysql_num_fields(res);
    memset(dish_row, 0, sizeof(dish_row));
    sprintf(dish_row, "%s  %s  %s  %s",row[0], row[1], row[2], row[3]);

    send(socket, dish_row, sizeof(dish_row), 0);
    
  }
}

MYSQL_RES* getDishes(char* category){
  MYSQL_RES * res;
  char query[255];

  if(category)
    sprintf(query, "SELECT d.ID, d.Name, d.Price, c.Name  FROM Dishes as d JOIN Categories as c ON d.Category_id=c.ID WHERE d.status = 1 AND c.name = '%s'", category);
  else
    sprintf(query, "SELECT d.ID, d.Name, d.Price, c.Name  FROM Dishes as d JOIN Categories as c ON d.Category_id=c.ID WHERE d.status = 1");

  res = executeQuery(query);

  return res;
}*/

void connectToDatabase(){    
  char *server = "127.0.0.1";
  char *user = "root";
  char *password = "8566";
  char *database = "os_project";

  conn = mysql_init(NULL);
  if(!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0))
  {
    fprintf(stderr, "%s\n", mysql_error(conn));
    exit(1);
  }
}


void closeDatabaseConnection(){
  mysql_close(conn);
}

MYSQL_RES* executeQuery(char* query){
  if(mysql_query(conn, query))
  {
    fprintf(stderr, "%s\n", mysql_error(conn));
    return NULL;
  }

 return mysql_store_result(conn);
 
}


void createSocket(){
  int server_fd, new_socket, valread;
  struct sockaddr_in address;
  int opt = 1;
  int addrlen = sizeof(address);

  printf("CREATING SOCKET ......\n");
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
  {
    perror("Socket creation failed!");
    exit(EXIT_FAILURE);
  }
  printf("DEFINING SERVER SOCKET OPTIONS, FAMILY, ADDRESS & PORT .....\n");
  // Forcefully attaching socket to the port 8080
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
  {
    perror("Socket setsockopt failed");
    exit(EXIT_FAILURE);
  }
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons( 8080 );

  //inet_pton(AF_INET, "192.168.213.109", &address.sin_addr);

  printf("BINDING SERVER SOCKET TO PORT 8080 .....\n");
  // Forcefully attaching socket to the port 8080
  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
  {
    perror("Socket bind failed");
    exit(EXIT_FAILURE);
  }
  printf("SERVER LISTENING ON PORT 8080 FOR NEW CONNECTION.....\n");
  if (listen(server_fd, 3) < 0)
  {
    perror("Socket listen failed");
    exit(EXIT_FAILURE);
  }

  pthread_t thread_id;

  while(new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))
  {
    printf("Connection accepted.\n");

    if(pthread_create(&thread_id, NULL, connection_handler, (void *) &new_socket)){
      perror("Error creating thread");
    }

  }

  if(new_socket < 0)
  {
    perror("Socket accept failed");
    exit(EXIT_FAILURE);
  }
  
}

void *connection_handler(void *socket){

  int new_socket = *(int *)socket;
  printf("Client %d is connected\n", new_socket);
  char query[512];
  char row_string[1024];
  char query_type[10];
  int num_fields;
  int num_rows;
  Request_result req_result;
  char request_res_string[256];

  while(recv(new_socket, query, sizeof(query), 0)){
    sscanf(query, "%s", query_type);
    printf("Query type is: |%s|\n", query_type);
    printf("Query is: |%s|\n", query);
    res = executeQuery(query);
    num_rows = mysql_affected_rows(conn);
    printf("Number of affected rows: %d, client: %d\n", num_rows, new_socket);
    send(new_socket, &num_rows, sizeof(int), 0);
    if(strcmp(query_type, "SELECT") == 0)
    {
      num_fields = mysql_num_fields(res);
    
    

      while(row = mysql_fetch_row(res)){

        for (int i = 0; i < num_fields; ++i)
        {
          sprintf(row_string, "%s '%s'", row_string, row[i]);
        }

        send(new_socket, row_string, sizeof(row_string), 0);
      }
    }else{
      if(res == NULL){
        req_result.accepted = false;
        strcpy(req_result.comment, mysql_error(conn));
        printf("Comment: %s\n", req_result.comment);
      }else{
        req_result.accepted = true;
        strcpy(req_result.comment, "Successful!");
      }

      sprintf(request_res_string, "%d `%s`", req_result.accepted, req_result.comment);
      send(new_socket, request_res_string, sizeof(request_res_string), 0);
    }

      
    
    
  }
  
  return 0;
}

