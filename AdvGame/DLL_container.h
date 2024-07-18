#pragma once

#include "entities.h"

// OBJECT-STORING DOUBLY LINKED LIST

typedef struct ObjNode {
	Object* o;
	struct ObjNode* prev;
	struct ObjNode* next;
} ObjNode;

typedef struct OBJ_DLL {
	ObjNode* head, * tail;
} OBJ_DLL;

// ENEMY-STORING DOUBLY LINKED LIST

typedef struct FoeNode {
	Foe* f;
	struct FoeNode* prev;
	struct FoeNode* next;
} FoeNode;

typedef struct FOE_DLL {
	FoeNode* head, * tail;
} FOE_DLL;

// METHODS RELATED TO OBJECT_DLL

static OBJ_DLL* createObjList();

static ObjNode* findObjByPos(OBJ_DLL* list, Pos p);

static void insertObjIntoList(OBJ_DLL* olist, FOE_DLL* flist, char sign, unsigned int eff, Player* p);
static void emptyObjList(OBJ_DLL* list);
static void destroyObjList(OBJ_DLL* list);
static void removeObjNode(OBJ_DLL* list, ObjNode* node);

// METHODS RELATED TO FOE_DLL

static FOE_DLL* createFoeList();

static FoeNode* createFoe(Foe* e);
static FoeNode* findFoeByName(FOE_DLL* list, const char* name);
static FoeNode* findFoeByPos(FOE_DLL* list, Pos p);
static FoeNode* findFoeByStatus(FOE_DLL* list);

static void emptyFoeList(FOE_DLL* list);
static void destroyFoeList(FOE_DLL* list);
static void insertFoeIntoList(Player* p, OBJ_DLL* ol, FOE_DLL* fl, const char* name, unsigned int hp, unsigned int atk, unsigned int def, unsigned int xp, unsigned int loot);
static void removeFoeNode(FOE_DLL* list, FoeNode* node);
static void removeFoeByStatus(FOE_DLL* list);

static unsigned int getFoeCount(FOE_DLL* list);