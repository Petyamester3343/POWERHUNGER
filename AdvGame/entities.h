#pragma once

#include <stdbool.h>
#include <stdlib.h>

#define SKILL_ROSTER 6

// A homemade struct for defining position on a map
typedef struct Pos {
	unsigned int col;
	unsigned int row;
	bool occupied;
} Pos;

// The struct for casting magic on Player's behalf
typedef struct Magia {
	char* magName;
	unsigned int magATK;
	unsigned int magCost;
	unsigned int magLVReq;
	bool acquired;
} Magia;

// Objects which would permanently buff the player until their death
typedef struct Object {
	char sign;
	unsigned int eff;
	bool found;
	Pos pos;
} Object;

// The core struct of everything sentient in this project
typedef struct Entity {
	char* name;
	unsigned int hp;
	unsigned int atk;
	unsigned int def;
	unsigned int xp;
	bool dead;
	bool fled;
	Pos pos;
} Entity;

// The playable character's struct, having Entity "inherited,"
// and being able to cast magic, possess money, and level up
typedef struct Player {
	Entity E;
	Magia M[SKILL_ROSTER];
	unsigned int mp;
	unsigned int deathCount;
	unsigned int money;
	unsigned int lv;
	unsigned int remembers;
} Player;

// The enemy struct, having Entity "inherited,"
// and containing loot for the Player to pick up after it died
typedef struct Foe {
	Entity E;
	unsigned int loot;
} Foe;