#ifndef STUDENTS_H
#define STUDENTS_H

#define STUDENT_COUNT 27
#define MAX_ID_LEN 20
#include "utils.h"
#include <stdlib.h>

void initStudentIDs(void);    
void sortDecrypted(int arr[], int size);
int binarySearch(int arr[], int size, int key);
int login(const char* enteredID);

extern char studentIDs[STUDENT_COUNT][MAX_ID_LEN];  
extern int studentIDInts[STUDENT_COUNT];       
extern int studentIDInts[STUDENT_COUNT];
#endif
