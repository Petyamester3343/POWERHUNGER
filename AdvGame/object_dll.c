#include "DLL_container.h"
#include "pos_gen.h"

#include <stdlib.h>

// Constructor
OBJ_DLL* createObjList()
{
	OBJ_DLL* list = (OBJ_DLL*)malloc(sizeof(OBJ_DLL));
	if (list != NULL) {
		list->head = NULL;
		list->tail = NULL;
	}
	return list;
}

// Finds an object by a given position
ObjNode* findObjByPos(OBJ_DLL* list, Pos p)
{
	ObjNode* curr = list->head;
	while (curr != NULL) {
		if (curr->o->pos.col == p.col && curr->o->pos.row == p.row) {
			return curr;
		}
		curr = curr->next;
	}
	return NULL;
}

// Inserts an instance of object into the list
void insertObjIntoList(OBJ_DLL* olist, FOE_DLL* flist, char sign, unsigned int eff, Player* p)
{
	Object* o = (Object*)malloc(sizeof(Object));
	if (o != NULL) {
		o->sign = sign;
		o->eff = eff;
		o->pos = generateRandomPos(p, olist, flist);
		o->pos.occupied = true;
	}
	
	ObjNode* neu = (ObjNode*)malloc(sizeof(ObjNode));
	if (neu != NULL) {
		neu->o = o;
		neu->prev = NULL;
		neu->next = NULL;

		if (olist->head == NULL) {
			olist->head = neu;
			olist->tail = neu;
		}
		else {
			olist->tail->next = neu;
			neu->prev = olist->tail;
			olist->tail = neu;
		}
	}
}

// Empties the object list
void emptyObjList(OBJ_DLL* list)
{
	ObjNode* curr = list->head;
	ObjNode* next;

	while (curr != NULL) {
		next = curr->next;
		free(curr);
		curr = next;
	}
}

// Destructor
void destroyObjList(OBJ_DLL* list) {
	emptyObjList(list);
	free(list);
}

// Removes an instance of Object from the list
void removeObjNode(OBJ_DLL* list, ObjNode* node)
{
	node->o->pos.occupied = false;

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

	free(node);
}
