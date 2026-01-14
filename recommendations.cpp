#define _CRT_SECURE_NO_WARNINGS
#include "recommendations.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "books.h"

// Create a circular linked list of top N rated books
RecNode* createRecommendationList(book books[], int count, int topN) {

    // CREATE ARRAY OF POINTERS
    book* sorted[MAX_BOOKS];
    for (int i = 0; i < count; i++) {
        sorted[i] = &books[i];
    }

    // SORT POINTERS BY AVERAGE RATING (DESCENDING) 
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (sorted[j]->average_rating < sorted[j + 1]->average_rating) {
                book* temp = sorted[j];
                sorted[j] = sorted[j + 1];
                sorted[j + 1] = temp;
            }
        }
    }

    // BUILD CIRCULAR LINKED LIST FROM SORTED POINTERS 
    RecNode* head = NULL;
    RecNode* prev = NULL;
    int added = 0;

    for (int i = 0; i < count && added < topN; i++) {
        if (sorted[i]->available == 0) continue; // skip borrowed books

        RecNode* node = (RecNode*)malloc(sizeof(RecNode));
        node->data = sorted[i];
        node->next = NULL;

        if (!head)
            head = node;
        if (prev)
            prev->next = node;

        prev = node;
        added++;
    }

    if (prev)
        prev->next = head; // make circular

    return head;
}

// Display N books starting from head
void displayRecommendations(RecNode* head, int n) {
    if (!head) {
        printf("No recommendations available.\n");
        return;
    }

    RecNode* current = head;
    for (int i = 0; i < n; i++) {
        printf("%d. %s | %s | Rating: %.2f\n",
            i + 1,
            current->data->title,
            current->data->authors,
            current->data->average_rating);
        current = current->next;
    }
}

// Move to next recommendation in the circular list
RecNode* nextRecommendation(RecNode* current) {
    if (!current) return NULL;
    return current->next;
}

void showTopRecommendations(book* books, int count) {
    int topN = 5;
    int availableCount = 0;

    // Count available books
    for (int i = 0; i < count; i++)
        if (books[i].available) availableCount++;

    if (availableCount == 0) {
        printf("No books available for recommendations.\n");
        return;
    }

    // Create sorted array of pointers
    book* sorted[MAX_BOOKS];
    for (int i = 0; i < count; i++) sorted[i] = &books[i];

    // Bubble sort by average_rating descending
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (sorted[j]->average_rating < sorted[j + 1]->average_rating) {
                book* temp = sorted[j];
                sorted[j] = sorted[j + 1];
                sorted[j + 1] = temp;
            }
        }
    }

    // --- Jump Search Implementation ---
    int blockSize = (int)sqrt(count); // optimal jump size
    int index = 0;
    int shown = 0;
    int startIndex = 5; // skip first 5 as before
    printf("Top Rated Recommendations: \n");
    index = startIndex;
    while (shown < topN && index < count) {
        // Jump ahead by blockSize
        int nextIndex = index + blockSize;
        if (nextIndex > count) nextIndex = count;

        // Linear search inside the block
        for (int i = index; i < nextIndex && shown < topN; i++) {
            if (sorted[i]->available) {
                printf("%d. %s | %s | Pages: %d | Year: %d\n",
                    shown + 1,
                    sorted[i]->title,
                    sorted[i]->authors,
                    sorted[i]->num_pages,
                    sorted[i]->published_year);
                shown++;
            }
        }
        index = nextIndex;
    }

    // Show next books one by one
    char choice;
    while (index < count) {
        printf("Press n for next recommendation, 0 to quit: ");
        scanf(" %c", &choice);

        if (choice == '0') break;
        if (choice == 'n') {
            // Skip unavailable
            while (index < count && !sorted[index]->available) index++;
            if (index >= count) break;

            printf("Recommendation: %s | %s | Pages: %d | Year: %d\n",
                sorted[index]->title,
                sorted[index]->authors,
                sorted[index]->num_pages,
                sorted[index]->published_year);
            index++;
        }
    }

    if (index >= count) {
        printf("No more recommendations.\n");
    }
}