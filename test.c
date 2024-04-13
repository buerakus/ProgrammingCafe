#define _CRT_SECURE_NO_WARNINGS
#include "functions.h"
#include "structs.h"
#include <stdio.h>
#include <string.h>

int main() {

    struct Session session1 = { "PYP101", "Python Programming", "Saturday", "9.00am", "C-01-01", "T01", "TP080080", 1 };
    struct Session session2 = { "JAV102", "Java Programming", "Sunday", "9.00am", "C-01-02", "T02", "TP031031", 1 };
    struct Session session3 = { "CPL103", "C Programming", "Saturday", "2.00pm","C-01-03", "T03", "TP071054", 1 };
    struct Session session4 = { "WEB104", "Web Development", "Sunday", "2.00pm", "C-01-04", "T04" };
    struct Session session5 = { "CSP105", "C Sharp Programming", "Monday","7.00pm", "C-01-05","T05" };

    struct Tutor tutor1 = { "T01","123","Albert","Web Development" };
    struct Tutor tutor2 = { "T02", "124", "Amad", "C Sharp Programming" };
    struct Tutor tutor3 = { "T03", "125", "Steve", "Python Programming" };
    addTutor(&tutor1);
    addTutor(&tutor2);
    addTutor(&tutor3);

    struct Student student1 = { "Timur", "TP071054", "12345", "CPL103", 1 };
    struct Student student2 = { "Coal", "TP031031", "123456", "JAV102", 1 };
    struct Student student3 = { "Hwabag", "TP080080", "1234567", "PYP101", 1 };

    addSession(&session1);
    addSession(&session2);
    addSession(&session3);
    addSession(&session4);
    addSession(&session5);

    addStudent(&student1);
    addStudent(&student2);
    addStudent(&student3);

}