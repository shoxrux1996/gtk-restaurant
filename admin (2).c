//Client side C program to demostrate Socket programming
#include<stdio.h>
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
	char phone[32];
	char email[255];
	char password[255];
} User;
typedef struct{
	int ID;
	int userID;
	char text[255];
	int status;
} Message;
typedef struct{
	bool accepted;
	char comment[255];
} Request_result;
//*************************************//STRUCTS*************************************************************

//**************************************USER MANAGEMENT******************************************************
int createUser(); //Creating a user
int createUserRecord(User user); //Building a create query for user table
Employee *checkForUserExistense(char* name, char *password);
int login(); //Login
void reg(); //Registration
void listUsers(); //Listing all users
void modifyUser(); //Modify user
User* getUserByID(int ID); //Get user by ID
void saveUser(User *user);
//**************************************/USER MANAGEMENT*****************************************************

//**************************************ORDER MANAGEMENT*****************************************************

void listOrders(); //Listing orders
void changeOrderStatus(); //Changing order status
Dish* getDishByID(int ID);
// -1 -->Rejected, 0 ---> Not checked, 1 ---->Approved, 2 ----> Delivered
void changeOrdStatus(Order *order, int status);
//*************************************/ORDER MANAGEMENT*****************************************************

//**************************************BOOK MANAGEMENT******************************************************
void listBooks();
Book *getBookByID(int ID);
Table* getTableByID(int ID);
void changeBkStatus(Book *book, int ID);
//*************************************/BOOK MANAGEMENT******************************************************

//***************************************MENU MANAGEMENT******************************************************
void listMenu();
Category* getCategoryByID(int ID);
void saveDish(Dish *dish, int type);
//**************************************/MENU MANAGEMENT******************************************************

//***************************************WAITER MANAGEMENT****************************************************
void listWaiters();
void addWaiter();
void saveWaiter(Employee *employee, int type);
Employee *getWaiterByID(int ID);
//***************************************/WAITER MANAGEMENT***************************************************

//*************************************************MESSAGES***************************************************
void sendMessage();
void saveMessage(Message *message);
void listMessage();
void printMessages(User *user);
void displayMessageMenu();
//************************************************/MESSAGES***************************************************

//****************************************MENUS**************************************************************
void displayGreetingMenu(); //Display first menu
void greetingFunction(); //Greeting function
void loginMenu();
void exitApp(); //Exit from applications
void attemptAgain(); //Attemp when not chosen option listed above
void displayManagerFunctionsMenu(); //Display menu 2 {after logging in}
void displayMenuUser(); //Display user management 
void displayMenuOrder(); //Display order management
void displayMenuBook(); //Display book management
void displayMenuMenu(); //Display menu management
void displayMenuWaiter(); //Display waiter management
void afterLoggingInMenu(); //After logging in
//***************************************/MENUS**************************************************************

//***************************************ADDITIONAL FUNCTIONS ***********************************************
void clrscr(); //Clear screen
//***************************************/ADDITIONAL FUNCTIONS **********************************************

