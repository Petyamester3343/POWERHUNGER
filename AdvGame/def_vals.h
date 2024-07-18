#pragma once

#include <stdbool.h>

#include "entities.h"

// DEFAULT MAP SIZE
#define MAP_SIZE 15
#define LVUP_VAL 10

// PLAYER DEFAULT STATS
unsigned int PLAYER_MAX_HP = 100;
unsigned int PLAYER_MAX_MP = 50;
unsigned int PLAYER_ATK = 10;
unsigned int PLAYER_DEF = 10;
unsigned int PLAYER_MIN_LV = 1;
Pos PLAYER_START_POS = { 7,7 };

static void defPlayerValRestore(Player* p) {
	PLAYER_MAX_HP = 100;
	PLAYER_MAX_MP = 50;
	PLAYER_ATK = 10;
	PLAYER_DEF = 10;

	p->E.hp = PLAYER_MAX_HP;
	p->mp = PLAYER_MAX_MP;
	p->E.atk = PLAYER_ATK;
	p->E.def = PLAYER_DEF;
	p->lv = PLAYER_MIN_LV;
	p->E.xp = 0;
	p->E.pos = PLAYER_START_POS;
	p->E.dead = false;
}

// Enemies

// SLIME DEFAULT STATS
unsigned int SLIME_HP = 15;
unsigned int SLIME_ATK = 3;
unsigned int SLIME_DEF = 2;
unsigned int SLIME_XP = 10;

// GOBLIN DEFAULT STATS
unsigned int GOBLIN_HP = 25;
unsigned int GOBLIN_ATK = 7;
unsigned int GOBLIN_DEF = 6;
unsigned int GOBLIN_XP = 20;

// ACOLYTE DEFAULT STATS
unsigned int ACOLYTE_HP = 40;
unsigned int ACOLYTE_ATK = 12;
unsigned int ACOLYTE_DEF = 10;
unsigned int ACOLYTE_XP = 40;

// THWARTED SELF DEFAULT STATS
unsigned int THWARTED_SELF_HP = 70;
unsigned int THWARTED_SELF_ATK = 20;
unsigned int THWARTED_SELF_DEF = 14;
unsigned int THWARTED_SELF_XP = 80;

// GOLEM DEFAULT STATS
unsigned int GOLEM_HP = 90;
unsigned int GOLEM_ATK = 30;
unsigned int GOLEM_DEF = 20;
unsigned int GOLEM_XP = 120;

// JIMMY DEFAULT STATS
unsigned int JIMMY_HP = 150;
unsigned int JIMMY_ATK = 50;
unsigned int JIMMY_DEF = 40;

// Magic

// FIREBALL DEFAULT STATS
unsigned int FIREBALL_ATK = 10;
unsigned int FIREBALL_COST = 5;
unsigned int FIREBALL_LV_REQ = 2;

// BLIZZARD DEFAULT STATS
unsigned int BLIZZARD_ATK = 20;
unsigned int BLIZZARD_COST = 10;
unsigned int BLIZZARD_LV_REQ = 4;

// LIGHTNING DEFAULT STATS
unsigned int LIGHTNING_ATK = 25;
unsigned int LIGHTNING_COST = 15;
unsigned int LIGHTNING_LV_REQ = 6;

// GRAVITY_BOMB DEFAULT STATS
unsigned int GRAV_BOMB_ATK = 35;
unsigned int GRAV_BOMB_COST = 25;
unsigned int GRAV_BOMB_LV_REQ = 7;

// RSoJ DEFAULT STATS
unsigned int RSOJ_ATK = 50;
unsigned int RSOJ_COST = 40;
unsigned int RSOJ_LV_REQ = 9;