#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "structures.h"



void createStudent(struct Student* newStudentP);
void createSession(struct Session* newSessionP);
void createTutor(struct Tutor* newTutorP);
void assignSessionToTutor(struct Session* sessionP, struct Tutor* tutorP);
void normalization(char* str);
void enrolledSessions(struct Student* sP);
void allSessions(void);
void outputSession(struct Session* sP);
void studentEnroll(struct Student* studentP, struct Session* sessionP);
int structCount(const char* path, size_t structuresSize);
void studentRemove(struct Student* studentP, struct Session* sessionP);
void showTutors(void);
void showStudents(void);
void sessionsWithStudents(void);
void outputStudents(struct Student* sP);



int outputStructsNum(const char* path, size_t structuresSize, void* structP) {
	FILE* fp = fopen(path, "r+b");
	if (fp != NULL) {
		char* comparison = (char*)malloc(structuresSize);
		for (int i = 0; i < structCount(path, structuresSize); i++) {
			fread(comparison, structuresSize, 1, fp);
			if (memcmp(comparison, structP, structuresSize) == 0) {
				return i;
			}
		}
		return -1;
	}
	else {
		fprintf(stderr, "Could not open outputStructsNum\n");
	}
}

int structCount(const char* path, size_t structuresSize) {
	FILE* fp = fopen(path, "r+b");
	if (fp != NULL) {
		fseek(fp, 0, SEEK_END);
		return (ftell(fp) / structuresSize);
	}
	else {
		fprintf(stderr, "Could not open structCount\n");
	}
}

int readstructure(const char* path, size_t structuresSize, int structIndex, void* buffer) {
	FILE* fp = fopen(path, "r+b");
	if ((fp != NULL) && (structuresSize > 0)) {
		fseek(fp, structuresSize * structIndex, SEEK_SET);
		fread(buffer, structuresSize, 1, fp);
		return 0;
	}
	else {
		if (structuresSize <= 0) {
			fprintf(stderr, "Invalid structuresSize parameter\n");
		}
		else {
			fprintf(stderr, "Could not open readstructure\n");
			return -1;
		}
	}
}

int updatestructures(const char* path, size_t structuresSize, int structIndex, void* structP) {
	FILE* fp = fopen(path, "r+b");
	if (fp != NULL) {
		fseek(fp, structuresSize * structIndex, SEEK_SET);
		fwrite(structP, structuresSize, 1, fp);
		return 0;
	}
	else {
		fprintf(stderr, "Could not open readstructure\n");
		return -1;
	}
}

int deletestructure(const char* path, size_t structuresSize, void* structToDeleteP) {
	FILE* fp = fopen(path, "r+b");
	if ((fp != NULL) && (structuresSize > 0)) {
		unsigned long long structNum = 0;
		int fileLength = 0;

		fseek(fp, 0, SEEK_END);
		fileLength = ftell(fp);
		structNum = fileLength / structuresSize;
		rewind(fp);

		if (structNum <= 0) {
		
			fclose(fp);
			FILE* fp = fopen(path, "w+b");
			fclose(fp);
			return 1;
		}

		char* Storage = (char*)malloc((structNum - 1) * structuresSize);
		char* shortStruct = (char*)malloc(structuresSize);
		int found = 0;
		for (int i = 0; i < structNum; i++) {
			fread(shortStruct, structuresSize, 1, fp);
			if (memcmp(structToDeleteP, shortStruct, structuresSize) == 0 && found != 1) {
				found = 1;
				continue;
			}
			memcpy(Storage + i * structuresSize, shortStruct, structuresSize);
		}
		if (!found) {
			fprintf(stderr, "Could not find  struct to be deleted in  file\n");
			return;
		}
		fclose(fp);
		FILE* fp = fopen(path, "w+b");
		for (int i = 0; i < structNum - 1; i++) {
			fwrite(Storage + i * structuresSize, structuresSize, 1, fp);
		}
		free(Storage);
		free(shortStruct);
		return 1;
	}
	else {
		if (structuresSize <= 0) {
			fprintf(stderr, "Invalid structuresSize parameter passed into  function deletestructure\n");
		}
		else {
			fprintf(stderr, "Could not open deletestructure\n");
			return -1;
		}
	}
}


