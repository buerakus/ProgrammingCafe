#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "structures.h"
#include "functions.h"
#define admin_log "admin"
#define admin_pass "admin"

int main() {
	int runtime = 1;
	//runtime and while loop used for endless looping programm in case of unsuccessful logins, stopping at exit option in main menu of cafe 
	char input[32];
	int Access = 0;
	char login[32];
	char password[32];
	int loginsuccess;
	//declarations of variables used in main function

	while (runtime) {
		printf("Welcome to the Programming Cafe Management, please choose your access level : \n1. Student\n2. Tutor\n3. Admin\n0. Exit\n");
		fgets(input, sizeof(input), stdin);
		if (sscanf(input, "%d", &Access) == 1) {

			switch (Access) {
			case 0:
				printf("Exiting the program\n");
				runtime = 0;
				break;

				//case 1 - students menu and options
			case 1:
				printf("input TP number:\n");
				fgets(login, 32, stdin);
				printf("input password:\n");
				fgets(password, 32, stdin);
				normalization(login);
				normalization(password);
				struct Student StudentLatest;
				int StudentNum;
				// loginsuccess variable either grants access (1) or denies (0)
				loginsuccess = studentLogin(login, password, &StudentLatest, &StudentNum);
				if (loginsuccess) {
					studentMenu(&StudentLatest, StudentNum);
				}
				else if (loginsuccess == 0) 
				break;
			default:
				printf("Invalid command entered.\n");
				break;
			//case 2 - tutors menu and options
			case 2:
				printf("Enter the Tutor Code:\n");
				fgets(login, 32, stdin);
				printf("Enter the password:\n");
				fgets(password, 32, stdin);
				normalization(login);
				normalization(password);
				printf("Your login and password are: %s %s\n", login, password);
				struct Tutor TutorLatest;
				int TutorNum;
				loginsuccess = tutorLogin(login, password, &TutorLatest, &TutorNum);
				if (loginsuccess) {
					tutorMenu(&TutorLatest, TutorNum);
				}
				else if (loginsuccess == 0) 
				break;
			//case 3 - admin menu and options
			case 3:
				printf("Enter the login:\n");
				fgets(login, 32, stdin);
				printf("Enter the password:\n");
				fgets(password, 32, stdin);
				normalization(login);
				normalization(password);
				if ((strcmp(login, admin_log) == 0) && (strcmp(password, admin_pass) == 0)) {
					adminMenu();
				}
				else {
					printf("Wrong login and password!\n");
				}

				break;


			}
		}

	}

	return 0;
}