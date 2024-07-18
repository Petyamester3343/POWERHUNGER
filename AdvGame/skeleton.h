#pragma once
#pragma warning(disable:4996)

#include "object_dll.c"
#include "foe_dll.c"
#include "game_data.h"
#include "levelup_sys.h"
#include "battle_sys.h"

#include <conio.h>
#include <math.h>
#include <time.h>

static void initPlayerMagic(Player* p);
static void playerChoosesAction(Player* p, Foe* f);
static void foeChoosesAction(Player* p, Foe* f);
static void playerCastsMagic(Player* p, Foe* f, Magia m);

// draws the map
static void drawMap(char map[15][15], Player* P, OBJ_DLL* objList, FOE_DLL* foeList) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // to manipulate the color of the output
	system("cls");

	for (unsigned int i = 0; i < MAP_SIZE; i++) {
		for (unsigned int j = 0; j < MAP_SIZE; j++) {
			bool isDrawn = false;
			bool isMultipleObjects = false; // Flag to check if there are multiple objects at this position

			// Draw player
			if (i == P->E.pos.col && j == P->E.pos.row) {
				SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
				printf("X \x1b[0m");
				isDrawn = true;
			}

			// Draw objects
			ObjNode* currObj = objList->head;
			int count = 0;
			while (currObj != NULL) {
				if (currObj->o->pos.col == i && currObj->o->pos.row == j) {
					count++;
					if (count == 1 && !isDrawn) {
						SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN);
						printf("%c \x1b[0m", currObj->o->sign);
						isDrawn = true;
					}
					else if (count > 1) {
						isMultipleObjects = true; // Mark that there are multiple objects at this position
					}
				}
				currObj = currObj->next;
			}

			// Draw foes
			FoeNode* currFoe = foeList->head;
			while (currFoe != NULL) {
				if (currFoe->f->E.pos.col == i && currFoe->f->E.pos.row == j) {
					if (!isDrawn) {
						SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
						printf("! \x1b[0m");
						isDrawn = true;
					}
					else {
						// Foe cannot occupy this cell because it's already occupied
						isDrawn = false;
						break; // Exit the loop to prevent further checks
					}
				}
				currFoe = currFoe->next;
			}

			if (!isDrawn) {
				printf("%c ", map[i][j]);
			}
		}
		printf("\n");
	}
}

static void moveFoes(FOE_DLL* foeList, OBJ_DLL* objList, Player* p) {
	FoeNode* current = foeList->head;
	while (current != NULL) {
		bool moved = false;
		Pos newPos;
		Pos prevPos = current->f->E.pos;
		prevPos.occupied = false;
		do {
			newPos = prevPos;
			switch (rand() % 4) { // 0 = up, 1 = down, 2 = left, 3 = right
			case 0: // Move up
				if (newPos.col > 0) {
					newPos.occupied = false;
					newPos.col -= 1;
					newPos.occupied = true;
				}
				break;
			case 1: // Move down
				if (newPos.col < 14) {
					newPos.occupied = false;
					newPos.col += 1;
					newPos.occupied = true;
				}
				break;
			case 2: // Move left
				if (newPos.row > 0) {
					newPos.occupied = false;
					newPos.row -= 1;
					newPos.occupied = true;
				}
				break;
			case 3: // Move right
				if (newPos.row < 14) {
					newPos.occupied = false;
					newPos.row += 1;
					newPos.occupied = true;
				}
				break;
			}
		} while ((findFoeByPos(foeList, newPos) != NULL) && findObjByPos(objList, newPos) != NULL && newPos.occupied); // Retry if occupied

		// Move to new position if it's not occupied
		if (findFoeByPos(foeList, newPos) == NULL && findObjByPos(objList, newPos) == NULL) {
			current->f->E.pos = newPos;
			moved = true;
		}
		else {
			current->f->E.pos = prevPos;
			moved = false;
		}

		if (current->f->E.pos.col == p->E.pos.col && current->f->E.pos.row == p->E.pos.row)
			foeChoosesAction(p, current->f);

		if (moved)
			printf("%s moved (X: %d, Y: %d)\n", current->f->E.name, current->f->E.pos.col+1, current->f->E.pos.row+1);
		else
			printf("%s passed this turn (X: %d, Y: %d)\n", current->f->E.name, current->f->E.pos.col+1, current->f->E.pos.row+1);

		current = current->next;
		Sleep(166);
	}
}

// initializes the player
static Player* initPlayer() {
	Player* p = (Player*)malloc(sizeof(Player));

	if (p != NULL) {
		p->E.name = strdup("Aleister");
		p->money = 0;
		defPlayerValRestore(p);
		initPlayerMagic(p);
	}

	return p;
}

