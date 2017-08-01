#include "./dominion.h"
#include "./dominion_helpers.h"
#include "./testing_helpers.h"
#include "rngs.h"

int main() {
	int i, j, expectedDrawnTreasure, expectedDiscardedCards;
	int numTestScenarios = 100;
	struct StatusTracker tracker;
	struct gameState state;
	struct gameState unchangedState;

	//set up random number generator
	SelectStream(1);
	PutSeed((long)(-1));

	initStatusTracker(&tracker);

	for(i = 0; i < numTestScenarios; i++) {
		initializeRandomState(&state);
		state.phase = 0; //need to be in action phase for playing adventurer to make sense
		expectedDrawnTreasure = 0;
		expectedDiscardedCards = 1; //starts at one to account for adventurer discarding itself from hand
		for(j = 0; j < state.deckCount[state.whoseTurn]; j++) {
			if(treasure(state.deck[state.whoseTurn][j])) {
				expectedDrawnTreasure++;
				if(expectedDrawnTreasure == 2) {
					break;
				}
			} else {
				expectedDiscardedCards++;
			}
		}

		unchangedState = state;

		adventurerCardEffect(state.whoseTurn, &state);	
	
		addContextToTracker("", &tracker);
		assertEqual("Current player", unchangedState.whoseTurn, state.whoseTurn, &tracker);		
		assertEqual("Number of cards added to hand", expectedDrawnTreasure, state.handCount[state.whoseTurn] - unchangedState.handCount[unchangedState.whoseTurn], &tracker);		
		assertEqual("Number of treasure cards added to hand", expectedDrawnTreasure, treasureInHand(state, state.whoseTurn) - treasureInHand(unchangedState, unchangedState.whoseTurn), &tracker);		
		assertEqual("Number of cards added to discard", expectedDiscardedCards, state.discardCount[state.whoseTurn] - unchangedState.discardCount[unchangedState.whoseTurn], &tracker);		
	}

	printFailedTestResults(tracker);

	return 0;
}