void normalization(char* str) {
	char* c = str;
	while (*c != '\0') {
		if (*c == '\n') {
			*c = '\0';
			return;
		}
		c++;
	}
	return;
}


int isstudentinsession(struct Student* studentP, struct Session* sessionP) {
	for (int i = 0; i < 64; i++) {
		if (strcmp(studentP->enrolledSessionCodes[i], sessionP->code) == 0) {
			return 1;
		}
	}
	return 0;
}

int adminMenu() {
	int exit = 0;
	
	char inputBuffer[256];
	
	int userCommand = 0;
	int TutorNum = 0;
	int sessionIndex = 0;
	
	struct Session currentSession;
	while (!exit) {
		printf("1. Show students\n");
		printf("2. Show tutors\n");
		printf("3. Show all sessions with students\n");
		printf("4. Register a student\n");
		printf("5. Register a tutor\n");
		printf("6. Register a session\n");
		printf("7. Enroll a student\n");
		printf("8. Create session to a tutor\n");
		printf("0. Exit\n");
		fgets(inputBuffer, sizeof(inputBuffer), stdin);
		if (sscanf(inputBuffer, "%d", &userCommand)) {

			switch (userCommand) {
			case 0:
				exit = 1;
				break;

			case 1:
				showStudents();
				break;

			case 2:
				showTutors();
				break;

			case 3:
				sessionsWithStudents();
				break;

			case 4:;
				struct Student newStudent;
				printf("Enter name: ");
				fgets(inputBuffer, sizeof(inputBuffer), stdin);
				sscanf(inputBuffer, "%31[^\n]", newStudent.name);

				
				printf("Enter TP: ");
				fgets(inputBuffer, sizeof(inputBuffer), stdin);
				sscanf(inputBuffer, "%s", newStudent.tp);

				printf("Enter password: ");
				fgets(inputBuffer, sizeof(inputBuffer), stdin);
				sscanf(inputBuffer, "%31[^\n]", newStudent.password);

				newStudent.numEnrolledSessions = 0;

				createStudent(&newStudent);
				printf("Created student\n");
				outputStudents(&newStudent);

				break;

			case 5:;
				struct Tutor newTutor;
				printf("Enter name: ");
				fgets(inputBuffer, sizeof(inputBuffer), stdin);
				sscanf(inputBuffer, "%31[^\n]", newTutor.name);

			
				printf("Enter  Tutor Code: ");
				fgets(inputBuffer, sizeof(inputBuffer), stdin);
				sscanf(inputBuffer, "%s", newTutor.tutorCode);

				printf("Enter  password: ");
				fgets(inputBuffer, sizeof(inputBuffer), stdin);
				sscanf(inputBuffer, "%31[^\n]", newTutor.password);

				newTutor.tutorCode[0] = '\0';

				createSession(&newTutor);
				printf("Created  tutor\n");

				break;

			case 6:;
				struct Session newSession;
				printf("Enter  code: ");
				fgets(inputBuffer, sizeof(inputBuffer), stdin);
				sscanf(inputBuffer, "%s", newSession.code);


				printf("Enter  title: ");
				fgets(inputBuffer, sizeof(inputBuffer), stdin);
				sscanf(inputBuffer, "%127[^\n]", newSession.title);

				printf("Enter  day: ");
				fgets(inputBuffer, sizeof(inputBuffer), stdin);
				sscanf(inputBuffer, "%s", newSession.day);

				printf("Enter  time: ");
				fgets(inputBuffer, sizeof(inputBuffer), stdin);
				sscanf(inputBuffer, "%s", newSession.time);

				printf("Enter  location: ");
				fgets(inputBuffer, sizeof(inputBuffer), stdin);
				sscanf(inputBuffer, "%31[^\n]", newSession.location);
				newSession.tutorCode[0] = '\0';	
				newSession.numEnrolledStudents = 0;	
				int countEnrolledStudents = sizeof(newSession.enrolledStudentTPs) / sizeof(newSession.enrolledStudentTPs[0]);
				for (int i = 0; i < countEnrolledStudents; i++) {
					newSession.enrolledStudentTPs[i][0] = '\0';
				}

				createSession(&newSession);
				printf("Created  session\n");
				outputSession(&newSession);

				break;
			case 7:;
				int StudentNum = 0;
				int sessionIndex = 0;
				struct Student StudentLatest;
				struct Session currentSession;
				printf("Choose  student: \n");
				showStudents();

				fgets(inputBuffer, sizeof(inputBuffer), stdin);
				sscanf(inputBuffer, "%d", &StudentNum);
				StudentNum--;
				if ((StudentNum < 0) || (StudentNum >= structCount("students.dat", sizeof(struct Student)))) {
					printf("Invalid student chosen!\n");
					break;
				}

				printf("Choose  session: \n");
				allSessions();

				fgets(inputBuffer, sizeof(inputBuffer), stdin);
				sscanf(inputBuffer, "%d", &sessionIndex);

				sessionIndex--;
				if ((sessionIndex < 0) || (sessionIndex >= structCount("sessions.dat", sizeof(struct Session)))) {
					printf("Invalid session chosen!\n");
					break;
				}

				readstructure("students.dat", sizeof(struct Student), StudentNum, &StudentLatest);
				readstructure("sessions.dat", sizeof(struct Session), sessionIndex, &currentSession);

				studentEnroll(&StudentLatest, &currentSession);

				updatestructures("students.dat", sizeof(struct Student), StudentNum, &StudentLatest);
				updatestructures("sessions.dat", sizeof(struct Session), sessionIndex, &currentSession);

				printf("Successfully enrolled into  desired session\n");

				break;
			case 8:;
				struct Tutor TutorLatest;

				printf("Choose  session to assign:\n");
				allSessions();
				fgets(inputBuffer, sizeof(inputBuffer), stdin);
				sscanf(inputBuffer, "%d", &sessionIndex);

				sessionIndex--;
				if ((sessionIndex < 0) || (sessionIndex >= structCount("sessions.dat", sizeof(struct Session)))) {
					printf("Invalid session chosen!\n");
					break;
				}

				printf("Choose  tutor to assign  session to:\n");
				showTutors();
				fgets(inputBuffer, sizeof(inputBuffer), stdin);
				sscanf(inputBuffer, "%d", &TutorNum);

				TutorNum--;
				if ((TutorNum < 0) || (TutorNum >= structCount("tutors.dat", sizeof(struct Tutor)))) {
					printf("Invalid tutor chosen!\n");
					break;
				}

				readstructure("sessions.dat", sizeof(struct Session), sessionIndex, &currentSession);
				readstructure("tutors.dat", sizeof(struct Tutor), TutorNum, &TutorLatest);

				assignSessionToTutor(&currentSession, &TutorLatest);

				updatestructures("sessions.dat", sizeof(struct Session), sessionIndex, &currentSession);
				updatestructures("tutors.dat", sizeof(struct Tutor), TutorNum, &TutorLatest);


				printf("Successfully assigned  session to  chosen Tutor\n");

				break;
			default:
				printf("Invalid command entered!");
				;
			};
		}
	}
}


