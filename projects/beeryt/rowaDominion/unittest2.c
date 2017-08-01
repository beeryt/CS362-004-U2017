#include "./dominion.h"
#include "./dominion_helpers.h"
#include "./testing_helpers.h"
#include <stdlib.h>
#include <stdio.h>

int main() {
	const int chosenKingdomCards[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse, sea_hag, tribute, smithy};
	const int playerInitialHands[][MAX_HAND] = { { smithy, copper, copper, copper, copper  }, { } };
   	const int playerInitialHandSizes[] = { 5, 0 };
	const int playerInitialDecks[][MAX_DECK] = { { gardens, embargo, village, minion }, { } };
	const int playerInitialDeckSizes[] = { 4, 0 };
	
	int player = 0;
	int cardToGain = adventurer;
	int cardNotInGame = great_hall; 
	int initialSupplyCount;
	struct gameState state;
	struct StatusTracker tracker;

	initStatusTracker(&tracker);
	initializeTestGame(2, playerInitialHands, playerInitialHandSizes, playerInitialDecks, playerInitialDeckSizes, chosenKingdomCards, 10, 1, &state); 
	initialSupplyCount = state.supplyCount[cardToGain];

	printf("----gainCard Function Test----\n");

	addContextToTracker("Gain card to hand.", &tracker);
	assertTrue(gainCard(cardToGain, &state, 2, player) == 0, "Return value of gainCard should be zero indicating success.", &tracker);
	assertTrue(onlyGainedCardNumTimes(cardToGain, 1, playerInitialHands[player], playerInitialHandSizes[player], state.hand[player], state.handCount[player]), "Hand should be unchanged aside from addition of gained card.", &tracker);
	assertTrue(state.deckCount[player] == playerInitialDeckSizes[player] && sameOrderedCollectionOfCards(state.deck[player], playerInitialDecks[player], state.deckCount[player]), "Deck should remain unchanged after card is gained.", &tracker);
	assertTrue(state.discardCount[player] == 0, "Discard should remain unchanged after card is gained.", &tracker);
	assertTrue(state.supplyCount[cardToGain] == initialSupplyCount - 1, "Supply count of gained card should be one less after card is gained.", &tracker);

	addContextToTracker("Gain card to deck.", &tracker);
	initializeTestGame(2, playerInitialHands, playerInitialHandSizes, playerInitialDecks, playerInitialDeckSizes, chosenKingdomCards, 10, 1, &state); 
	assertTrue(gainCard(cardToGain, &state, 1, player) == 0, "Return value of gainCard should be zero indicating success.", &tracker);
	assertTrue(onlyGainedCardNumTimes(cardToGain, 1, playerInitialDecks[player], playerInitialDeckSizes[player], state.deck[player], state.deckCount[player]), "Deck should be unchanged aside from addition of gained card.", &tracker);
	assertTrue(state.handCount[player] == playerInitialHandSizes[player] && sameOrderedCollectionOfCards(state.hand[player], playerInitialHands[player], state.handCount[player]), "Hand should remain unchanged after card is gained.", &tracker);
	assertTrue(state.discardCount[player] == 0, "Discard should remain unchanged after card is gained.", &tracker);
	assertTrue(state.supplyCount[cardToGain] == initialSupplyCount - 1, "Supply count of gained card should be one less after card is gained.", &tracker);

	addContextToTracker("Gain card to discard.", &tracker);
	initializeTestGame(2, playerInitialHands, playerInitialHandSizes, playerInitialDecks, playerInitialDeckSizes, chosenKingdomCards, 10, 1, &state); 
	assertTrue(gainCard(cardToGain, &state, 0, player) == 0, "Return value of gainCard should be zero indicating success.", &tracker);
	assertTrue(state.discardCount[player] == 1 && state.discard[player][0] == cardToGain, "Discard should be unchanged aside from addition of gained card.", &tracker);
	assertTrue(state.handCount[player] == playerInitialHandSizes[player] && sameOrderedCollectionOfCards(state.hand[player], playerInitialHands[player], state.handCount[player]), "Hand should remain unchanged after card is gained.", &tracker);
	assertTrue(state.deckCount[player] == playerInitialDeckSizes[player] && sameOrderedCollectionOfCards(state.deck[player], playerInitialDecks[player], state.deckCount[player]), "Deck should remain unchanged after card is gained.", &tracker);
	assertTrue(state.supplyCount[cardToGain] == initialSupplyCount - 1, "Supply count of gained card should be one less after card is gained.", &tracker);

	addContextToTracker("Gain card to invalid location.", &tracker);
	initializeTestGame(2, playerInitialHands, playerInitialHandSizes, playerInitialDecks, playerInitialDeckSizes, chosenKingdomCards, 10, 1, &state); 
	assertTrue(gainCard(cardToGain, &state, 100, player) == -1, "Return value of gainCard should be -1 indicating failure.", &tracker);
	assertTrue(state.handCount[player] == playerInitialHandSizes[player] && sameOrderedCollectionOfCards(state.hand[player], playerInitialHands[player], state.handCount[player]), "Hand should remain unchanged after gain fails.", &tracker);
	assertTrue(state.deckCount[player] == playerInitialDeckSizes[player] && sameOrderedCollectionOfCards(state.deck[player], playerInitialDecks[player], state.deckCount[player]), "Deck should remain unchanged after gain fails.", &tracker);
	assertTrue(state.discardCount[player] == 0, "Discard should remain unchanged after card gain fails.", &tracker);
	assertTrue(state.supplyCount[cardToGain] == initialSupplyCount, "Supply count of gained card should be unchanged after gain fails.", &tracker);

	addContextToTracker("Gain card not used in game.", &tracker);
	initializeTestGame(2, playerInitialHands, playerInitialHandSizes, playerInitialDecks, playerInitialDeckSizes, chosenKingdomCards, 10, 1, &state); 
	assertTrue(gainCard(cardNotInGame, &state, 1, player) == -1 && gainCard(cardNotInGame, &state, 0, player) == -1 && gainCard(cardNotInGame, &state, 2, player) == -1, "Return value of gainCard should -1 indicating failure.", &tracker);
	assertTrue(state.handCount[player] == playerInitialHandSizes[player] && sameOrderedCollectionOfCards(state.hand[player], playerInitialHands[player], state.handCount[player]), "Hand should remain unchanged after gain fails.", &tracker);
	assertTrue(state.deckCount[player] == playerInitialDeckSizes[player] && sameOrderedCollectionOfCards(state.deck[player], playerInitialDecks[player], state.deckCount[player]), "Deck should remain unchanged after gain fails.", &tracker);
	assertTrue(state.discardCount[player] == 0, "Discard should remain unchanged after card gain fails.", &tracker);
	assertTrue(state.supplyCount[cardToGain] == initialSupplyCount, "Supply count of gained card should be unchanged after gain fails.", &tracker);

	addContextToTracker("Gain card used in game that has run out of supply.", &tracker);
	initializeTestGame(2, playerInitialHands, playerInitialHandSizes, playerInitialDecks, playerInitialDeckSizes, chosenKingdomCards, 10, 1, &state); 
	state.supplyCount[cardToGain] = 0;
	assertTrue(gainCard(cardToGain, &state, 1, player) == -1 && gainCard(cardToGain, &state, 0, player) == -1 && gainCard(cardToGain, &state, 2, player) == -1, "Return value of gainCard should be -1 indicating failure.", &tracker);
	assertTrue(state.handCount[player] == playerInitialHandSizes[player] && sameOrderedCollectionOfCards(state.hand[player], playerInitialHands[player], state.handCount[player]), "Hand should remain unchanged after gain fails.", &tracker);
	assertTrue(state.deckCount[player] == playerInitialDeckSizes[player] && sameOrderedCollectionOfCards(state.deck[player], playerInitialDecks[player], state.deckCount[player]), "Deck should remain unchanged after gain fails.", &tracker);
	assertTrue(state.discardCount[player] == 0, "Discard should remain unchanged after card gain fails.", &tracker);
	assertTrue(state.supplyCount[cardToGain] == 0, "Supply count of gained card should still zero after gain fails.", &tracker);


	printTestResults(tracker);
	destroyStatusTracker(&tracker);	

	return 0;
}
