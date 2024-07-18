#pragma once

#include "entities.h"
#include "def_vals.h"

#include <stdio.h>
#include <math.h>

// Increases the player level as long as they have a sufficient amount of experience points
static void level_up(Player* p) {
	unsigned int req = 0;
	req = (p->lv <= 4) ? (unsigned int)pow(2, (double)p->lv) * 10 : req + 200;
	if (p->E.xp >= req) {
		p->lv++;
		PLAYER_MAX_HP += 10;
		PLAYER_ATK += 10;
		PLAYER_DEF += 10;
		PLAYER_MAX_MP += 10;
		p->E.hp = PLAYER_MAX_HP;
		p->E.atk = PLAYER_ATK;
		p->E.def = PLAYER_DEF;
		p->mp = PLAYER_MAX_MP;
		printf("%s is now at LEVEL %d!\n", p->E.name, p->lv);
		Sleep(500);
	}
	req = (p->lv <= 4) ? (unsigned int)pow(2, (double)p->lv) * 15 : req + 200;
}

// Grants the player experience points
// (Either from an Arcane Tome or from slaying a Foe)
static void xp_gain(unsigned int experience, Player* p, bool fought) {
	unsigned int req = 0;
	req = (p->lv <= 4) ? (unsigned int)pow(2, (double)p->lv) * 15 : req * 200;
	p->E.xp += experience;
	if (fought)
		printf("%s has gained %d XP from the battle! (Current: %d / %d)\n",
			p->E.name, experience, p->E.xp, req);
	else
		printf("(Current: %d / %d)\n", p->E.xp, req);
	while (p->E.xp >= req) {
		level_up(p);
		req = (p->lv <= 4) ? (unsigned int)pow(2, (double)p->lv) * 15 : req * 200;
	}
}

// Grants the player money from slaying a Foe
// Shops will be at Town one day
static void looting(Player* p, unsigned int money) {
	p->money += money;
	printf("%d GOLD gained!\n", money);
	Sleep(500);
}
