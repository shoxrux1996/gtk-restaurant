/*
	tasks:
		free(user) in dashboard logout
		accomplish request_booking function
		accomplish request_order function
*/
#include <stdio.h>
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

int main()
{
	server_socket = createSocket();
	menu();
	return 0;
}

void menu(){
	int choice;
	
	while(1)
	{

		printf("\033c");
		printf("----------Client----------\n");
		printf("1) Login\n");
		printf("2) Register\n");
		printf("Your choice: ");
		scanf("%d", &choice);
		getchar();

		printf("\033c");
		if(choice == 1)
		{
			login_menu();
		}else if(choice == 2){
			register_menu();
		}
		
		
	}
	
}

void login_menu(){
	char email[255], password[255];
	printf("----------Login menu----------\n");
	printf("Email: ");
	scanf("%s", email);
	printf("Password: ");
	scanf("%s", password);

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

	}
		
}

void dashboard_menu(User *user){
	
	char query[512];
	int num_rows;
	char row_string[1024];
	int new_messages;

	
	int choice; 

	do{
		sprintf(query, "SELECT COUNT(*) FROM Messages WHERE User_id = %d AND Status = 0", user->ID);
		send(server_socket, query, sizeof(query), 0);
		recv(server_socket, &num_rows, sizeof(int), 0);

		recv(server_socket, row_string, sizeof(row_string), 0);
		sscanf(row_string, " '%d'", &new_messages);
		
		printf("\033c");
		printf("----------Welcome, %s----------\n", user->name);
		printf("1) Booking a seat\n");
		printf("2) Order food\n");
		printf("3) Messages (%d)\n", new_messages);
		printf("-----------------\n");
		printf("0) Logout\n");
		printf("Your choice: ");
		scanf("%d", &choice);

		if(choice == 1)
		{
			booking_menu(user);

		}else if(choice == 2)
		{
			ordering_food_menu(user);

		}else if(choice == 3)
		{
			messages_menu(user);
		}

	}while(choice != 0);

}

void register_menu(){
	
	char name[255],phone[12], email[255], password[255];
	Request_result reg;
	do{
		printf("----------Register menu----------\n");
		printf("Enter your name: ");
		fgets(name, 255, stdin);
		name[strlen(name)-1] = '\0';
		printf("Enter your phone (Ex: 901686962): ");
		fgets(phone, 12, stdin);
		phone[strlen(phone)-1] = '\0';
		printf("Enter your email: ");
		fgets(email, 255, stdin);
		email[strlen(email)-1] = '\0';
		printf("Enter your password: ");
		fgets(password, 255, stdin);
		password[strlen(password)-1] = '\0';

		reg = request_register(name, phone, email, password);

		if(reg.accepted)
			printf( ANSI_COLOR_GREEN "%s\n" ANSI_COLOR_RESET, reg.comment);
		else
			printf( ANSI_COLOR_RED "%s\n" ANSI_COLOR_RESET, reg.comment);
		
		sleep(2);

	}while(!reg.accepted);
	

}

void booking_menu(User *user){
	int num_of_people;
	Time *time = malloc(sizeof(Time));
	char comments[1024];

	printf("\033c");
	printf("----------Booking a seat----------\n");
	printf("Enter number of people: ");
	scanf("%d", &num_of_people);
	printf("Enter time of reservation(dd.mm.yyyy, h:m): ");
	scanf("%d.%d.%d, %d:%d", &time->day, &time->month, &time->year, &time->hour, &time->minute);
	sprintf(time->format, "%04d-%02d-%02d, %02d:%02d:00", time->year, time->month, time->day, time->hour, time->minute);

	//printf("Time: %s\n", time->format);
	//sleep(50);

	printf("Enter comments for reservation: ");
	scanf("%s", comments);

	Request_result booking = request_booking(user, num_of_people, *time, comments);
	free(time);
	if(booking.accepted)
		printf( ANSI_COLOR_GREEN "%s\n" ANSI_COLOR_RESET, booking.comment);
	else
		printf( ANSI_COLOR_RED "%s\n" ANSI_COLOR_RESET, booking.comment);
	
	sleep(3);
	
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

void ordering_food_menu(User *user){
	int choice;

	do{
		printf("\033c");
		printf("Choose the category: \n");
		printf("1) First meal\n");
		printf("2) Second meal\n");
		printf("3) Drinks\n");
		printf("4) Salads\n");
		printf("0) Back\n");
		printf("Your choice: ");
		scanf("%d", &choice);


		
		if(choice > 0 && choice < 5)
		{
			int meal_id = choose_meal(user, choice);

			if(meal_id != 0)
			{
				int portions;
				char address[255];
				printf("Enter how many portions would you like to order: ");
				scanf("%d", &portions);
				printf("Enter your address: ");
				scanf("%s", address);

				Request_result ordering = request_order(user, meal_id, portions, address);

				if(ordering.accepted)
					printf( ANSI_COLOR_GREEN "%s\n" ANSI_COLOR_RESET, ordering.comment);
				else
					printf( ANSI_COLOR_RED "%s\n" ANSI_COLOR_RESET, ordering.comment);
				
				sleep(2);
			}
			
		}
		
	}while(choice != 0);
	
}

int choose_meal(User *user, int category_id){

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

	
	do{
		printf("\033c");
		
		printf(ANSI_COLOR_BLUE "%-7s %-15s %-10s\n" ANSI_COLOR_RESET, "ID", "Name", "Price");
	
		for (int i = 0; i < num_rows; ++i)
		{	
			printf("%-7d %-15s %-10d\n", meals[i].ID, meals[i].name, meals[i].price);
		}
		printf("--------------------\n");
		printf("0) Back\n");
		printf("Your choice (choose ID): ");
		scanf("%d", &choice);

		return choice;
		
	}while(choice != 0);

	return 0;
}

void messages_menu(User *user){
	
	char query[512];
	char row_string[1024];
	char message[512];
	int status;
	int num_rows;
	int choice;

	sprintf(query, "SELECT Text, Status FROM Messages WHERE User_id = %d ORDER BY ID DESC", user->ID);
	send(server_socket, query, sizeof(query), 0);
	recv(server_socket, &num_rows, sizeof(num_rows), 0);


	
	printf("\033c");
	printf("Messages: \n");
	printf("-------------\n");
	
	for (int i = 0; i < num_rows; ++i)
	{
		recv(server_socket, row_string, sizeof(row_string), 0);
		sscanf(row_string, " '%[^']' '%d'", message, &status);

		if(status == 0)
			printf(ANSI_COLOR_GREEN "*) %s\n" ANSI_COLOR_RESET, message);
		else
			printf("*) %s\n", message);
	}


	printf("------------------\n");
	printf("0) Back\n");

	printf("Enter any number to return: ");
	scanf("%d", &choice);
	

	sprintf(query, "UPDATE Messages SET Status = 1 WHERE User_id = %d AND Status = 0", user->ID);
	send(server_socket, query, sizeof(query), 0);
	recv(server_socket, &num_rows, sizeof(int), 0);

	recv(server_socket, row_string, sizeof(row_string), 0);
	
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