int main(int argc, char const *argv[])
{
	server_socket = createSocket();
	greetingFunction();
	return 0;
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

//**************************************USER MANAGEMENT*****************************************************

int createUserRecord(User user){
	Request_result reg;
	char request_result_string[256];
	char *query = malloc(1024);
	sprintf(query, "INSERT INTO Users(name, phone, email, password) VALUES ('%s', '%s', '%s', '%s')", user.name, user.phone, user.email, user.password);
	send(server_socket, query, strlen(query), 0);
	int numOfRows;
	recv(server_socket, &numOfRows, sizeof(int), 0);
	recv(server_socket, request_result_string, sizeof(request_result_string), 0);
	sscanf(request_result_string, "%d `%[^`]", (int *)&reg.accepted, reg.comment);
	free(query);
	return numOfRows;
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

int login(){
	char email[64], password[32];
	printf("%s\n", "Login page");
	printf("%s", "Enter your email: ");
	scanf("%s", email);
	printf("%s", "Enter your password: ");
	scanf("%s", password);
	clrscr();
	Employee *manager = checkForUserExistense(email, password);
	if(manager != NULL){
		free(manager);
		return 1;
	}else{
		printf("%s\n", "Incorrect username or password");
		return 0;
	}
}

void reg(){
	clrscr();
	User user;
	printf("%s\n", "Registering page");
	printf("%s", "Enter your name: ");
	scanf("%s", user.name);
	printf("%s", "Enter your password: ");
	scanf("%s", user.password);
	printf("%s", "Enter your email: ");
	scanf("%s", user.email);
	printf("%s", "Enter your phone number: ");
	scanf("%s", user.phone);
	clrscr();
	if(createUserRecord(user) == 1){
		printf("%s\n", "User Successfully created");
	}else{
		printf("%s\n", "Error occured during creation of user");
	}
}

void listUsers(){
	clrscr();
	char query[512] = "SELECT * FROM Users";
	char result[1024];
	send(server_socket, query, sizeof(query), 0);
	int numOfRows;
	recv(server_socket, &numOfRows, sizeof(int), 0);
	if(numOfRows > 0){
		User users[numOfRows];
		for(int i = 0; i < numOfRows; i++){
			recv(server_socket, result, sizeof(result), 0);
			sscanf(result, " '%d' '%[^']' '%[^']' '%[^']' '%[^']'", &(users[i].ID), users[i].name, users[i].phone, users[i].email, users[i].password);
		}
		printf("ID\tName\tPhone\tEmail\tPassword\n");
		for(int i = 0; i < numOfRows; i++)
			printf("%d\t%s\t%s\t%s\t%s\n", users[i].ID, users[i].name, users[i].phone, users[i].email, users[i].password);
		printf("\n");
	}else{
		printf("No users found\n");
	}
}

void modifyUser(){
	clrscr();
	listUsers();
	int ID;
	printf("Please enter user's ID: ");
	scanf("%d", &ID);
	User *user = getUserByID(ID);
	if(user != NULL){
		while(1){
			printf("%s\n", "Modifying user...");
			printf("ID: %d\n", user->ID);
			printf("Name: %s\n", user->name);
			printf("Phone: %s\n", user->phone);
			printf("Email: %s\n", user->email);
			printf("What do you want to change?\n");
			printf("1. Name\n");
			printf("2. Phone\n");
			printf("3. Email\n");
			printf("4. Save\n");
			printf("5. Back to page\n");
			int choice;
			printf("Enter your choice: ");
			scanf("%d", &choice);
			switch(choice){
				case 1:
			    	printf("Enter new name: ");
			    	scanf("%s", user->name);
			    	break;
			    case 2:
			    	printf("Enter new phone: ");  	
			    	scanf("%s", user->phone);
			    	break;
			    case 3:
			    	printf("Enter new email: ");
			    	scanf("%s", user->email);
			    	break;
			    case 4:
			    	saveUser(user);
			    	break;
			    case 5:
			    	displayMenuUser();
			    	break;
			}
		}
	}else{
		printf("Incorrect user ID");
	}
}

void removeUser(){
	clrscr();
	listUsers();
	int ID;
	int numOfRows;
	Request_result reg;
	char request_result_string[256];
	printf("Enter user's ID: ");
	scanf("%d", &ID);
	User *user = getUserByID(ID);
	if(user != NULL){
		while(1){
			printf("%s\n", "Deleting user...");
			printf("ID: %d\n", user->ID);
			printf("Name: %s\n", user->name);
			printf("Phone: %s\n", user->phone);
			printf("Email: %s\n", user->email);
			printf("Are you really want to delete user? (y/n)");
			char decision;
			scanf(" %s", &decision);
			if(decision == 'y'){
				char query[512];
				sprintf(query, "DELETE FROM Users WHERE ID = %d", user->ID);

				send(server_socket, query, sizeof(query), 0);
				recv(server_socket, &numOfRows, sizeof(int), 0);
				recv(server_socket, request_result_string, sizeof(request_result_string), 0);
				sscanf(request_result_string, "%d `%[^`]", (int *)&reg.accepted, reg.comment);
				if(reg.accepted){
					printf("User Successfully deleted\n");
				}else{
					printf("%s", reg.comment);
				}
				break;
			}else{
				printf("The operation has cancelled\n");
				break;
			}
		}
		free(user);
		displayMenuUser();
	}
}

User* getUserByID(int ID){
	char query[512];
	char result[1024];
	User *user = NULL;
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

void saveUser(User *user){
	Request_result reg;
	char request_result_string[256];
	char query[512];
	int numOfRows;
	sprintf(query, "UPDATE Users SET Name = '%s', Phone = '%s', Email = '%s' WHERE ID = %d", user->name, user->phone, user->email, user->ID);
	send(server_socket, query, sizeof(query), 0);
	recv(server_socket, &numOfRows, sizeof(int), 0);
	recv(server_socket, request_result_string, sizeof(request_result_string), 0);
	sscanf(request_result_string, "%d `%[^`]", (int *)&reg.accepted, reg.comment);
	if(reg.accepted)
		printf("Successfully saved\n");
	else
		printf("%s\n", reg.comment);
}
//*************************************/USER MANAGEMENT******************************************************

//**************************************ORDER MANAGEMENT*****************************************************
void listOrders(){
	clrscr();
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
		printf("ID\tUser\tDish\tStatus\tAddress\t# of portions\n");
		for(int i = 0; i < numOfRows; i++){
			int ID = orders[i].ID;
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
				strcpy(status, "Delivered");
			printf("%d\t%s\t%s\t%s\t%s\t%d\n", orders[i].ID, user->name, dish->name, status, orders[i].address, orders[i].noOfPportions);
			if(user != NULL)
				free(user);
			if(dish != NULL)
				free(dish);
		}
		printf("\n");
	}else{
		printf("No orders found\n");
	}
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

void changeOrderStatus(){
	clrscr();
	listOrders();
	int ID;
	printf("Please enter order's ID: ");
	scanf("%d", &ID);
	Order *order = getOrderByID(ID);
	if(order != NULL){
		while(1){
			printf("%s\n", "Changing order status...");
			printf("%s", "Current status: ");
			char status[32];
			if(order->status == 0)
				strcpy(status, "Not checked");
			if(order->status == -1)
				strcpy(status, "Rejected");
			if(order->status == 1)
				strcpy(status, "Approved");
			if(order->status == 2)
				strcpy(status, "Delivered");
			printf("%s\n", status);
			printf("What status do you want to set?\n");
			printf("1. Rejected\n");
			printf("2. Approved\n");
			printf("3. Delivered\n");
			printf("4. Back to page\n");
			int choice;
			printf("Enter your choice: ");
			scanf("%d", &choice);
			switch(choice){
				case 1:
					changeOrdStatus(order, -1);
					break;
				case 2:
					changeOrdStatus(order, 1);
					break;
				case 3:
					changeOrdStatus(order, 2);
					break;
			    case 4:
			    	displayMenuOrder();
			    	break;
			}
		}
	}else{
		printf("Incorrect user ID");
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
	if(reg.accepted)
		printf("Status Successfully changed\n");
	else{
		printf("%s\n", reg.comment);
	}
	free(order);
	displayMenuOrder();
}

void removeOrder(){
	clrscr();
	listOrders();
	int ID;
	int numOfRows;
	Request_result reg;
	char request_result_string[256];
	printf("Enter order's ID: ");
	scanf("%d", &ID);
	Order *order = getOrderByID(ID);
	if(order != NULL){
		while(1){
			printf("%s\n", "Deleting order...");
			printf("ID: %d\n", order->ID);
			char name[255];
			User *user = getUserByID(order->userID);
			printf("User: %s\n", user->name);
			free(user);
			Dish *dish = getDishByID(order->dishID);
			printf("Dish: %s\n", dish->name);
			free(dish);
			char status[32];
			if(order->status == 0)
				strcpy(status, "Not checked");
			if(order->status == -1)
				strcpy(status, "Rejected");
			if(order->status == 1)
				strcpy(status, "Approved");
			if(order->status == 2)
				strcpy(status, "Delivered");
			printf("Status: %s\n", status);
			printf("Address: %s\n", order->address);
			printf("Num of portions: %d\n", order->noOfPportions);
			printf("Are you really want to delete order? (y/n)");
			char decision;
			scanf(" %s", &decision);
			if(decision == 'y'){
				char query[512];
				sprintf(query, "DELETE FROM Orders WHERE ID = %d", order->ID);
				send(server_socket, query, sizeof(query), 0);
				recv(server_socket, &numOfRows, sizeof(int), 0);
				recv(server_socket, request_result_string, sizeof(request_result_string), 0);
				sscanf(request_result_string, "%d `%[^`]", (int *)&reg.accepted, reg.comment);
				if(reg.accepted){
					printf("Order Successfully deleted\n");
				}else{
					printf("%s\n", reg.comment);
				}
				break;
			}else{
				printf("The operation has cancelled\n");
				break;
			}
		}
		free(order);
		displayMenuOrder();
	}
}
//**************************************/ORDER MANAGEMENT*****************************************************

//***************************************BOOK MANAGEMENT******************************************************
void listBooks(){
	clrscr();
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
		printf("ID\tUser\t# of people\tDate\tStatus\n");
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
			printf("%d\t%s\t%d\t%s\t%s\n", books[i].ID, user->name, table->noOfPeople, books[i].date, status);
			free(user);
			free(table);
		}
		printf("\n");
	}else{
		printf("No bookings found\n");
	}
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

void changeBookStatus(){
	clrscr();
	listBooks();
	int ID;
	printf("Please enter book's ID: ");
	scanf("%d", &ID);
	Book *book = getBookByID(ID);
	if(book != NULL){
		while(1){
			printf("%s\n", "Changing book status...");
			printf("%s", "Current status: ");
			char status[32];
			if(book->status == 0)
				strcpy(status, "Not checked");
			if(book->status == -1)
				strcpy(status, "Rejected");
			if(book->status == 1)
				strcpy(status, "Approved");
			if(book->status == 2)
				strcpy(status, "Served");
			printf("%s\n", status);
			printf("What status do you want to set?\n");
			printf("1. Rejected\n");
			printf("2. Approved\n");
			printf("3. Served\n");
			printf("4. Back to page\n");
			int choice;
			printf("Enter your choice: ");
			scanf("%d", &choice);
			switch(choice){
				case 1:
					changeBkStatus(book, -1);
					break;
				case 2:
					changeBkStatus(book, 1);
					break;
				case 3:
					changeBkStatus(book, 2);
					break;
			    case 4:
			    	displayMenuBook();
			    	break;
			}
		}
		free(book);
	}else{
		printf("Incorrect book ID");
	}
}

void changeBkStatus(Book *book, int status){
	Request_result reg;
	char request_result_string[256];
	char query[512];
	int numOfRows;
	sprintf(query, "UPDATE Bookings SET Status = %d WHERE ID = %d", status, book->ID);
	printf("%s\n", query);
	send(server_socket, query, sizeof(query), 0);
	recv(server_socket, &numOfRows, sizeof(int), 0);
	if(numOfRows == 1)
		printf("Status Successfully changed\n");
	else{
		printf("Status Unsuccessfully changed\n");
	}
	recv(server_socket, request_result_string, sizeof(request_result_string), 0);
	sscanf(request_result_string, "%d `%[^`]", (int *)&reg.accepted, reg.comment);
	displayMenuBook();
}

void removeBook(){
	clrscr();
	listBooks();
	int ID;
	int numOfRows;
	Request_result reg;
	char request_result_string[256];
	printf("Enter book's ID: ");
	scanf("%d", &ID);
	Book *book = getBookByID(ID);
	if(book != NULL){
		while(1){
			printf("%s\n", "Deleting book...");
			printf("ID: %d\n", book->ID);
			User *user = getUserByID(book->userID);
			printf("User: %s\n", user->name);
			Table *table = getTableByID(book->tableID);
			printf("# of people: %d\n", table->noOfPeople);
			if(user != NULL)
				free(user);
			if(table != NULL)
				free(table);
			printf("Date: %s\n", book->date);
			char status[32];
			if(book->status == 0)
				strcpy(status, "Not checked");
			if(book->status == -1)
				strcpy(status, "Rejected");
			if(book->status == 1)
				strcpy(status, "Approved");
			if(book->status == 2)
				strcpy(status, "Served");
			printf("Status: %s\n", status);
			printf("Are you really want to delete order? (y/n)");
			char decision;
			scanf(" %s", &decision);
			if(decision == 'y'){
				char query[512];
				sprintf(query, "DELETE FROM Bookings WHERE ID = %d", book->ID);
				send(server_socket, query, sizeof(query), 0);
				recv(server_socket, &numOfRows, sizeof(int), 0);
				recv(server_socket, request_result_string, sizeof(request_result_string), 0);
				sscanf(request_result_string, "%d `%[^`]", (int *)&reg.accepted, reg.comment);
				if(numOfRows == 1){
					printf("Book Successfully deleted\n");
				}else{
					printf("Error occured during deletion\n");
				}
				break;
			}else{
				printf("The operation has cancelled\n");
				break;
			}
		}
		free(book);
		displayMenuBook();
	}
}
//**************************************/BOOK MANAGEMENT******************************************************

//***************************************MENU MANAGEMENT******************************************************
void listMenu(){
	clrscr();
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
		printf("ID\tName\tPrice\tCategory\tStatus\n");
		for(int i = 0; i < numOfRows; i++){
			Category *category = getCategoryByID(dishes[i].categoryID);
			char status[32];
			if(dishes[i].status == 0)
				strcpy(status, "Not available");
			if(dishes[i].status == 1)
				strcpy(status, "Available");
			printf("%d\t%s\t%d\t%s\t%s\n", dishes[i].ID, dishes[i].name, dishes[i].price, category->name, status);
			free(category);
		}
		printf("\n");
	}else{
		printf("No dishes found\n");
	}
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

void addDish(){
	clrscr();
	Dish *dish = malloc(sizeof(Dish));
	printf("Adding new dish\n");
	printf("Enter dish name: ");
	scanf("%s", dish->name);
	printf("Enter a price: ");
	scanf("%d", &(dish->price));
	printf("Choose a category: \n");
	printf("1. First meal\n");
	printf("2. Second meal\n");
	printf("3. Drinks\n");
	printf("4. Salad\n");
	scanf("%d", &(dish->categoryID));
	printf("Choose a status: \n");
	printf("0. Unavailable\n");
	printf("1. Available\n");
	scanf("%d", &(dish->status));
	saveDish(dish, 0); //0 --> create, 1 --> update
}

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
	free(dish);
	if(reg.accepted){
		printf("Dish saved Successfully\n");
	}else{
		printf("Error occured during the saving of dish\n");
	}
	sleep(3);
	clrscr();
	displayMenuMenu();
}

void modifyDish(){
	clrscr();
	listMenu();
	int ID;
	printf("Please enter dish's ID: ");
	scanf("%d", &ID);
	Dish *dish = getDishByID(ID);
	if(dish != NULL){
		while(1){
			clrscr();
			printf("%s\n", "Modifying dish...");
			printf("ID: %d\n", dish->ID);
			printf("Name: %s\n", dish->name);
			printf("Price: %d\n", dish->price);
			Category *category = getCategoryByID(dish->categoryID);
			printf("Category: %s\n", category->name);
			free(category);
			char status[32];
			if(dish->status == 0)
				strcpy(status, "Not available");
			else
				strcpy(status, "Available");
			printf("Status: %s\n", status);
			printf("What do you want to change?\n");
			printf("1. Name\n");
			printf("2. Price\n");
			printf("3. Category\n");
			printf("4. Status\n");
			printf("5. Save\n");
			printf("6. Back to page\n");
			int choice;
			printf("Enter your choice: ");
			scanf("%d", &choice);
			switch(choice){
				case 1:
			    	printf("Enter new name: ");
			    	scanf("%s", dish->name);
			    	break;
			    case 2:
			    	printf("Enter new price: ");  	
			    	scanf("%d", &(dish->price));
			    	break;
			    case 3:
			    	printf("Choose a category: \n");
					printf("1. First meal\n");
					printf("2. Second meal\n");
					printf("3. Drinks\n");
					printf("4. Salad\n");
					scanf("%d", &(dish->categoryID));
			    	break;
			    case 4:
			    	printf("Choose a status: \n");
					printf("0. Unavailable\n");
					printf("1. Available\n");
					scanf("%d", &(dish->status));
			    	break;
			    case 5:
			    	saveDish(dish, 1);
			    	break;
			    case 6:
			    	displayMenuMenu();
			    	break;
			   	default:
			  	 	attemptAgain();
			}
		}
		free(dish);
	}else{
		printf("Incorrect user ID");
	}
}

void removeDish(){
	clrscr();
	listMenu();
	int ID;
	int numOfRows;
	Request_result reg;
	char request_result_string[256];
	printf("Enter dish's ID: ");
	scanf("%d", &ID);
	Dish *dish = getDishByID(ID);
	if(dish != NULL){
		while(1){
			printf("%s\n", "Deleting dish...");
			printf("%s\n", "Modifying dish...");
			printf("ID: %d\n", dish->ID);
			printf("Name: %s\n", dish->name);
			printf("Price: %d\n", dish->price);
			Category *category = getCategoryByID(dish->categoryID);
			printf("Category: %s\n", category->name);
			free(category);
			char status[32];
			if(dish->status == 0)
				strcpy(status, "Not available");
			else
				strcpy(status, "Available");
			printf("Status: %s\n", status);
			printf("Are you really want to delete order? (y/n)");
			char decision;
			scanf(" %s", &decision);
			if(decision == 'y'){
				char query[512];
				sprintf(query, "DELETE FROM Dishes WHERE ID = %d", dish->ID);
				send(server_socket, query, sizeof(query), 0);
				recv(server_socket, &numOfRows, sizeof(int), 0);
				recv(server_socket, request_result_string, sizeof(request_result_string), 0);
				sscanf(request_result_string, "%d `%[^`]", (int *)&reg.accepted, reg.comment);
				if(reg.accepted){
					printf("Dish Successfully deleted\n");
				}else{
					printf("%s\n", reg.comment);
				}
				break;
			}else{
				printf("The operation has cancelled\n");
				break;
			}
		}
		free(dish);
		displayMenuMenu();
	}
}
//**************************************/MENU MANAGEMENT******************************************************

//***************************************WAITER MANAGEMENT****************************************************
void listWaiters(){
	clrscr();
	char query[512] = "SELECT * FROM Employees WHERE Role = 1";
	char result[1024];
	send(server_socket, query, sizeof(query), 0);
	int numOfRows;
	recv(server_socket, &numOfRows, sizeof(int), 0);
	if(numOfRows > 0){
		Employee employees[numOfRows];
		for(int i = 0; i < numOfRows; i++){
			recv(server_socket, result, sizeof(result), 0);
			sscanf(result, " '%d' '%[^']' '%[^']' '%[^']' '%d'", &(employees[i].ID), employees[i].name, employees[i].login, employees[i].password, &(employees[i].role));
		}
		printf("ID\tName\tLogin\tPassword\tRole\n");
		for(int i = 0; i < numOfRows; i++)
			printf("%d\t%s\t%s\t%s\t%s\n", employees[i].ID, employees[i].name, employees[i].login, employees[i].password, "Waiter");
		printf("\n");
	}else{
		printf("No waiters found\n");
	}
}

void addWaiter(){
	clrscr();
	Employee *waiter = malloc(sizeof(Employee));
	printf("%s\n", "Registering waiter");
	printf("%s", "Enter waiter name: ");
	scanf("%s", waiter->name);
	printf("%s", "Enter waiter login: ");
	scanf("%s", waiter->login);
	printf("%s", "Enter waiter password: ");
	scanf("%s", waiter->password);
	waiter->role = 1;
	clrscr();
	saveWaiter(waiter, 0);
}

void saveWaiter(Employee *employee, int type){
	Request_result reg;
	char request_result_string[256];
	char query[512];
	int numOfRows;
	if(type == 0)
		sprintf(query, "INSERT INTO Employees(Name, Login, Password, Role) VALUES ('%s', '%s', '%s', %d)", employee->name, employee->login, employee->password, employee->role);
	else
		sprintf(query, "UPDATE Employees SET Name = '%s', Login = '%s', password = '%s', Role = %d WHERE ID = %d", employee->name, employee->login, employee->password, employee->role, employee->ID);
	send(server_socket, query, sizeof(query), 0);
	recv(server_socket, &numOfRows, sizeof(int), 0);
	recv(server_socket, request_result_string, sizeof(request_result_string), 0);
	sscanf(request_result_string, "%d `%[^`]", (int *)&reg.accepted, reg.comment);
	if(reg.accepted)
		printf("Waiter successfully saved\n");
	else{
		printf("%s\n", reg.comment);
	}
	free(employee);
	sleep(3);
	clrscr();
	displayMenuWaiter();
}

void modifyWaiter(){
	clrscr();
	listWaiters();
	int ID;
	printf("Please enter waiter's ID: ");
	scanf("%d", &ID);
	Employee *waiter = getWaiterByID(ID);
	if(waiter != NULL){
		while(1){
			printf("%d\n", waiter->role);
			printf("%s\n", "Modifying waiter...");
			printf("ID: %d\n", waiter->ID);
			printf("Name: %s\n", waiter->name);
			printf("Login: %s\n", waiter->login);
			printf("Password: %s\n", waiter->password);
			printf("What do you want to change?\n");
			printf("1. Name\n");
			printf("2. Login\n");
			printf("3. Password\n");
			printf("4. Make waiter as admin\n");
			printf("5. Save\n");
			printf("6. Back to page\n");
			int choice;
			printf("Enter your choice: ");
			scanf("%d", &choice);
			switch(choice){
				case 1:
			    	printf("Enter new name: ");
			    	scanf("%s", waiter->name);
			    	break;
			    case 2:
			    	printf("Enter new login: ");  	
			    	scanf("%s", waiter->login);
			    	break;
			    case 3:
			    	printf("Enter new password: ");
			    	scanf("%s", waiter->password);
			    	break;
			    case 4:
			    	waiter->role = 0;
			    	printf("Waiter became as admin\n");
			    	sleep(3);
			    	break;
			    case 5:
			    	saveWaiter(waiter, 1);
			    	break;
			    case 6:
			    	displayMenuWaiter();
			    	break;
			}
		}
		free(waiter);
	}else{
		printf("Incorrect waiter ID");
	}
}

Employee *getWaiterByID(int ID){
	char query[512];
	char result[1024];
	Employee *waiter = NULL;
	sprintf(query, "SELECT * FROM Employees WHERE ID = %d AND Role = 1", ID);
	int count;
	send(server_socket, query, sizeof(query), 0);
	recv(server_socket, &count, sizeof(int), 0);
	if(count == 1){
		waiter = malloc(sizeof(Employee));
		recv(server_socket, result, sizeof(result), 0);
		sscanf(result, " '%d' '%[^']' '%[^']' '%[^']' '%d'", &(waiter->ID), waiter->name, waiter->login, waiter->password, &(waiter->role));
	}
	return waiter;
}

void removeWaiter(){
	clrscr();
	listWaiters();
	int ID;
	int numOfRows;
	Request_result reg;
	char request_result_string[256];
	printf("Enter waiter's ID: ");
	scanf("%d", &ID);
	Employee *waiter = getWaiterByID(ID);
	if(waiter != NULL){
		while(1){
			printf("%s\n", "Deleting waiter...");
			printf("ID: %d\n", waiter->ID);
			printf("Name: %s\n", waiter->name);
			printf("Login: %s\n", waiter->login);
			printf("Password: %s\n", waiter->password);
			printf("Are you really want to delete waiter? (y/n)");
			char decision;
			scanf(" %s", &decision);
			if(decision == 'y'){
				char query[512];
				sprintf(query, "DELETE FROM Employees WHERE ID = %d", waiter->ID);
				send(server_socket, query, sizeof(query), 0);
				recv(server_socket, &numOfRows, sizeof(int), 0);
				recv(server_socket, request_result_string, sizeof(request_result_string), 0);
				sscanf(request_result_string, "%d `%[^`]", (int *)&reg.accepted, reg.comment);
				if(reg.accepted){
					printf("Waiter Successfully deleted\n");
				}else{
					printf("%s\n", reg.comment);
				}
				break;
			}else{
				printf("The operation has cancelled\n");
				break;
			}
		}
		free(waiter);
		displayMenuWaiter();
	}
}


//***************************************/WAITER MANAGEMENT***************************************************

//*************************************************MESSAGES***************************************************
void sendMessage(){
	printf("Sending message....\n");
	listUsers();
	printf("Please enter user's ID: ");
	int ID;
	scanf("%d", &ID);
	User *user = getUserByID(ID);
	if(user != NULL){
		Message *message = malloc(sizeof(Message));
		message->userID = ID;
		clrscr();
		printf("Sending message to: %s\n", user->name);
		printf("Enter the message text below please: ");
		scanf(" %[^\n]s", message->text);
		free(user);
		saveMessage(message);
	}else{
		printf("Incorrect user ID entered\n");
	}
}

void saveMessage(Message *message){
	Request_result reg;
	char request_result_string[256];
	char query[512];
	int numOfRows;
	sprintf(query, "INSERT INTO Messages(User_id, Text) VALUES (%d, '%s')", message->userID, message->text);
	send(server_socket, query, sizeof(query), 0);
	recv(server_socket, &numOfRows, sizeof(int), 0);
	recv(server_socket, request_result_string, sizeof(request_result_string), 0);
	sscanf(request_result_string, "%d `%[^`]", (int *)&reg.accepted, reg.comment);
	if(reg.accepted){
		printf("Message successfully send\n");
	}else{
		printf("%s\n", reg.comment);
	}
	free(message);
	sleep(3);
}

void listMessage(){
	printf("Printing messages ...\n");
	listUsers();
	printf("Please enter user's ID: ");
	int ID;
	scanf("%d", &ID);
	User *user = getUserByID(ID);
	if(user != NULL){
		clrscr();
		printf("Message history of the user --> %s\n", user->name);
		printMessages(user);
	}else{
		printf("Incorrect user's ID\n");
	}
}

void printMessages(User *user){
	char query[512];
	sprintf(query, "SELECT * FROM Messages WHERE ID = %d", user->ID);
	char result[1024];
	send(server_socket, query, sizeof(query), 0);
	int numOfRows;
	recv(server_socket, &numOfRows, sizeof(int), 0);
	if(numOfRows > 0){
		Message messages[numOfRows];
		for(int i = 0; i < numOfRows; i++){
			recv(server_socket, result, sizeof(result), 0);
			sscanf(result, " '%d' '%d' '%[^']' '%d'", &(messages[i].ID), &(messages[i].userID), messages[i].text, &(messages[i].status));
		}
		printf("Text\tStatus\n");
		for(int i = 0; i < numOfRows; i++){
			char status[32];
			if(messages[i].status == 0)
				strcpy(status, "Not read");
			else
				strcpy(status, "Read");
			printf("%s\t%s\n", messages[i].text, status);
		}
		printf("\n");
	}else{
		printf("No messages found\n");
	}
	free(user);
}
//************************************************/MESSAGES***************************************************

//****************************************MENUS**************************************************************
void greetingFunction(){
	while(1){
		printf("%s\n", "Welcome to Restaurant Management System");
		printf("%s\n", "Menu");
		printf("%s\n", "1.Login");
		printf("%s\n", "2.Register");
		printf("%s\n", "3.Exit");
		printf("%s", "Please enter your choice: ");
		int choice;
		scanf("%d", &choice);
		clrscr();
		switch(choice){
			case 1:
				loginMenu();
				break;
			case 2:
				reg();
				break;
			case 3:
				exitApp();
				break;
			default:
				attemptAgain();
		}
	}
}

void loginMenu(){
	int check = login();
	while(check){
		afterLoggingInMenu();		
	}
}

void afterLoggingInMenu(){
	while(1){
		clrscr();
		printf("%s\n", "Management");
		printf("%s\n", "1.Managing users");
		printf("%s\n", "2.Managing orders");
		printf("%s\n", "3.Managing books");
		printf("%s\n", "4.Managing menu");
		printf("%s\n", "5.Managing waiters");
		printf("%s\n", "6.Message management");
		printf("%s\n", "7.Back to page");
			int choice;
			scanf("%d", &choice);
			clrscr();
			switch(choice){
				case 1:
					displayMenuUser();
					break;
				case 2:
					displayMenuOrder();
					break;
				case 3:
					displayMenuBook();
					break;
				case 4:
					displayMenuMenu();
					break;
				case 5:
					displayMenuWaiter();
					break;
				case 6:
					displayMessageMenu();
					break;
				case 7:
					greetingFunction();
					break;
				default:
					attemptAgain();
		}
	}
}

void displayMenuUser(){
	while(1){
		printf("%s\n", "Managing users");
		printf("%s\n", "1.List users");
		printf("%s\n", "2.Add user");
		printf("%s\n", "3.Modify user");
		printf("%s\n", "4.Remove user");
		printf("%s\n", "5.Back to page");
		int choice;
		scanf("%d", &choice);
		switch(choice){
			case 1:
				listUsers();
				break;
			case 2:
				reg();
				break;
			case 3:
				modifyUser();
				break;
			case 4:
				removeUser();
				break;
			case 5:
				afterLoggingInMenu();
				break;
			default:
				attemptAgain();
		}
	}
}

void displayMenuOrder(){
	while(1){
		printf("%s\n", "Managing orders");
		printf("%s\n", "1.List orders");
		printf("%s\n", "2.Modify order");
		printf("%s\n", "3.Remove order");
		printf("%s\n", "4.Back to page");
		int choice;
		scanf("%d", &choice);
		switch(choice){
			case 1:
				listOrders();
				break;
			case 2:
				changeOrderStatus();
				break;
			case 3:
				removeOrder();
				break;
			case 4:
				afterLoggingInMenu();
			default:
				attemptAgain();
		}
	}
}

void displayMenuBook(){
	while(1){
		printf("%s\n", "Managing books");
		printf("%s\n", "1.List books");
		printf("%s\n", "2.Change status");;
		printf("%s\n", "3.Remove book");
		printf("%s\n", "4.Back to page");
		int choice;
		scanf("%d", &choice);
		switch(choice){
			case 1:
				listBooks();
				break;
			case 2:
				changeBookStatus();
				break;
			case 3:
				removeBook();
				break;
			case 4:
				afterLoggingInMenu();
				break;
			default:
				attemptAgain();
				break;
		}
	}
}

void displayMenuMenu(){
	while(1){
		printf("%s\n", "Managing menu");
		printf("%s\n", "1.List menu");
		printf("%s\n", "2.Add dish");
		printf("%s\n", "3.Modify dish");
		printf("%s\n", "4.Remove dish");
		printf("%s\n", "5.Back to page");
		int choice;
		scanf("%d", &choice);
		switch(choice){
			case 1:
				listMenu();
				break;
			case 2:
				addDish();
				break;
			case 3:
				modifyDish();
				break;
			case 4:
				removeDish();
				break;
			case 5:
				afterLoggingInMenu();
				break;
			default:
				attemptAgain();
		}
	}
}

void displayMenuWaiter(){
	while(1){
		printf("%s\n", "Managing waiters");
		printf("%s\n", "1.List waiters");
		printf("%s\n", "2.Add waiter");
		printf("%s\n", "3.Modify waiter");
		printf("%s\n", "4.Remove waiter");
		printf("%s\n", "5.Back to page");
		int choice;
		scanf("%d", &choice);
		switch(choice){
			case 1:
				listWaiters();
				break;
			case 2:
				addWaiter();
				break;
			case 3:
				modifyWaiter();
				break;
			case 4:
				removeWaiter();
				break;
			case 5:
				afterLoggingInMenu();
			default:
				attemptAgain();
		}
	}
}

void displayMessageMenu(){
	while(1){
		printf("%s\n", "Managing messages");
		printf("%s\n", "1.Send message");
		printf("%s\n", "2.List message");
		printf("%s\n", "3.Back to page");
		int choice;
		scanf("%d", &choice);
		switch(choice){
			case 1:
				sendMessage();
				break;
			case 2:
				listMessage();
				break;
			case 3:
				afterLoggingInMenu();
			default:
				attemptAgain();
		}
	}
}

void exitApp(){
	printf("%s\n", "Thank you for using our application!!!");
	exit(1);
}

void attemptAgain(){
	printf("%s\n", "Please choose the option listed above!!!");
}
//***************************************/MENUS**************************************************************



//***************************************ADDITIONAL FUNCTIONS ***********************************************
void clrscr(){
	printf("\033c");
}
//**************************************/ADDITIONAL FUNCTIONS ***********************************************