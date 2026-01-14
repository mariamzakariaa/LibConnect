#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "books.h"
#include "utils.h"
#include "students.h"

#define LINE_SIZE 8192 // Maximum length of a single CSV line 

extern int currentUser;
extern int borrowedCount[MAX_USERS];
extern int borrowedBooks[MAX_USERS][MAX_BORROWED];
extern char studentIDs[STUDENT_COUNT][MAX_ID_LEN];

// Parses one CSV field, supports quoted fields
void parseField(char** ptr, char* buffer, int size) {
    int i = 0;
    // Skip leading spaces
    while (**ptr == ' ') (*ptr)++;

    if (**ptr == '"') { // quoted field
        (*ptr)++;
        while (**ptr) {
            if (**ptr == '"' && (*(*ptr + 1) == ',' || *(*ptr + 1) == '\0' || *(*ptr + 1) == '\n' || *(*ptr + 1) == '\r')) {
                (*ptr)++;
                break;
            }
            if (i < size - 1) buffer[i++] = **ptr;
            (*ptr)++;
        }
        buffer[i] = '\0';
        if (**ptr == ',') (*ptr)++;
    }
    else { // unquoted
        while (**ptr && **ptr != ',' && **ptr != '\n' && **ptr != '\r') {
            if (i < size - 1) buffer[i++] = **ptr;
            (*ptr)++;
        }
        buffer[i] = '\0';
        if (**ptr == ',') (*ptr)++;
    }

    // Trim trailing spaces
    int len = strlen(buffer);
    while (len > 0 && (buffer[len - 1] == ' ' || buffer[len - 1] == '\t')) {
        buffer[len - 1] = '\0';
        len--;
    }
}

// Load books from CSV
void loadBooks(book books[], int* count) {
    FILE* file = fopen("books.csv", "r");
    if (!file) { printf("Error opening books.csv\n"); return; }

    char line[LINE_SIZE];
    *count = 0;

    // Skip header
    if (!fgets(line, sizeof(line), file)) { fclose(file); return; }

    while (*count < MAX_BOOKS && fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\r\n")] = 0;
        char* ptr = line;
        char buffer[DESC_SIZE];
        int col = 0;

        // Initialize numeric fields to safe defaults
        books[*count].published_year = 0;
        books[*count].average_rating = 0.0f;
        books[*count].num_pages = 0;
        books[*count].ratings_count = 0;
        books[*count].available = 1;

        while (*ptr) {
            parseField(&ptr, buffer, DESC_SIZE);
            switch (col) {
            case 0: strncpy(books[*count].isbn13, buffer, ISBN_SIZE - 1); break;
            case 1: strncpy(books[*count].title, buffer, TITLE_SIZE - 1); break;
            case 4: strncpy(books[*count].authors, buffer, AUTHOR_SIZE - 1); break;
            case 5: strncpy(books[*count].categories, buffer, CAT_SIZE - 1); break;
            case 7: strncpy(books[*count].description, buffer, DESC_SIZE - 1); break;
            case 8: books[*count].published_year = buffer[0] ? atoi(buffer) : 0; break;
            case 9: books[*count].average_rating = buffer[0] ? atof(buffer) : 0.0f; break;
            case 10: books[*count].num_pages = buffer[0] ? atoi(buffer) : 0; break;
            case 11: books[*count].ratings_count = buffer[0] ? atoi(buffer) : 0; break;
            case 12: books[*count].available = buffer[0] ? atoi(buffer) : 1; break;
            default: break;
            }
            col++;
        }

        (*count)++;
    }

    fclose(file);
}

// Save books to CSV
void saveBooks(book books[], int count, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) { printf("Error opening file for writing\n"); return; }

    fprintf(file, "isbn13,title,unused1,unused2,authors,categories,unused3,description,published_year,average_rating,num_pages,ratings_count,available\n");

    for (int i = 0; i < count; i++) {
        fprintf(file, "\"%s\",\"%s\",,,\"%s\",\"%s\",,\"%s\",%d,%.2f,%d,%d,%d\n",
            books[i].isbn13,
            books[i].title,
            books[i].authors,
            books[i].categories,
            books[i].description,
            books[i].published_year,
            books[i].average_rating,
            books[i].num_pages,
            books[i].ratings_count,
            books[i].available
        );
    }

    fclose(file);
}

// Save borrow
void saveBorrow(const char* encryptedID, const char* isbn) {
    char decryptedID[MAX_ID_LEN];
    decryptID(encryptedID, decryptedID);
    FILE* f = fopen("borrowed.csv", "a");
    if (!f) return;
    fprintf(f, "%s,%s\n", decryptedID, isbn);
    fclose(f);
}