void tutorMenu(struct Tutor* tutorP, int TutorNum) {
	int exit = 0;
	
	char inputBuffer[256];
	
	int userCommand = 0;
	while (!exit) {
		printf("1. show all sessions\n0. Exit\n");
		fgets(inputBuffer, sizeof(inputBuffer), stdin);
		if (sscanf(inputBuffer, "%d", &userCommand)) {

			switch (userCommand) {
			case 1:
				enrolledSessions(tutorP);
				break;
			case 0:
				exit = 1;
				break;
			default:
				printf("Invalid command entered.\n");
			}
		}
	}
}

int tutorLogin(const char* login, const char* password, struct Tutor* tutorP, int* TutorNum) {
	int counter = 0;
	FILE* fp = fopen("tutors.dat", "r+b");
	if (fp != NULL) {
		struct Tutor TutorLatest;
		while (fread(&TutorLatest, sizeof(struct Tutor), 1, fp) > 0) {
			if (strcmp(TutorLatest.tutorCode, login) == 0) {
				if (strcmp(TutorLatest.password, password) == 0) {
					memcpy(tutorP, &TutorLatest, sizeof(struct Tutor));
					fclose(fp);
					*TutorNum = counter;
					return 1;
				}
			}
			counter++;
		}
		fclose(fp);
		return 0;
	}
	else {
		fprintf(stderr, "Could not open  file in  function tutorLogin\n");
		return -1;
	}
}

