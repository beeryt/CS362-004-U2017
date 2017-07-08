#ifndef _CARDEFFECTS_H
#define _CARDEFFECTS_H

int cardEffect_Adventurer(struct gameState *state);
int cardEffect_Smithy(struct gameState *state, int handPos);
int cardEffect_Steward(struct gameState *state, int handPos, int choice1, int choice2, int choice3);
int cardEffect_Gardens(struct gameState *state);
int cardEffect_Feast(struct gameState *state, int choice1);

#endif
