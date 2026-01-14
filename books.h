#ifndef BOOKS_H
#define BOOKS_H
#include "utils.h"
#define ISBN_SIZE 20
#define TITLE_SIZE 260
#define AUTHOR_SIZE 260
#define CAT_SIZE 130
#define DESC_SIZE 3000
#define MAX_BOOKS 8000

struct book {
    char isbn13[ISBN_SIZE];
    char title[TITLE_SIZE];
    char authors[AUTHOR_SIZE];
    char categories[CAT_SIZE];
    char description[DESC_SIZE];
    int published_year;
    float average_rating;
    int num_pages;
    int ratings_count;

    int available;   // 1 = available, 0 = borrowed
};

void parseField(char** ptr, char* buffer, int size);
void loadBooks(book books[], int* count);
void saveBooks(book books[], int count, const char* filename);
void saveBorrow(const char* studentID, const char* isbn);
void loadUserBorrows(const char* encryptedID, book books[], int bookCount);
void removeBorrow(const char* studentID, const char* isbn);
void handleBorrow(book books[], int matches[], int matchCount);
void showBorrowedBooks(book books[]);
void returnBook(book books[], int totalBooks);

#endif // BOOKS_H
