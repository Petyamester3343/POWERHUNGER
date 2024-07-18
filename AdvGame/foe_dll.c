#pragma warning(disable:4996)

#include "DLL_container.h"
#include "pos_gen.h"

#include <string.h>
#include <stdlib.h>

// Constructor
static FOE_DLL* createFoeList() {
	FOE_DLL* list = (FOE_DLL*)malloc(sizeof(FOE_DLL));
	if (list != NULL) {
		list->head = NULL;
		list->tail = NULL;
	}
	return list;
}

// Creates an instance of Foe inside the list
static FoeNode* createFoe(Foe* f) {
	FoeNode* neu = (FoeNode*)malloc(sizeof(FoeNode));
	if (neu != NULL) {
		neu->f = f;
		neu->prev = NULL;
		neu->next = NULL;
	}
	return neu;
}

FoeNode* findFoeByName(FOE_DLL* list, const char* name)
{
	FoeNode* curr = list->head;
	while (curr != NULL) {
		if (curr->f->E.name == name) {
			return curr;
		}
		curr = curr->next;
	}
	return NULL;
}

// Inserts a Foe into the list
static void insertFoeIntoList(Player* p, OBJ_DLL* ol, FOE_DLL* fl, const char* name, unsigned int hp, unsigned int atk, unsigned int def, unsigned int xp, unsigned int loot) {
	Foe* f = (Foe*)malloc(sizeof(Foe));
	if (f != NULL) {
		f->E.name = strdup(name);
		f->E.hp = hp;
		f->E.atk = atk;
		f->E.def = def;
		f->E.xp = xp;
		f->E.pos = generateRandomPos(p, ol, fl);
		f->E.dead = false;
		f->E.fled = false;
		f->loot = loot;
	}
	
	FoeNode* neu = createFoe(f);
	if (fl->tail == NULL) {
		fl->head = neu;
		fl->tail = neu;
	}
	else {
		fl->tail->next = neu;
		neu->prev = fl->tail;
		fl->tail = neu;
	}
}

// Removes an instance of Foe from the list
static void removeFoeNode(FOE_DLL* list, FoeNode* node) {
	node->f->E.pos.occupied = false;
	
	if (node->prev != NULL) {
		node->prev->next = node->next;
	}
	else {
		list->head = node->next;
	}

	if (node->next != NULL) {
		node->next->prev = node->prev;
	}
	else {
		list->tail = node->prev;
	}

	free(node->f);
	free(node);
}

// Finds a foe by a given position
static FoeNode* findFoeByPos(FOE_DLL* list, Pos p) {
	FoeNode* curr = list->head;
	while (curr != NULL) {
		if (curr->f->E.pos.col == p.col && curr->f->E.pos.row == p.row) {
			return curr;
		}
		curr = curr->next;
	}
	return NULL;
}

// Finds a foe by its state of living
static FoeNode* findFoeByStatus(FOE_DLL* list) {
	FoeNode* curr = list->head;
	while (curr != NULL) {
		if (curr->f->E.dead) {
			return curr;
		}
		curr = curr->next;
	}
	return NULL;
}

// Removes a foe if it is dead
void removeFoeByStatus(FOE_DLL* list) {
	FoeNode* node = findFoeByStatus(list);
	if (node != NULL) {
		removeFoeNode(list, node);
	}
}

unsigned int getFoeCount(FOE_DLL* list)
{
	unsigned int size = 0;
	FoeNode* curr = list->head;
	while (curr != NULL) {
		size++;
		curr = curr->next;
	}
	return size;
}

// Empties the doubly linked list
void emptyFoeList(FOE_DLL* list) {
	FoeNode* curr = list->head;
	FoeNode* next;

	while (curr != NULL) {
		next = curr->next;
		free(curr);
		curr = next;
	}
}

// Destructor
void destroyFoeList(FOE_DLL* list) {
	emptyFoeList(list);
	free(list);
}