static void initMap(char map[MAP_SIZE][MAP_SIZE]) {
	FILE* mapFP = fopen("map.txt", "r");

	if (mapFP == NULL) {
		perror("ERROR OPENING FILE");
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < MAP_SIZE; i++) {
		for (int j = 0; j < MAP_SIZE; j++) {
			int c = fgetc(mapFP);
			if (c == EOF) {
				fprintf(stderr, "Unexpected end of file!\n");
				fclose(mapFP);
				exit(EXIT_FAILURE);
			}
			map[i][j] = (char)c;
		}
		fgetc(mapFP);
	}

	fclose(mapFP);
}

static void placeObjectsOnMap(OBJ_DLL* list, FOE_DLL* fl, Player* p) {
	for (int i = 0; i < 5; i++) {
		Object* o = (Object*)malloc(sizeof(Object));
		if (o != NULL) {
			switch (rand() % 6) {
			case 0: {
				insertObjIntoList(list, fl, 'S', 5, p);
				break;
			}
			case 1: {
				insertObjIntoList(list, fl, 'A', 5, p);
				break;
			}
			case 2: {
				insertObjIntoList(list, fl, '+', 5, p);
				break;
			}
			case 3: {
				insertObjIntoList(list, fl, '/', 5, p);
				break;
			}
			case 4: {
				insertObjIntoList(list, fl, 'M', 5, p);
				break;
			}
			case 5: {
				insertObjIntoList(list, fl, '&', 10, p);
			}
			}
		}
	}
}

static void playerSeeksObj(Player* p, OBJ_DLL* list) {
	ObjNode* currObjNode = list->head;
	while (currObjNode != NULL) {
		if (currObjNode->o->pos.col == p->E.pos.col && currObjNode->o->pos.row == p->E.pos.row) {
			ObjNode* nextObjNode = currObjNode->next;

			// Found the object
			switch (currObjNode->o->sign) {
			case '+':
				printf("%s found some ELixir! HP +%d\n", p->E.name, currObjNode->o->eff);
				if ((currObjNode->o->eff + p->E.hp) >= PLAYER_MAX_HP) {
					printf("Health overcharge!\n");
					p->E.hp += currObjNode->o->eff;
				}
				else {
					printf("You are healed!\n");
					p->E.hp = PLAYER_MAX_HP;
				}
				currObjNode->o->found = true;
				break;
			case 'S':
				printf("%s found a sturdy and sharp blade! ATK +%d\n", p->E.name, currObjNode->o->eff);
				p->E.atk += currObjNode->o->eff;
				currObjNode->o->found = true;
				break;
			case 'A':
				printf("%s found an Aegis Core! DEF +%d\n", p->E.name, currObjNode->o->eff);
				p->E.def += currObjNode->o->eff;
				currObjNode->o->found = true;
				break;
			case '/':
				printf("%s found an Arcane Tome! XP +%d ", p->E.name, currObjNode->o->eff);
				xp_gain(currObjNode->o->eff, p, false);
				currObjNode->o->found = true;
				break;
			case 'M':
				printf("%s found some Blue Spike! MP +%d\n", p->E.name, currObjNode->o->eff);
				if ((currObjNode->o->eff + p->mp) >= PLAYER_MAX_MP) {
					printf("Mana overcharge!\n");
					p->mp += currObjNode->o->eff;
				}
				else {
					printf("Your mana has been replenished!\n");
					p->mp = PLAYER_MAX_MP;
				}
				currObjNode->o->found = true;
				break;
			case '&':
				if (rand() % 2 == 0) {
					if (rand() % 3 == 0) {
						if (rand() % 5 == 0) {
							if (rand() % 7 == 0) {
								if (rand() % 11 == 0) {
									printf("%s has found a Mana Potion! Its warranty is still intact! MP +%d\n", p->E.name, currObjNode->o->eff);
									p->mp += currObjNode->o->eff;
								}
								printf("%s has found an Ancient Diary! XP +%d ", p->E.name, currObjNode->o->eff);
								xp_gain(currObjNode->o->eff, p, false);
							}
							printf("%s has found an Antaresian Shield! DEF +%d\n", p->E.name, currObjNode->o->eff);
							p->E.def += currObjNode->o->eff;
						}
						printf("%s has found a Titanium Sword! ATK +%d\n", p->E.name, currObjNode->o->eff);
						p->E.atk += currObjNode->o->eff;
					}
					printf("%s has found some Mac & Cheese with Chicken Nuggets! It's still edible! HP +%d\n", p->E.name, currObjNode->o->eff);
					p->E.hp += currObjNode->o->eff;
				}
				else {
					printf("%s has found nothing in that chest...\nExcept for a manga with a topless girl and butterflies on its cover...\nHe left it there, for it was a bad omen.\n", p->E.name);
				}
				currObjNode->o->found = true;
				break;
			}

			Sleep(500);

			if (currObjNode->o->found == true) {
				if (currObjNode == list->head && currObjNode == list->tail) {
					// Case: Only one node in the list
					list->head = NULL;
					list->tail = NULL;
				}
				else if (currObjNode == list->head) {
					// Case: Deleting the head node
					list->head = currObjNode->next;
					list->head->prev = NULL;
				}
				else if (currObjNode == list->tail) {
					// Case: Deleting the tail node
					list->tail = currObjNode->prev;
					list->tail->next = NULL;
				}
				else {
					// Case: Deleting a node in between
					currObjNode->prev->next = currObjNode->next;
					currObjNode->next->prev = currObjNode->prev;
				}

				// Free the memory allocated to the node
				free(currObjNode);
			}
			currObjNode = nextObjNode;
		}
		else {
			currObjNode = currObjNode->next;
		}
	}
}

