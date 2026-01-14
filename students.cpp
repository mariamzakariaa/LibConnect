#define _CRT_SECURE_NO_WARNINGS
#include "students.h"
#include "utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

extern int currentUser;
char studentIDs[STUDENT_COUNT][MAX_ID_LEN]; // encrypted IDs

// integer array for binary search
int studentIDInts[STUDENT_COUNT] = {
    232004188, 241004496, 241004719, 241005347, 241005807,
    241006483, 241007044, 241007108, 241007160, 241007332,
    241007337, 241007800, 241014050, 241014101, 241014107,
    241014108, 241014212, 241014235, 241014415, 241014446,
    241017109, 241004491, 241004541, 241004855, 241005581,
    241006244, 241006956
};

// Sorts decrypted integer IDs in ascending order
void sortDecrypted(int arr[], int size) {
    int temp;
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

// Encrypts all student IDs and stores them in the global array
void initStudentIDs(void) {
    // Encrypt all IDs
    encryptID("232004188", studentIDs[0]);
    encryptID("241004496", studentIDs[1]);
    encryptID("241004719", studentIDs[2]);
    encryptID("241005347", studentIDs[3]);
    encryptID("241005807", studentIDs[4]);
    encryptID("241006483", studentIDs[5]);
    encryptID("241007044", studentIDs[6]);
    encryptID("241007108", studentIDs[7]);
    encryptID("241007160", studentIDs[8]);
    encryptID("241007332", studentIDs[9]);
    encryptID("241007337", studentIDs[10]);
    encryptID("241007800", studentIDs[11]);
    encryptID("241014050", studentIDs[12]);
    encryptID("241014101", studentIDs[13]);
    encryptID("241014107", studentIDs[14]);
    encryptID("241014108", studentIDs[15]);
    encryptID("241014212", studentIDs[16]);
    encryptID("241014235", studentIDs[17]);
    encryptID("241014415", studentIDs[18]);
    encryptID("241014446", studentIDs[19]);
    encryptID("241017109", studentIDs[20]);
    encryptID("241004491", studentIDs[21]);
    encryptID("241004541", studentIDs[22]);
    encryptID("241004855", studentIDs[23]);
    encryptID("241005581", studentIDs[24]);
    encryptID("241006244", studentIDs[25]);
    encryptID("241006956", studentIDs[26]);

    // Sort the integer array (for binary search)
    sortDecrypted(studentIDInts, STUDENT_COUNT);
}
// Binary search for integer ID
int binarySearch(int arr[], int size, int key) {
    int start = 0, end = size - 1;
    while (start <= end) {
        int mid = start + (end - start) / 2;
        if (arr[mid] == key) return mid;
        else if (arr[mid] < key) start = mid + 1;
        else end = mid - 1;
    }
    return -1;
}
// Returns index of logged - in student if successful, -1 if invalid
int login(const char* enteredID) {
    int enteredInt = atoi(enteredID);          // convert input to int
    int idx = binarySearch(studentIDInts, STUDENT_COUNT, enteredInt); // use already sorted ints
    if (idx != -1) {
        currentUser = idx;
        return idx; // success
    }
    return -1; // failed
}
