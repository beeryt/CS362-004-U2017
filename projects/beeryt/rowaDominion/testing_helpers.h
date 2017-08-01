#ifndef _TESTING_HELPERS_H
#define _TESTING_HELPERS_H

#include "./dominion.h"

typedef enum { FAILED=0, PASSED } Status;

struct StatusTracker {
	Status _overallStatus;
	Status* _statuses;
	char** _statusReasons;
	int _capacity;
	int numTests;

	char** _contexts;
	int* _contextIndexToStatusIndex;
	int _contextCapacity;
	int numContexts;
};

void initStatusTracker(struct StatusTracker*);
void destroyStatusTracker(struct StatusTracker*);
void addContextToTracker(char*, struct StatusTracker*);
int assertTrue(int, char*, struct StatusTracker*);
int assertEqual(char*, int, int, struct StatusTracker*);
void failTest(struct StatusTracker*, char*);
void passTest(struct StatusTracker*, char*);
void printTestResults(struct StatusTracker);
void printFailedTestResults(struct StatusTracker);

void initializeTestGame(int, const int[][MAX_HAND], const int*, const int[][MAX_DECK], const int*, const int*, int, int, struct gameState*);
void initializeRandomState(struct gameState*);
int findInHand(int, struct gameState *);
int numCardIn(const int, const int*, const int);
int fullDeckValue(int, struct gameState);
int onlyGainedCardNumTimes(const int, const int, const int*, const int, const int*, const int); 
int sameCompositionOfCards(const int*, const int*, const int);
int sameOrderedCollectionOfCards(const int*, const int*, const int);
int treasureInHand(struct gameState, int);
int treasureInDeck(struct gameState, int);
int treasure(int);

#endif
