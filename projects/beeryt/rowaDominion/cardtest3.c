#include "./dominion.h"
#include "./dominion_helpers.h"
#include "./testing_helpers.h"
#include <stdlib.h>
#include <assert.h>

#include <stdio.h>

int main() {
	const int chosenKingdomCards[10] = {remodel, gardens, embargo, village, minion, mine, cutpurse, sea_hag, tribute, smithy};
	const int playerInitialHands[][MAX_HAND] = { { remodel, copper, copper, copper, copper  }, { } };
   	const int playerInitialHandSizes[] = { 5, 0 };
	const int playerInitialDecks[][MAX_DECK] = { { gardens, embargo, village, minion }, { } };
	const int playerInitialDeckSizes[] = { 4, 0 };
	
	int player = 0, initialDeckValue, initialFullPlayerCardCount, initialDeckCount, supplyCountChanged = 0, initialNumActions, initialNumBuys, i;
	int initialTrashedCardDeckCount, initialTrashedCardHandCount, initialTrashedCardDiscardCount; 
	int initialGainedCardDeckCount, initialGainedCardHandCount, initialGainedCardDiscardCount;
	struct gameState state;
	int initialSupplyCount[treasure_map + 1];
	int initialDeck[MAX_DECK];
	int gainedCard = sea_hag;
   	int trashedCard = minion;	
	struct StatusTracker tracker;

	initStatusTracker(&tracker);
	initializeTestGame(2, playerInitialHands, playerInitialHandSizes, playerInitialDecks, playerInitialDeckSizes, chosenKingdomCards, 10, 1, &state); 
	initialDeckValue = fullDeckValue(player, state);
	initialDeckCount = state.deckCount[player];
   	initialTrashedCardDeckCount = numCardIn(trashedCard, state.deck[player], state.deckCount[player]);	
   	initialTrashedCardHandCount = numCardIn(trashedCard, state.hand[player], state.handCount[player]);	
   	initialTrashedCardDiscardCount = numCardIn(trashedCard, state.discard[player], state.discardCount[player]);	
	initialGainedCardDeckCount = numCardIn(gainedCard, state.deck[player], state.deckCount[player]);	
   	initialGainedCardHandCount = numCardIn(gainedCard, state.hand[player], state.handCount[player]);	
   	initialGainedCardDiscardCount = numCardIn(gainedCard, state.discard[player], state.discardCount[player]);	
	initialNumActions = state.numActions;
	initialNumBuys = state.numBuys;
	initialFullPlayerCardCount = state.deckCount[player] + state.discardCount[player] + state.handCount[player];
	initialTrashedCardDeckCount = fullDeckCount(player, trashedCard, &state);

	for(i = 0; i < state.deckCount[player]; i++) {
		initialDeck[i] = state.deck[player][i];
	}

	for(i = 0; i < treasure_map + 1; i++) {
		initialSupplyCount[i] = state.supplyCount[i];
	}

	remodelCardEffect(player, &state, trashedCard, gainedCard, findInHand(remodel, &state)); 

	printf("----Remodel Card Test----\n");
	addContextToTracker("Basic test where Remodel is used to trash card and gain other card with <= +2 value of trashed card.", &tracker);
	assertTrue(numCardIn(gainedCard, state.discard[player], state.discardCount[player]) >= initialGainedCardDiscardCount + 1, "Player should have at least one more of gained card in discard.", &tracker);
	assertTrue(numCardIn(gainedCard, state.discard[player], state.discardCount[player]) <= initialGainedCardDiscardCount + 1, "Player should have at most one more of gained card in discard.", &tracker);
	assertTrue(numCardIn(gainedCard, state.hand[player], state.handCount[player]) == initialGainedCardHandCount, "Player should have the same number of gained card in hand after Remodel is played (card should go to discard, not hand).", &tracker);
	assertTrue(numCardIn(gainedCard, state.deck[player], state.deckCount[player]) == initialGainedCardDeckCount, "Player should have the same number of gained card in deck after Remodel is played (card should go to discard, not deck).", &tracker);
	
	assertTrue(fullDeckValue(player, state) == initialDeckValue - getCost(trashedCard) + getCost(gainedCard), "Player's full deck value should change by the difference in value of the gained card and the trashed card.", &tracker);
	assertTrue(numCardIn(trashedCard, state.hand[player], state.handCount[player]) == initialTrashedCardHandCount - 1, "Player should have one less of trashed card in their hand after Remodel is played.", &tracker);
	assertTrue(numCardIn(trashedCard, state.discard[player], state.discardCount[player]) == initialTrashedCardDiscardCount, "Trashed card should not be placed in Player's discard pile when Remodel is played.", &tracker);
	assertTrue(fullDeckCount(player, trashedCard, &state) == initialTrashedCardDeckCount - 1, "Player should have one less of trashed card in their entire deck after Remodel is played (checking to make sure trashed cards wasn't placed in discard or deck).", &tracker);
	assertTrue(initialFullPlayerCardCount == state.deckCount[player] + state.discardCount[player] + state.handCount[player], "Player should have same number of total cards after Remodel is played.", &tracker);
	assertTrue(initialDeckCount == state.deckCount[player] && sameCompositionOfCards(initialDeck, state.deck[player], initialDeckCount), "Deck should remain unchanged after Remodel is played.", &tracker);	
	assertTrue(state.numActions == initialNumActions, "Player's number of action turns should not be directly affected by Remodel.", &tracker);
	assertTrue(state.numBuys == initialNumBuys, "Player should have same number of turns for buy phase after Remodel is played.", &tracker);

	for(i = 0; i < treasure_map + 1; i++) {
		if(i != gainedCard && initialSupplyCount[i] != state.supplyCount[i]) {
			supplyCountChanged = 1;
			break;
		}
	}

	assertTrue(!supplyCountChanged, "The supply stacks of cards that player did not gain card from should not be changed when Remodel is played.", &tracker); 

	printTestResults(tracker);

	destroyStatusTracker(&tracker);	

	return 0;
}
