#ifndef RECOMMENDATIONS_H
#define RECOMMENDATIONS_H

#include "books.h"

typedef struct RecNode {
    book* data;
    struct RecNode* next;
} RecNode;

RecNode* createRecommendationList(book books[], int count, int topN);
void displayRecommendations(RecNode* head, int n);
RecNode* nextRecommendation(RecNode* current);
void showTopRecommendations(book* books, int count);

#endif