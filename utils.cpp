#define _CRT_SECURE_NO_WARNINGS
#include "utils.h"
#include "books.h"
#include <string.h>
#include <ctype.h>

#define MAX_ID_LEN 20

// Clean a string by removing leading/trailing spaces, quotes, and newlines
void cleanString(char* str) {
    if (!str) return;

    // remove leading spaces and quotes
    while (*str == ' ' || *str == '"') {
        memmove(str, str + 1, strlen(str));
    }

    // remove trailing spaces, quotes, newline
    int len = strlen(str);
    while (len > 0 &&
        (str[len - 1] == ' ' ||
            str[len - 1] == '"' ||
            str[len - 1] == '\n' ||
            str[len - 1] == '\r')) {
        str[--len] = '\0';
    }
}

// Convert entire string to lowercase (for case-insensitive comparisons)
void toLowerStr(char* s) {
    for (; *s; s++) *s = (char)tolower(*s);
}


//Encrypt a student ID
void encryptID(const char id[], char encrypted[]) {
    char last3[4], first3[4], middle[4];
    char temp[20] = "";

    // Extract manually
    last3[0] = id[6];
    last3[1] = id[7];
    last3[2] = id[8];
    last3[3] = '\0';

    first3[0] = id[0];
    first3[1] = id[1];
    first3[2] = id[2];
    first3[3] = '\0';

    middle[0] = id[3];
    middle[1] = id[4];
    middle[2] = id[5];
    middle[3] = '\0';

    // Concatenate: last3 + first3 + middle
    strcpy(temp, last3);
    strcat(temp, first3);
    strcat(temp, middle);

    // Add # after 4th character
    encrypted[0] = temp[0];
    encrypted[1] = temp[1];
    encrypted[2] = temp[2];
    encrypted[3] = temp[3];
    encrypted[4] = '\0';

    strcat(encrypted, "#");
    strcat(encrypted, temp + 4);
}

// Decrypt a student ID (for loading or saving borrowed books)
void decryptID(const char encrypted[], char decrypted[]) {
    char temp[20] = "";
    char last3[4], first3[4], middle[4];

    // Remove #
    temp[0] = encrypted[0];
    temp[1] = encrypted[1];
    temp[2] = encrypted[2];
    temp[3] = encrypted[3];
    temp[4] = '\0';

    strcat(temp, encrypted + 5);

    // Extract parts
    last3[0] = temp[0];
    last3[1] = temp[1];
    last3[2] = temp[2];
    last3[3] = '\0';

    first3[0] = temp[3];
    first3[1] = temp[4];
    first3[2] = temp[5];
    first3[3] = '\0';

    middle[0] = temp[6];
    middle[1] = temp[7];
    middle[2] = temp[8];
    middle[3] = '\0';

    // Rebuild original ID
    strcpy(decrypted, first3);
    strcat(decrypted, middle);
    strcat(decrypted, last3);
}

// Find a book in the array by ISBN (for borrowed book management functions)
int findBookByISBN(book books[], int count, const char* isbn) {
    for (int i = 0; i < count; i++) {
        if (strcmp(books[i].isbn13, isbn) == 0)
            return i;
    }
    return -1;
}

