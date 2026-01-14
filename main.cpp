#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "books.h"
#include "search.h"
#include "students.h"
#include "utils.h"
#include "recommendations.h"


// Global variables for user session and borrowed books

int currentUser = 0; // tracks logged in student
int borrowedCount[MAX_USERS] = { 0 }; // stores count of books borrowed per user
int borrowedBooks[MAX_USERS][MAX_BORROWED] = { 0 }; // stores indices of borrowed books per user

void showMenu(book* books, int count) {
    int choice;
    printf("\nSearch Menu:\n");
    printf("1. Search by Book Title\n");
    printf("2. Search by Author Name\n");
    printf("3. Search by Category\n");
    printf("4. Search by Published Year\n");
    printf("5. Search by Minimum Rating\n");
    printf("6. View Borrowed Books\n");
    printf("7. Return a Book\n");
    printf("8. View Top Recommendations\n");
    printf("0. Exit\n");
    printf("Enter choice: ");
    scanf("%d", &choice);

    switch (choice) {
    case 1:
        searchByTitle(books, count);
        break;
    case 2:
        searchByAuthor(books, count);
        break;
    case 3:
        searchByCategory(books, count);
        break;
    case 4:
        searchByYear(books, count);
        break;
    case 5:
        searchByRating(books, count);
        break;
    case 6:
        showBorrowedBooks(books);
        break;
    case 7:
        returnBook(books, count);
        break;
    case 8:
        showTopRecommendations(books, count);
        break;
    case 0:
        saveBooks(books, count, "books.csv");
        printf("Exiting...\n");
        return; // exit recursion
    default:
        printf("Invalid choice! Try again.\n");
        break;
    }

    showMenu(books, count); // recurse back to menu
}


int main() {
    initStudentIDs(); 
    char enteredID[MAX_ID_LEN];
    printf("Enter your student ID: ");
    scanf("%s", enteredID);

    int idx = login(enteredID);
    if (idx == -1) {
        printf("Access denied. Invalid student ID.\n");
        return 0;
    }

    printf("Login successful. Welcome!\n");

    // Allocate memory for books and load CSV
    int count = 0;
    book* books = (book*)malloc(MAX_BOOKS * sizeof(book));
    if (books == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }
    loadBooks(books, &count);
    loadUserBorrows(studentIDs[currentUser], books, count);

    // Main menu loop for searching, borrowing, returning books
    showMenu(books, count);

    // Free allocated memory and exit
    free(books);
    return 0;
}
