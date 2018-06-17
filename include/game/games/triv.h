#pragma once

#include <master.h>

#include <game/game-impl.h>

#include <game/player.h>

// CONSTRUCTORS

Game * create_game_triv (
	in Player * winner
);

// METHODS

const Player * game_triv_winner (
	in Game * this
);

// TESTS

void test_game_triv (void);
