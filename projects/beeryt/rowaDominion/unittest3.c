#include "./dominion.h"
#include "./dominion_helpers.h"
#include "./testing_helpers.h"
#include <stdlib.h>
#include <stdio.h>

int main() {
	const int chosenKingdomCards[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse, sea_hag, tribute, smithy};
	const int playerInitialHands[][MAX_HAND] = { { smithy, copper, copper, copper, copper  }, { gardens, sea_hag, tribute } , { } };
   	const int playerInitialHandSizes[] = { 5, 3, 0 };
	const int playerInitialDecks[][MAX_DECK] = { { gardens, embargo, village, minion }, { } };
	const int playerInitialDeckSizes[] = { 4, 0 };
	
	struct gameState state;
	struct StatusTracker tracker;

	initStatusTracker(&tracker);
	initializeTestGame(3, playerInitialHands, playerInitialHandSizes, playerInitialDecks, playerInitialDeckSizes, chosenKingdomCards, 10, 1, &state); 
	
	printf("----numHandCards Function Test----\n");

	addContextToTracker("Basic test that function returns number of cards in current player's hand.", &tracker);
	assertTrue(numHandCards(&state) == 5, "Should return number of cards in first player's hand.", &tracker);
	state.whoseTurn++;
	assertTrue(numHandCards(&state) == 3, "After moving to next player, should return number of cards in second player's hand.", &tracker);
	state.whoseTurn++;
	
	addContextToTracker("Test for player with no cards in hand.", &tracker);
	assertTrue(numHandCards(&state) == 0, "Should return 0 found in player's hand.", &tracker);
	
	printTestResults(tracker);
	destroyStatusTracker(&tracker);	

	return 0;
}