// The player takes action upon input (movement, attacking foes in its vicinity)
// It becomes true when they moved
static bool playerAction(char input, Player* p, OBJ_DLL* objList, FOE_DLL* foeList) {
	bool moved = false;

	char exitChoice[2] = { '>', ' ' };
	bool exitChose = false;

	// keyboard input
	input = getch();
	switch (input) {
	case 'w':
		if (p->E.pos.col > 0) {
			p->E.pos.occupied = false;
			p->E.pos.col--;
			p->E.pos.occupied = true;
			moved = true;
		}
		else printf("%s bumped into a wall! (X: %d, Y: %d)\n", p->E.name, p->E.pos.col + 1, p->E.pos.row + 1);
		break;
	case 's':
		if (p->E.pos.col < 14) {
			p->E.pos.occupied = false;
			p->E.pos.col++;
			p->E.pos.occupied = true;
			moved = true;
		}
		else printf("%s bumped into a wall! (X: %d, Y: %d)\n", p->E.name, p->E.pos.col + 1, p->E.pos.row + 1);
		break;
	case 'a':
		if (p->E.pos.row > 0) {
			p->E.pos.occupied = false;
			p->E.pos.row--;
			p->E.pos.occupied = true;
			moved = true;
		}
		else printf("%s bumped into a wall! (X: %d, Y: %d)\n", p->E.name, p->E.pos.col + 1, p->E.pos.row + 1);
		break;
	case 'd':
		if (p->E.pos.row < 14) {
			p->E.pos.occupied = false;
			p->E.pos.row++;
			p->E.pos.occupied = true;
			moved = true;
		}
		else printf("%s bumped into a wall! (X: %d, Y: %d)\n", p->E.name, p->E.pos.col + 1, p->E.pos.row + 1);
		break;
	case 'e': impendingDoom(p, foeList); break;
	case 'l': placeObjectsOnMap(objList, foeList, p); break; // debug
	case '0': saveData(p); break;
	case 27:
		while (!exitChose) {
			system("cls");
			printf("Are you sure you want to exit? (Any unsaved data will be lost.)\n\t%c YES\n\t%c NO", exitChoice[0], exitChoice[1]);
			char exitInput = getch();
			switch (exitInput) {
			case 'w':
				if (exitChoice[0] == '>') {
					exitChoice[0] = ' ';
					exitChoice[1] = '>';
				}
				else {
					exitChoice[0] = '>';
					exitChoice[1] = ' ';
				}
				break;
			case 's':
				if (exitChoice[1] == '>') {
					exitChoice[0] = '>';
					exitChoice[1] = ' ';
				}
				else {
					exitChoice[0] = ' ';
					exitChoice[1] = '>';
				}
				break;
			case 13:
				exitChose = true;
				if (exitChoice[0] == '>') exit(177013);
				else break;
			}
		}
	}

	if (moved) {
		printf("%s moved! (X: %d, Y: %d)\n", p->E.name, p->E.pos.col + 1, p->E.pos.row + 1);
		playerSeeksObj(p, objList);
	}

	return moved;
}

