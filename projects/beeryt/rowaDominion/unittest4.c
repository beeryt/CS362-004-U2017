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
	
	int i, j, allHandCardsCorrectlyIdentified;
	char buffer[256];
	struct gameState state;
	struct StatusTracker tracker;

	initStatusTracker(&tracker);
	initializeTestGame(3, playerInitialHands, playerInitialHandSizes, playerInitialDecks, playerInitialDeckSizes, chosenKingdomCards, 10, 1, &state); 
	
	printf("----handCard Function Test----\n");

	addContextToTracker("Basic test that function returns card at given index in current player's hand.", &tracker);
	for(i = 0; i < state.numPlayers; i++) {
		allHandCardsCorrectlyIdentified = 1;
		
		for(j = 0; j < playerInitialHandSizes[i]; j++) {
			if(handCard(j, &state) != playerInitialHands[i][j]) {
				allHandCardsCorrectlyIdentified = 0;
				break;
			}
		}

		sprintf(buffer, "Function should correctly identify all cards in hand of player %d", i + 1);
		assertTrue(allHandCardsCorrectlyIdentified, buffer, &tracker); 
		state.whoseTurn++;
		if(i < state.numPlayers - 1) {
			addContextToTracker("Moved to next player.", &tracker);
		}
	}	
	
	printTestResults(tracker);
	destroyStatusTracker(&tracker);	

	return 0;
}
