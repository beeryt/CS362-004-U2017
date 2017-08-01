#include "./dominion.h"
#include "./dominion_helpers.h"
#include "./testing_helpers.h"
#include <stdlib.h>
#include <assert.h>

#include <stdio.h>

int main() {
	const int chosenKingdomCards[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse, sea_hag, tribute, smithy};
	const int playerInitialHands[][MAX_HAND] = { { smithy, copper, copper, copper, copper  }, { } };
   	const int playerInitialHandSizes[] = { 5, 0 };
	const int playerInitialDecks[][MAX_DECK] = { { gardens, embargo, village, minion }, { } };
	const int playerInitialDeckSizes[] = { 4, 0 };
	
	int player = 0, supplyCountChanged = 0, initialHandCount, initialDeckCount, i;
	struct gameState state;
	int initialSupplyCount[treasure_map] = { 0 };
	int targetHand[MAX_HAND] = { 0 };
	int smithyFoundInHand = 0;
	struct StatusTracker tracker;

	initStatusTracker(&tracker);
	initializeTestGame(2, playerInitialHands, playerInitialHandSizes, playerInitialDecks, playerInitialDeckSizes, chosenKingdomCards, 10, 1, &state); 
	
	for(i = 0; i < state.handCount[player]; i++) {
		if(!smithyFoundInHand && state.hand[player][i] == smithy) {
			smithyFoundInHand = 1; 
		} else {
			targetHand[i - smithyFoundInHand] = state.hand[player][i];
		}
	}

	//By using smithyFoundInHand to alter index, we can put the smithy into the hand anywhere before this point (for modifiability).
	targetHand[state.handCount[player] - smithyFoundInHand] = state.deck[player][state.deckCount[player] - 1];
	targetHand[state.handCount[player] - smithyFoundInHand + 1] = state.deck[player][state.deckCount[player] - 2];
	targetHand[state.handCount[player] - smithyFoundInHand + 2] = state.deck[player][state.deckCount[player] - 3];

	initialHandCount = state.handCount[player];
	initialDeckCount = state.deckCount[player];

	for(i = 0; i < treasure_map; i++) {
		initialSupplyCount[i] = state.supplyCount[i];
	}

	smithyCardEffect(player, &state, findInHand(smithy, &state)); 

	//Note: I don't test whether Smithy discards itself or not because I don't believe that functionality necessarily needs to belong in the Smithy card effect.
	printf("----Smithy Card Test----\n");

	addContextToTracker("Basic test where deck has at least three cards.", &tracker);
	assertTrue(initialHandCount + 2 <= state.handCount[player], "Number of cards in player's hand should be at least two greater after Smithy is played (-1 for Smithy, +3 for drawn cards).", &tracker);
	assertTrue(initialHandCount + 2 >= state.handCount[player], "Number of cards in player's hand should be at most two greater after Smithy is played (-1 for Smithy, +3 for drawn cards).", &tracker);
	assertTrue(sameCompositionOfCards(targetHand, state.hand[player], initialHandCount + 3), "Player's hand should remain unchanged aside from gaining the top three cards of the deck from before Smithy after Smithy is played.", &tracker);
	assertTrue(initialDeckCount - 3 >= state.deckCount[player], "Number of cards in player's deck should be at least three less after Smithy is played.", &tracker);
	assertTrue(initialDeckCount - 3 <= state.deckCount[player], "Number of cards in player's deck should be at most three less after Smithy is played.", &tracker);

	for(i = 0; i < treasure_map; i++) {
		if(initialSupplyCount[i] != state.supplyCount[i]) {
			supplyCountChanged = 1;
			break;
		}
	}

	assertTrue(!supplyCountChanged, "The supply stacks should not be changed when Smithy is played.", &tracker); 

	printTestResults(tracker);

	destroyStatusTracker(&tracker);	

	return 0;
}
