#ifndef UTILS_H
#define UTILS_H
#define MAX_ID_LEN 20
#define MAX_USERS 50
#define MAX_BORROWED 20
#include <string.h>
#include <ctype.h>

struct book;
void cleanString(char* str);
void toLowerStr(char* s);
void encryptID(const char id[], char encrypted[]);
void decryptID(const char encrypted[], char decrypted[]);
int findBookByISBN(book books[], int count, const char* isbn);


#endif
