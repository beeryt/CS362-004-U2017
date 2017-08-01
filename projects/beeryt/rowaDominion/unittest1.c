#include "./dominion.h"
#include "./dominion_helpers.h"
#include "./testing_helpers.h"
#include <stdlib.h>
#include <stdio.h>

int main() {
	int onlyDeck = gardens, onlyHand = smithy, onlyDiscard = sea_hag, allThree = copper, notInAny = adventurer;
	const int chosenKingdomCards[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse, sea_hag, tribute, smithy};
	const int playerInitialHands[][MAX_HAND] = { { smithy, copper, copper, copper, copper  }, { } };
   	const int playerInitialHandSizes[] = { 5, 0 };
	const int playerInitialDecks[][MAX_DECK] = { { gardens, embargo, village, minion, copper }, { } };
	const int playerInitialDeckSizes[] = { 5, 0 };

	int player = 0;
	struct gameState state;
	struct StatusTracker tracker;

	initStatusTracker(&tracker);
	initializeTestGame(2, playerInitialHands, playerInitialHandSizes, playerInitialDecks, playerInitialDeckSizes, chosenKingdomCards, 10, 1, &state); 
	state.discard[player][0] = copper;
	state.discard[player][1] = sea_hag;
	state.discardCount[player] = 2;

	printf("----fullDeckCount Function Test----\n");
	
	addContextToTracker("Card that is only in deck.", &tracker);
	assertTrue(fullDeckCount(player, onlyDeck, &state) == numCardIn(onlyDeck, state.deck[player], state.deckCount[player]), "Should return number equal to number of card in deck.", &tracker);

	addContextToTracker("Card that is only in hand.", &tracker);
	assertTrue(fullDeckCount(player, onlyHand, &state) == numCardIn(onlyHand, state.hand[player], state.handCount[player]), "Should return number equal to number of card in hand.", &tracker);

	addContextToTracker("Card that is only in discard.", &tracker);
	assertTrue(fullDeckCount(player, onlyDiscard, &state) == numCardIn(onlyDiscard, state.discard[player], state.discardCount[player]), "Should return number equal to number of card in discard.", &tracker);

	addContextToTracker("Card that is in deck, hand, and discard.", &tracker);
	assertTrue(fullDeckCount(player, allThree, &state) == numCardIn(allThree, state.hand[player], state.handCount[player]) + numCardIn(allThree, state.deck[player], state.deckCount[player]) + numCardIn(allThree, state.discard[player], state.discardCount[player]), "Should return number equal to number of card in hand + deck + discard.", &tracker);

	addContextToTracker("Card that player does not have.", &tracker);
	assertTrue(fullDeckCount(player, notInAny, &state) == 0, "Should return 0 instances of card found.", &tracker);

	printTestResults(tracker);
	destroyStatusTracker(&tracker);	

	return 0;
}