// A method which creates Foes based on the Player's level
static void breedFoes(Player* p, OBJ_DLL* objList, FOE_DLL* foeList) {
	bool jimmySummoned = false;

	for (int i = 0; i < 5; i++) {
		if (p->lv <= 3)
			insertFoeIntoList(p, objList, foeList, "Slime", SLIME_HP, SLIME_ATK, SLIME_DEF, SLIME_XP, (unsigned int)rand() % 10 + 5);

		if (p->lv >= 4) {
			if (rand() % 2 == 0) insertFoeIntoList(p, objList, foeList, "Goblin", GOBLIN_HP, GOBLIN_ATK, GOBLIN_DEF, GOBLIN_XP, (unsigned int)rand() % 20 + 10);
			else insertFoeIntoList(p, objList, foeList, "Slime", SLIME_HP, SLIME_ATK, SLIME_DEF, SLIME_XP, (unsigned int)rand() % 10 + 5);
		}

		if (p->lv >= 6) {
			if (rand() % 2 == 0) insertFoeIntoList(p, objList, foeList, "Acolyte", ACOLYTE_HP, ACOLYTE_ATK, ACOLYTE_DEF, ACOLYTE_XP, (unsigned int)rand() % 40 + 20);
			else insertFoeIntoList(p, objList, foeList, "Goblin", GOBLIN_HP, GOBLIN_ATK, GOBLIN_DEF, GOBLIN_XP, (unsigned int)rand() % 20 + 10);
		}

		if (p->lv >= 8) {
			if (rand() % 2 == 0) insertFoeIntoList(p, objList, foeList, "Thwarted Self", THWARTED_SELF_HP, THWARTED_SELF_ATK, THWARTED_SELF_DEF, THWARTED_SELF_XP, (unsigned int)rand() % 80 + 40);
			else insertFoeIntoList(p, objList, foeList, "Acolyte", ACOLYTE_HP, ACOLYTE_ATK, ACOLYTE_DEF, ACOLYTE_XP, (unsigned int)rand() % 40 + 20);
		}
	}

	if (p->lv >= 10 && !jimmySummoned) {
		insertFoeIntoList(p, objList, foeList, "Jimmy", JIMMY_HP, JIMMY_ATK, JIMMY_DEF, 0, 0);
		foeList->tail->f->E.pos.col = foeList->tail->f->E.pos.row = 14;
		jimmySummoned = true;
	}
}

// debug feature
void showPlayerInfo(Player* p) {
	unsigned int req = (p->lv <= 4) ? (unsigned int)pow(2, (double)p->lv) * 15 : (unsigned int)pow(2, (double)p->lv) * 10;
	printf("HP:\t%d/%d\n", p->E.hp, PLAYER_MAX_HP);
	printf("MP:\t%d/%d\n", p->mp, PLAYER_MAX_MP);
	printf("ATK:\t%d/%d\n", p->E.atk, PLAYER_ATK);
	printf("DEF:\t%d/%d\n", p->E.def, PLAYER_DEF);
	printf("LV:\t%d (XP:\t%d/%d)\n", p->lv, p->E.xp, req);
	printf("$:\t%d\n", p->money);
}

// debug feature
static void narrate(char map[15][15], Player* p) {
	printf("Current location: ");
	switch (map[p->E.pos.col][p->E.pos.row]) {
	case 'F': printf("Forest\n"); break;
	case 'R': printf("Riverside\n"); break;
	case 'T': printf("Town\n"); break;
	case 'V': printf("Valley\n"); break;
	case 'D': printf("Desert\n"); break;
	}
}

static void initPlayerMagic(Player* p) {
	// initializing magic
	for (unsigned int i = 0; i < 5; i++) {
		switch (i) {
		case 0:
			p->M[i].magName = strdup("Homing Fireball");
			p->M[i].magATK = FIREBALL_ATK;
			p->M[i].magCost = FIREBALL_COST;
			p->M[i].magLVReq = FIREBALL_LV_REQ;
			break;
		case 1:
			p->M[i].magName = strdup("Blade of the Blizzard Queen");
			p->M[i].magATK = BLIZZARD_ATK;
			p->M[i].magCost = BLIZZARD_COST;
			p->M[i].magLVReq = BLIZZARD_LV_REQ;
			break;
		case 2:
			p->M[i].magName = strdup("Raijin's Wrath");
			p->M[i].magATK = LIGHTNING_ATK;
			p->M[i].magCost = LIGHTNING_COST;
			p->M[i].magLVReq = LIGHTNING_LV_REQ;
			break;
		case 3:
			p->M[i].magName = strdup("Gravity Bomb");
			p->M[i].magATK = GRAV_BOMB_ATK;
			p->M[i].magCost = GRAV_BOMB_COST;
			p->M[i].magLVReq = GRAV_BOMB_LV_REQ;
			break;
		case 4:
			p->M[i].magName = strdup("Raining Spears of Justice");
			p->M[i].magATK = RSOJ_ATK;
			p->M[i].magCost = RSOJ_COST;
			p->M[i].magLVReq = RSOJ_LV_REQ;
			break;
		}
	}
}