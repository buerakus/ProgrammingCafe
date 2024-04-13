#pragma once

struct Session {
	char code[32];
	char title[32];
	char day[32];
	char time[32]; // should strictly adhere to the format
	char location[32];
	char tutorCode[32];
	char enrolledStudentTPs[64][32];
	int numEnrolledStudents;
};

struct Student {
	char name[32];
	char tp[32];
	char password[32];
	char enrolledSessionCodes[64][32];
	int numEnrolledSessions;
};

struct Tutor {
	// These are all formatted
	char tutorCode[32];
	char password[32];
	char name[32];
	char title[32];
};