void printTutorFields(struct Tutor* tP) {
	printf("|%-8s|%-10s|%-10s|%-10s\n", tP->tutorCode, tP->password, tP->name, tP->title);
	return;
};

void showTutors(void) {
	FILE* fp = fopen("tutors.dat", "r+b");
	if (fp != NULL) {
		struct Tutor TutorLatest;
		while (fread(&TutorLatest, sizeof(struct Tutor), 1, fp) > 0) {
			if (TutorLatest.tutorCode[0] == '\0') break;
			printTutorFields(&TutorLatest);
		}
		fclose(fp);
		return;
	}
	else {
		fprintf(stderr, "Could not open  file in  function showTutors\n");
	}
}

void createSessionTutor(struct Tutor* newTutorP) {
	FILE* fp = fopen("tutors.dat", "a+b");
	if (fp != NULL) {
		fwrite(newTutorP, sizeof(struct Tutor), 1, fp);
		fclose(fp);
		return;
	}
	else {
		fprintf(stderr, "Could not open  file in  function createSession\n");
	}
}
void studentMenu(struct Student* studentP, int StudentNum) {
	int exit = 0;
	char inputBuffer[256];
	int userCommand = 0;
	while (!exit) {
		printf("1. show all enrolled sessions\n2. show all sessions\n3. Enroll in a session\n4. Withdraw from a session\n0. Exit\n");
		fgets(inputBuffer, sizeof(inputBuffer), stdin);
		if (sscanf(inputBuffer, "%d", &userCommand)) {

			switch (userCommand) {
			case 1:
				enrolledSessions(studentP);
				break;

			case 2:
				allSessions();
				break;

			case 3:
				printf("Please choose a session to enroll in:\n");
				allSessions();
				char chosenSessionInput[4];
				int sessionIndex = 0;

				fgets(chosenSessionInput, 4, stdin);
				normalization(chosenSessionInput);
				sessionIndex = atoi(chosenSessionInput) - 1;
				if ((sessionIndex < 0) || (sessionIndex >= structCount("sessions.dat", sizeof(struct Session)))) {
					printf("Invalid session index!");
					continue;
				}

				struct Session sessionTemp;
				readstructure("sessions.dat", sizeof(struct Session), sessionIndex, &sessionTemp);

				if (isstudentinsession(studentP, &sessionTemp)) {
					printf("You are already in  session.\n");
					return;
				}
				studentEnroll(studentP, &sessionTemp);

				updatestructures("sessions.dat", sizeof(struct Session), sessionIndex, &sessionTemp);
				updatestructures("students.dat", sizeof(struct Student), StudentNum, studentP);

				break;

			case 4:
				printf("Please choose a session to withdraw from:\n");
				enrolledSessions(studentP);

				char* withdrawInput[4];
				int withdrawIndexChosen;
				int globalWithdrawIndex;
				struct Session currentSession;

				fgets(withdrawInput, sizeof(withdrawInput), stdin);
				sscanf(withdrawInput, "%d", &withdrawIndexChosen);

				globalWithdrawIndex = enrolledIndexToNormalIndex(withdrawIndexChosen, studentP);

				readstructure("sessions.dat", sizeof(struct Session), globalWithdrawIndex, &currentSession);

				studentRemove(studentP, &currentSession);

				updatestructures("sessions.dat", sizeof(struct Session), sessionIndex, &sessionTemp);
				updatestructures("students.dat", sizeof(struct Student), StudentNum, studentP);

				break; tutorLogin

			; case 0:
				exit = 1;
				break;
			default:
				printf("Invalid command entered.\n");
			}
		}
	}
}

