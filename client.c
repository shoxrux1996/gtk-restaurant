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
} Time;

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
	int choice; 

	do{
		printf("\033c");
		printf("----------Welcome, %s----------\n", user->name);
		printf("1) Booking a seat\n");
		printf("2) Order food\n");
		printf("3) Messages (5)\n");
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
	Time time;
	char comments[1024];

	printf("\033c");
	printf("----------Booking a seat----------\n");
	printf("Enter number of people: ");
	scanf("%d", &num_of_people);
	printf("Enter time of reservation(dd.mm.yyyy, h:m): ");
	scanf("%d.%d.%d, %d:%d", &time.day, &time.month, &time.year, &time.hour, &time.minute);
	printf("Enter comments for reservation: ");
	scanf("%s", comments);

	Request_result booking = request_booking(user, num_of_people, time, comments);

	if(booking.accepted)
		printf( ANSI_COLOR_GREEN "%s\n" ANSI_COLOR_RESET, booking.comment);
	else
		printf( ANSI_COLOR_RED "%s\n" ANSI_COLOR_RESET, booking.comment);
	
	sleep(1);
	
}

User* authenticate(char *email, char *password){//return user struct on success, NULL otherwise
	User *user = NULL;
	
	char query[512];
	int num_rows;
	char row_string[1024];

	sprintf(query, "SELECT name, phone, email, password FROM Users WHERE email = '%s' AND password = '%s'", email, password);

	send(server_socket, query, sizeof(query), 0);
	recv(server_socket, &num_rows, sizeof(int), 0);
	if(num_rows>0)
	{
		user = malloc(sizeof(User));
		recv(server_socket, row_string, sizeof(row_string), 0);
		sscanf(row_string, " '%[^']' '%[^']' '%[^']' '%[^']'", user->name, user->phone, user->email, user->password);
	
	}
	
	return user;
}

Request_result request_booking(User *user, int num_of_people, Time time, char *comments){// return Booking_req struct object 

	char query[512];

	
	Request_result booking;
	booking.accepted = true;
	strcpy(booking.comment, "Your request has successfully been booked!");
	return booking;
}

Request_result request_order(User *user, int meal_id, int num_of_portions, char *address){
	Request_result ordering;
	ordering.accepted = true;
	strcpy(ordering.comment, "Your request for ordering has successfully been arranged!");

	return ordering;
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
	printf("Request_result: %s\n", request_result_string);
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


		
		if(choice > 0 && choice < 4)
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

	int choice;

	do{
		printf("\033c");
		printf("First meals: \n");
		printf("1) Palov\n");
		printf("2) Bifstrogan\n");
		printf("3) Chicken\n");
		printf("--------------------\n");
		printf("0) Back\n");
		printf("Your choice: ");
		scanf("%d", &choice);

		if(choice > 0 && choice < 3)
		{
			return choice;
		}
	}while(choice != 0);

	return 0;
}

void messages_menu(User *user){
	int choice;

	do{
		printf("\033c");
		printf("Messages: \n");
		printf("* Your booking has successfully been accepted!\n");
		printf("* Your order has successfully been arranged!\n");
		printf("* Your order has successfully been arranged!\n");

		printf("------------------\n");
		printf("0) Back\n");

		printf("Enter 0 to go back: ");
		scanf("%d", &choice);
	}while(choice != 0);
	
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