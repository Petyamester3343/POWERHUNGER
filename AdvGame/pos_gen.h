#pragma once

#include "DLL_container.h"

static bool isPosOccupied(unsigned int col, unsigned int row, Player* p, OBJ_DLL* ol, FOE_DLL* fl) {
	if (col == p->E.pos.col && row == p->E.pos.row && p->E.pos.occupied)
		return true;

	ObjNode* currObj = ol->head;
	while (currObj != NULL) {
		if (currObj->o->pos.col == col && currObj->o->pos.row == row && currObj->o->pos.occupied)
			return true;
		currObj = currObj->next;
	}

	FoeNode* currFoe = fl->head;
	while (currFoe != NULL) {
		if (currFoe->f->E.pos.col == col && currFoe->f->E.pos.row == row && currFoe->f->E.pos.occupied)
			return true;
		currFoe = currFoe->next;
	}

	return false;
}

// random position generator
static Pos generateRandomPos(Player* p, OBJ_DLL* ol, FOE_DLL* fl) {
	Pos pos;

	do {
		pos.col = (unsigned int)rand() % 15;
		pos.row = (unsigned int)rand() % 15;
	} while ((pos.col == 14 && pos.row == 14) && isPosOccupied(pos.col, pos.row, p, ol, fl) && (pos.col == p->E.pos.col && pos.row == p->E.pos.row));

	pos.occupied = true;
	return pos;
}