int studentLogin(char* tp, char* password, struct Student* studentP, int* StudentNum) {
	int counter = 0;
	FILE* fp = fopen("students.dat", "r+b");
	if (fp != NULL) {
		struct Student StudentLatest;
		while (fread(&StudentLatest, sizeof(struct Student), 1, fp) > 0) {
			if (StudentLatest.tp[0] == '\0') break;
			if (strcmp(StudentLatest.tp, tp) == 0) {
				if (strcmp(StudentLatest.password, password) == 0) {
					memcpy(studentP, &StudentLatest, sizeof(struct Student));
					fclose(fp);
					*StudentNum = counter;
					return 1;
				}
			}
			counter++;
		}
		fclose(fp);
		return 0;
	}
	else {
		fprintf(stderr, "Could not open studentLogin\n");
		return -1;
	}
}

void studentEnroll(struct Student* studentP, struct Session* sessionP) {
	strcpy(sessionP->enrolledStudentTPs[sessionP->numEnrolledStudents], studentP->tp);
	sessionP->numEnrolledStudents++;
	strcpy(studentP->enrolledSessionCodes[studentP->numEnrolledSessions], sessionP->code);
	studentP->numEnrolledSessions++;
}

void studentRemove(struct Student* studentP, struct Session* sessionP) {
	for (int i = 0; i < sessionP->numEnrolledStudents; i++) {
		if (strcmp(studentP->tp, sessionP->enrolledStudentTPs[i]) == 0) {
			memmove(sessionP->enrolledStudentTPs[i], sessionP->enrolledStudentTPs[i + 1], (sessionP->numEnrolledStudents - i) * sizeof(studentP->tp));
			sessionP->numEnrolledStudents--;
			break;
		}
	}
	for (int i = 0; i < studentP->numEnrolledSessions; i++) {
		if (strcmp(sessionP->code, studentP->enrolledSessionCodes[i]) == 0) {
			memmove(studentP->enrolledSessionCodes[i], studentP->enrolledSessionCodes[i + 1], (studentP->numEnrolledSessions - i) * sizeof(sessionP->code));
			studentP->numEnrolledSessions--;
			break;
		}
	}
}
void getStudentName(const char* tp, char* nameBuffer) {
	FILE* fp = fopen("students.dat", "r+b");
	if (fp != NULL) {
		struct Student StudentLatest;
		while (fread(&StudentLatest, sizeof(struct Student), 1, fp) > 0) {
			if (StudentLatest.tp[0] == '\0') break;
			if (strcmp(tp, StudentLatest.tp) == 0) {
				strcpy(nameBuffer, StudentLatest.name);
				return;
			}
		}
		printf("TP not found");
	}
	else {
		fprintf(stderr, "Could not open getStudentName\n");
	}
}

void enrolledSessions(struct Student* sP) {
	int noEnrolledSessions = 1;
	FILE* fp = fopen("sessions.dat", "r+b");
	if (fp != NULL) {
		struct Session currentSession;
		int counter = 1;
		while (fread(&currentSession, sizeof(struct Session), 1, fp) > 0) {

			if (currentSession.code[0] == '\0') break;

			if (isstudentinsession(sP, &currentSession)) {
				noEnrolledSessions = 0;
				printf("-%d-", counter);
				outputSession(&currentSession);
				counter++;
			}
		}
		if (noEnrolledSessions) printf("You are not enrolled in any sessions\n");
		fclose(fp);
		return;
	}
	else {
		fprintf(stderr, "Could not open enrolledSessions\n");
	}
}
int enrolledIndexToNormalIndex(int enrolledIndex, struct Student* studentP) {
	FILE* fp = fopen("sessions.dat", "r+b");
	if (fp != NULL) {
		int globalIndex = 0;
		int enrolledCounter = 0;
		struct Session currentSession;
		while (fread(&currentSession, sizeof(struct Session), 1, fp) > 0) {
			if (isstudentinsession(studentP, &currentSession)) {
				enrolledCounter++;
			}
			if (enrolledCounter == enrolledIndex) {
				return globalIndex;
			}
			globalIndex++;
		}
		fprintf(stderr, "enrolledIndex is invalid\n");
		return -1;
	}
	else {
		fprintf(stderr, "Could not open enrolledIndexToNormalIndex\n");
		return -1;
	}
}

