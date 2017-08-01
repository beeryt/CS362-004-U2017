#include "./dominion.h"
#include "./dominion_helpers.h"
#include "./testing_helpers.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

int main() {
	const int chosenKingdomCards[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse, sea_hag, tribute, smithy};
	const int playerInitialHands[][MAX_HAND] = { { adventurer, copper, copper, copper, copper  }, { } };
   	const int playerInitialHandSizes[] = { 5, 0 };
	const int playerInitialDecks[][MAX_DECK] = { { gardens, copper, embargo, copper, village, minion }, { } };
	const int playerInitialDeckSizes[] = { 4, 0 };

	int player = 0, supplyCountChanged, initialHandCount, firstTreasurePos = -1, secondTreasurePos = -1, initialNumActions, initialDeckCount, initialDiscardCount, initialTreasureInHand = 0, i;
	int initialSupplyCount[treasure_map];
	struct gameState state;
	struct StatusTracker tracker;	

	initStatusTracker(&tracker);
	initializeTestGame(2, playerInitialHands, playerInitialHandSizes, playerInitialDecks, playerInitialDeckSizes, chosenKingdomCards, 10, 1, &state); 
	initialNumActions = state.numActions;
	initialHandCount = state.handCount[player];
	initialDeckCount = state.deckCount[player];
	initialDiscardCount = state.discardCount[player];
	initialTreasureInHand = treasureInHand(state, player);
	for(i = 0; i < treasure_map; i++) {
		initialSupplyCount[i] = state.supplyCount[i];
	}	

	for(i = state.deckCount[player] - 1; i >= 0; i--) {
		if(treasure(state.deck[player][i])) {
			if(firstTreasurePos != -1) {
				secondTreasurePos = i;
				break;
			} else {
				firstTreasurePos = i;
			}
		}	
	}	

	adventurerCardEffect(player, &state);
	//TODO: test with one treasure card in deck
	printf("----Adventurer Card Test----\n");

	addContextToTracker("Basic test where deck has at least two treasure cards.", &tracker);	
	assertTrue(state.numActions == initialNumActions, "Player's available number of actions should not be directly affected by Adventurer.", &tracker);
	assertTrue(state.handCount[player] >= initialHandCount + 2, "Player should have at least two more cards in hand after Adventurer is played.", &tracker);
	assertTrue(state.handCount[player] <= initialHandCount + 2, "Player should have at most two more cards in hand after Adventurer is played.", &tracker);
	assertTrue(treasureInHand(state, player) >= initialTreasureInHand + 2, "Player should have added least two treasure cards to hand.", &tracker);
	assertTrue(treasureInHand(state, player) <= initialTreasureInHand + 2, "Player should have added at most two treasure cards to hand.", &tracker);
	assertTrue(state.deckCount[player] >= secondTreasurePos, "Deck should have lost at least all cards up to and including second revealed treasure card after Adventurer is played.", &tracker);	
	assertTrue(state.deckCount[player] <= secondTreasurePos, "Deck should have lost at most all cards up to and including second revealed treasure card after Adventurer is played.", &tracker);	
	assertTrue(state.discardCount[player] >= initialDiscardCount + initialDeckCount - secondTreasurePos, "Discard pile should have gained all of the non-treasure cards revealed before the second treasure card.", &tracker);

	for(i = 0; i < treasure_map; i++) {
		if(initialSupplyCount[i] != state.supplyCount[i]) {
			supplyCountChanged = 1;
			break;
		}
	}

	assertTrue(!supplyCountChanged, "The supply stacks should not be changed when Adventurer is played.", &tracker); 

	printTestResults(tracker);

	destroyStatusTracker(&tracker);	

	return 0;
}
