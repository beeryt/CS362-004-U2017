#include "./dominion.h"
#include "./dominion_helpers.h"
#include "./testing_helpers.h"
#include "rngs.h"

int main() {
	int i, j, numDrewOne;
	int numTestScenarios = 100;
	struct StatusTracker tracker;
	struct gameState state;
	struct gameState unchangedState;

	initStatusTracker(&tracker);

	//set up random number generator
	SelectStream(1);
	PutSeed((long)(-1));

	for(i = 0; i < numTestScenarios; i++) {
		initializeRandomState(&state);
		state.phase = 0; //need to be in action phase for playing card to make sense
		if(state.handCount[state.whoseTurn] > MAX_HAND - 4) {
			state.handCount[state.whoseTurn] = MAX_HAND - 4;
		}

		state.hand[state.whoseTurn][state.handCount[state.whoseTurn] - 1] = council_room;

		unchangedState = state;

		councilRoomCardEffect(state.whoseTurn, &state, state.handCount[state.whoseTurn] - 1);	

		numDrewOne = 0;
		for(j = 0; j < state.numPlayers; j++) {
			if(j != state.whoseTurn && state.handCount[j] == 1 + unchangedState.handCount[j]) {
				numDrewOne++;
			}
		}

		addContextToTracker("", &tracker);
		assertEqual("Current player", unchangedState.whoseTurn, state.whoseTurn, &tracker);		
		assertEqual("Number of buys gained", 1, state.numBuys - unchangedState.numBuys, &tracker);	//3 rather than 4 because council room gets discarded within its own cardEffect
		assertEqual("Net change of number of cards in hand", 3, state.handCount[state.whoseTurn] - unchangedState.handCount[unchangedState.whoseTurn], &tracker);	//3 rather than 4 because council room gets discarded within its own cardEffect
		if(unchangedState.discardCount[unchangedState.whoseTurn] > state.discardCount[state.whoseTurn] || unchangedState.deckCount[unchangedState.whoseTurn] < 3) {
			assertEqual("Number of cards added to discard", 1, state.discardCount[state.whoseTurn], &tracker);		//smithy should have been added to discard
			assertEqual("Number of cards removed from deck", 4, unchangedState.deckCount[unchangedState.whoseTurn] + unchangedState.discardCount[unchangedState.whoseTurn] - state.deckCount[state.whoseTurn], &tracker);		//accounts for reshuffling cards from discard back into deck
		} else {
			assertEqual("Number of cards added to discard", 1, state.discardCount[state.whoseTurn] - unchangedState.discardCount[unchangedState.whoseTurn], &tracker);	//accounts for reshuffling cards from discard back into deck then adding smithy into discard	
			assertEqual("Number of cards removed from deck", 4, unchangedState.deckCount[unchangedState.whoseTurn] - state.deckCount[state.whoseTurn], &tracker);		//3 cards should have been drawn from deck
		}

		if(assertTrue(state.discardCount[state.whoseTurn], "Should be at least one card in discard (smithy)", &tracker)) { 
			assertEqual("Top card of discard (8 == council room)", council_room, state.discard[state.whoseTurn][state.discardCount[state.whoseTurn] - 1], &tracker);		//smithy should have been added to discard
		} else {
			assertTrue(0, "Council Room should be top card of discard", &tracker);
		}

		if(assertEqual("Number of total players", unchangedState.numPlayers, state.numPlayers, &tracker)) {
			assertEqual("Number of other players who drew one card", unchangedState.numPlayers - 1, numDrewOne, &tracker);
		} else {
			assertTrue(0, "Can't test number of other players who drew one card since total number of players changed", &tracker);
		}
	}

	printFailedTestResults(tracker);

	return 0;
}
