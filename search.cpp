#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "books.h"
#include "search.h"
#include "utils.h"
#include "students.h"

// Helper: checks if substr exists in str ignoring case
static int containsIgnoreCase(const char* str, const char* substr) {
    char s1[260], s2[260];
    strncpy(s1, str, 259); s1[259] = '\0';
    strncpy(s2, substr, 259); s2[259] = '\0';
    toLowerStr(s1);
    toLowerStr(s2);
    return strstr(s1, s2) != NULL;
}

// Searching books: 
// Search for books by title and call borrow handler
void searchByTitle(book books[], int count) {
    char title[260];
    int* matches = (int*)malloc(count * sizeof(int));
    int matchCount = 0;

    if (!matches) {
        printf("Memory allocation failed.\n");
        return;
    }

    printf("Enter book title: ");
    scanf(" %259[^\n]", title);
    cleanString(title);

    for (int i = 0; i < count; i++) {
        if (containsIgnoreCase(books[i].title, title)) {
            matches[matchCount++] = i;
        }
    }

    if (matchCount == 0) {
        printf("No books found.\n");
        return;
    }
    handleBorrow(books, matches, matchCount);
    free(matches);
}

// Search books by author (supports multiple authors separated by ;
void searchByAuthor(book books[], int count) {
    char author[260];
    int* matches = (int*)malloc(count * sizeof(int));
    int matchCount = 0;

    if (!matches) {
        printf("Memory allocation failed.\n");
        return;
    }
    printf("Enter author name: ");
    scanf(" %259[^\n]", author);
    cleanString(author);

    for (int i = 0; i < count; i++) {
        char authorsCopy[AUTHOR_SIZE];
        strncpy(authorsCopy, books[i].authors, AUTHOR_SIZE - 1);
        authorsCopy[AUTHOR_SIZE - 1] = '\0';

        char* token = strtok(authorsCopy, ";");
        while (token) {
            cleanString(token);
            if (containsIgnoreCase(token, author)) {
                matches[matchCount++] = i;
                break;
            }
            token = strtok(NULL, ";");
        }
    }

    if (matchCount == 0)
        printf("No books found by author '%s'\n", author);
    else
        handleBorrow(books, matches, matchCount);
    free(matches);
}

// Search books by category (supports multiple categories)
void searchByCategory(book books[], int count) {
    char category[260];
    int* matches = (int*)malloc(count * sizeof(int));
    int matchCount = 0;

    if (!matches) {
        printf("Memory allocation failed.\n");
        return;
    }
    printf("Enter category: ");
    scanf(" %259[^\n]", category);
    cleanString(category);

    for (int i = 0; i < count; i++) {
        char categoriesCopy[CAT_SIZE];
        strncpy(categoriesCopy, books[i].categories, CAT_SIZE - 1);
        categoriesCopy[CAT_SIZE - 1] = '\0';

        char* token = strtok(categoriesCopy, ";");
        while (token) {
            cleanString(token);
            if (containsIgnoreCase(token, category)) {
                matches[matchCount++] = i;
                break;
            }
            token = strtok(NULL, ";");
        }
    }

    if (matchCount == 0)
        printf("No books found in category '%s'\n", category);
    else
        handleBorrow(books, matches, matchCount);
    free(matches);
}

// Search books by exact published year
void searchByYear(book books[], int count) {
    int year;
    int* matches = (int*)malloc(count * sizeof(int));
    int matchCount = 0;

    if (!matches) {
        printf("Memory allocation failed.\n");
        return;
    }
    printf("Enter published year to search: ");
    scanf("%d", &year);

    for (int i = 0; i < count; i++) {
        if (books[i].published_year == year) {
            matches[matchCount++] = i;
        }
    }

    if (matchCount == 0)
        printf("No books found published in %d\n", year);
    else
        handleBorrow(books, matches, matchCount);
    free(matches);
}

// Search books by minimum rating threshold
void searchByRating(book books[], int count) {
    float rating;
    int* matches = (int*)malloc(count * sizeof(int));
    int matchCount = 0;

    if (!matches) {
        printf("Memory allocation failed.\n");
        return;
    }

    printf("Enter minimum rating to search: ");
    scanf("%f", &rating);

    for (int i = 0; i < count; i++) {
        if (books[i].average_rating >= rating) {
            matches[matchCount++] = i;
        }
    }

    if (matchCount == 0)
        printf("No books found with rating >= %.2f\n", rating);
    else
        handleBorrow(books, matches, matchCount);

    free(matches);
}