void outputStudents(struct Student* sP) {
	printf("|%-16s|%-16s|%-16s\n", sP->tp, sP->name, sP->password);
	return;
}

void showStudents(void) {
	FILE* fp = fopen("students.dat", "r+b");
	if (fp != NULL) {
		struct Student StudentLatest;
		while (fread(&StudentLatest, sizeof(struct Student), 1, fp) > 0) {
			outputStudents(&StudentLatest);
		}
		fclose(fp);
		return;
	}
	else {
		fprintf(stderr, "Could not open showStudents\n");
	}
}

void createStudent(struct Student* newStudentP) {
	FILE* fp = fopen("students.dat", "a+b");
	if (fp != NULL) {
		fwrite(newStudentP, sizeof(struct Student), 1, fp);
		fclose(fp);
		return;
	}
	else {
		fprintf(stderr, "Could not open createStudent\n");
	}
}
int hasStudent(struct Session* sessionP, struct Student* studentP) {
	for (int i = 0; i < studentP->numEnrolledSessions; i++) {
		if (strcmp(sessionP->code, studentP->enrolledSessionCodes[i]) == 0) {
			return 1;
		}
	}
	return 0;
}

void outputSession(struct Session* sP) {
	printf("|%-16s|%-26s|%-16s|%-16s|%-16s|%-16s\n", sP->code, sP->title, sP->day, sP->time, sP->location, sP->tutorCode);
}


void allSessions(void) {
	FILE* fp = fopen("sessions.dat", "r+b");
	if (fp != NULL) {
		int counter = 1;
		struct Session currentSession;
		while (fread(&currentSession, sizeof(struct Session), 1, fp) > 0) {
			printf("-%d-", counter);
			outputSession(&currentSession);
			counter++;
		}
		fclose(fp);
		return;
	}
	else {
		fprintf(stderr, "Could not open showSessions\n");
	}
}

void sessionsWithStudents(void) {
	FILE* fp = fopen("sessions.dat", "r+b");
	if (fp != NULL) {
		int counter = 1;
		char nameBuffer[32];
		struct Session currentSession;
		while (fread(&currentSession, sizeof(struct Session), 1, fp) > 0) {
			printf("%d", ftell(fp));

			printf("-%d-", counter);
			outputSession(&currentSession);
			counter++;
			printf("Participants:\n");
			for (int i = 0; i < currentSession.numEnrolledStudents; i++) {
				getStudentName(currentSession.enrolledStudentTPs[i], nameBuffer);
				printf("   %-8s - %-14s\n", currentSession.enrolledStudentTPs[i], nameBuffer);
			}
		}
	}
	else {
		fprintf(stderr, "Could not open sessionsWithStudents\n");
	}
}

void createSession(struct Session* newSessionP) {
	FILE* fp = fopen("sessions.dat", "a+b");
	if (fp != NULL) {
		fwrite(newSessionP, sizeof(struct Session), 1, fp);
		fclose(fp);
		return;
	}
	else {
		fprintf(stderr, "Could not open createSession\n");
	}
}
void assignSessionToTutor(struct Session* sessionP, struct Tutor* tutorP) {
	if (sessionP->tutorCode[0] != '\0') {
		printf("Session is assigned to a tutor.\n");
		return;
	}
	strncpy(sessionP->tutorCode, tutorP->tutorCode, sizeof(sessionP->tutorCode) - 1);
	printf("Session '%s' assigned to tutor '%s'.\n", sessionP->code, tutorP->name);
}