// Load borrowed books
void loadUserBorrows(const char* encryptedID, book books[], int bookCount) {
    char decryptedID[MAX_ID_LEN];
    decryptID(encryptedID, decryptedID);
    FILE* f = fopen("borrowed.csv", "r");
    if (!f) return;

    char fileID[MAX_ID_LEN], isbn[20];
    borrowedCount[currentUser] = 0;

    while (fscanf(f, "%[^,],%s\n", fileID, isbn) == 2) {
        if (strcmp(fileID, decryptedID) == 0) {
            int idx = findBookByISBN(books, bookCount, isbn);
            if (idx != -1) {
                borrowedBooks[currentUser][borrowedCount[currentUser]++] = idx;
                books[idx].available = 0;
            }
        }
    }
    fclose(f);
}

// Remove borrow record
void removeBorrow(const char* encryptedID, const char* isbn) {
    char decryptedID[MAX_ID_LEN];
    decryptID(encryptedID, decryptedID);

    FILE* f = fopen("borrowed.csv", "r");
    if (!f) return;
    FILE* temp = fopen("temp.csv", "w");
    if (!temp) { fclose(f); return; }

    char fileID[MAX_ID_LEN], fileIsbn[20];
    while (fscanf(f, "%[^,],%s\n", fileID, fileIsbn) == 2) {
        if (!(strcmp(fileID, decryptedID) == 0 && strcmp(fileIsbn, isbn) == 0)) {
            fprintf(temp, "%s,%s\n", fileID, fileIsbn);
        }
    }
    fclose(f); fclose(temp);
    remove("borrowed.csv");
    rename("temp.csv", "borrowed.csv");
}

// Borrow book
void handleBorrow(book books[], int matches[], int matchCount) {
    if (matchCount == 0) return;
    int choice;
    if (matchCount == 1) {
        if (!books[matches[0]].available) { printf("This book is already borrowed.\n"); return; }
        while (1) {
            printf("\nBook found: %s\n", books[matches[0]].title);
            printf("1. Borrow\n2. Keep\nChoose: ");
            scanf("%d", &choice);
            if (choice == 2) return;
            if (choice == 1) {
                books[matches[0]].available = 0;
                borrowedBooks[currentUser][borrowedCount[currentUser]++] = matches[0];
                saveBorrow(studentIDs[currentUser], books[matches[0]].isbn13);
                printf("Book borrowed successfully.\n");
                return;
            }
            printf("Invalid choice. Try again.\n");
        }
    }

    while (1) {
        printf("\nBooks found:\n");
        for (int i = 0; i < matchCount; i++) {
            printf("%d. %s | %s | %s\n", i + 1, books[matches[i]].title, books[matches[i]].authors, books[matches[i]].available ? "Available" : "Borrowed");
        }
        printf("0. Cancel\nChoose a book number to borrow: ");
        scanf("%d", &choice);
        if (choice == 0) return;
        if (choice<1 || choice>matchCount) { printf("Invalid choice.\n"); continue; }
        int idx = matches[choice - 1];
        if (!books[idx].available) { printf("Already borrowed.\n"); continue; }
        books[idx].available = 0;
        borrowedBooks[currentUser][borrowedCount[currentUser]++] = idx;
        saveBorrow(studentIDs[currentUser], books[idx].isbn13);
        printf("Book borrowed successfully.\n");
        return;
    }
}

// Show borrowed books
void showBorrowedBooks(book books[]) {
    printf("\nYour borrowed books:\n");
    if (borrowedCount[currentUser] == 0) { printf("None\n"); return; }
    for (int i = 0; i < borrowedCount[currentUser]; i++) {
        int idx = borrowedBooks[currentUser][i];
        printf("%d. %s\n", i + 1, books[idx].title);
    }
}

// Return book
void returnBook(book books[], int totalBooks) {
    if (borrowedCount[currentUser] == 0) { printf("You have no borrowed books.\n"); return; }
    showBorrowedBooks(books);
    int choice;
    printf("Enter number to return (0 to cancel): ");
    scanf("%d", &choice);
    if (choice <= 0 || choice > borrowedCount[currentUser]) return;
    int col = choice - 1;
    int idx = borrowedBooks[currentUser][col];
    books[idx].available = 1;
    removeBorrow(studentIDs[currentUser], books[idx].isbn13);
    for (int i = col; i < borrowedCount[currentUser] - 1; i++)
        borrowedBooks[currentUser][i] = borrowedBooks[currentUser][i + 1];
    borrowedCount[currentUser]--;
    printf("Book returned successfully.\n");
}
