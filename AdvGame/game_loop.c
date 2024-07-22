#include "skeleton.c"

// prototype functions for the game loop and the main menu since they are intertwined
static void MainMenu(char getc, char map[15][15], Player* p, OBJ_DLL* objList, FOE_DLL* foeList);
static void GameLoop(char getc, char map[15][15], Player* p, OBJ_DLL* objList, FOE_DLL* foeList);

static void checkWinCondition(Player* p, FOE_DLL* foeList) {
	FoeNode* curr = foeList->head;
	while (curr != NULL) {
		if (curr == (findFoeByName(foeList, "Jimmy")) && curr->f->E.dead) {
			printf("You've beaten Jimmy. You win!\n");
			saveData(p);
			exit(0);
		}
		curr = curr->next;
	}
}

static void GameOver(Player* p, FOE_DLL* foeList, OBJ_DLL* objList, char input, char map[15][15]) {

	printf("You died!\n");
	p->deathCount++;																			// Death count is incremented by 1
	Sleep(2000);
	emptyFoeList(foeList);																		// Both of the containers get emptied
	emptyObjList(objList);
	if (p->deathCount == 10) {																	// Player can die 10 times in total. After that, the game restarts.
		const char* msg = "GAME OVER";
		for (int i = 0; i < sizeof(msg); i++) {
			printf("%c ", msg[i]);
			Sleep(666);
			MainMenu(input, map, p, objList, foeList);
		}
	}
	else {
		p->E.hp = PLAYER_MAX_HP;
		p->mp = PLAYER_MAX_MP;
		p->E.pos.col = p->E.pos.row = 7;
		p->E.dead = false;

		placeObjectsOnMap(objList, foeList, p, 5);
	}
}

static void NewGame(char getc, char map[15][15], Player* p, OBJ_DLL* objList, FOE_DLL* foeList) {
	p = initPlayer();																			// Player is initialized.

	objList = createObjList();																	// Object list is initialized here,
	placeObjectsOnMap(objList, foeList, p, 5);													// and instances of objects are added.

	foeList = (FOE_DLL*)createFoeList();																	// Same with the Foe list,
	breedFoes(p, objList, foeList);																// and foes.

	GameLoop(getc, map, p, objList, foeList);													// Everything's set. LET THE GAME BEGIN!!!
}

static void LoadGame(char getc, char map[15][15], Player* p, OBJ_DLL* objList, FOE_DLL* foeList) {
	initPlayerMagic(p);

	objList = createObjList();
	placeObjectsOnMap(objList, foeList, p, 5);

	foeList = (FOE_DLL*)createFoeList();
	breedFoes(p, objList, foeList);

	GameLoop(getc, map, p, objList, foeList);
}

// the game loop
static void GameLoop(char getc, char map[15][15], Player* p, OBJ_DLL* objList, FOE_DLL* foeList) {
	static bool jimmySummoned = false;															// A logic value which determines whether Jimmy is on the field or not (to prevent flooding the map with the endgame bozo).

	while (1) {
		removeFoeByStatus(foeList);																// In case there was a fight, the corpses need to be taken care of
		system("cls");																			// Clears the console for the new output
		drawMap(map, p, objList, foeList);														// Draws the map (player, foes, objects, unoccupied positions)
		showPlayerInfo(p);																		// A simple UI, which shows the current and max. stats of player, along with the money they possess
		narrate(map, p);																		// A desription of the territory the character is on (related buffs may be implemented later)

		bool acted = playerAction(getc, map, p, objList, foeList);								// This function determines if the player has "moved" in any way possible.
		if (acted) {
			moveFoes(foeList, objList, p);														// If so, the enemies move, too (but their movement is randomized, quasi-wandering).
			moveJimmy(foeList, p);																// If Jimmy is present, he moves towards the player (based on the absolute values of the hor. and vert. distance between them)

			if (getFoeCount(foeList) == 0) {													// Should the player clear the map, they are granted another chance to gain more power
				placeObjectsOnMap(objList, foeList, p, 5);
				breedFoes(p, objList, foeList);
			}
			if (p->E.hp < PLAYER_MAX_HP)
				placeObjectsOnMap(objList, foeList, p, 1);										// Loot rain for more power!
		}

		if (p->E.hp == 0) {
			GameOver(p, foeList, objList, getc, map);
		}

		checkWinCondition(p, foeList);

		Sleep(10);
	}
}

// the main menu and its mechanism
static void MainMenu(char getc, char map[15][15], Player* p, OBJ_DLL* objList, FOE_DLL* foeList) {
	char choice[3] = { '#', ' ', ' ' };
	const char* title = "POWERHUNGER";

	while (1) {
		system("cls");
		printf("%s\n\t%c New Game\n\t%c Load Game\n\t%c Delete Save File\n\n", title, choice[0], choice[1], choice[2]);

		getc = getch();
		// the player may "move" the hashtag by 1 step fwd/bwd
		switch (getc) {
		case 'w':
			if (choice[0] == '#') {
				choice[2] = '#';
				choice[0] = ' ';
			}
			else if (choice[1] == '#') {
				choice[0] = '#';
				choice[1] = ' ';
			}
			else {
				choice[1] = '#';
				choice[2] = ' ';
			}
			break;
		case 's':
			if (choice[0] == '#') {
				choice[1] = '#';
				choice[0] = ' ';
			}
			else if (choice[1] == '#') {
				choice[2] = '#';
				choice[1] = ' ';
			}
			else {
				choice[0] = '#';
				choice[2] = ' ';
			}
			break;
		case 13: // Enter key
			if (choice[0] == '#') {
				NewGame(getc, map, p, objList, foeList);
			}

			else if (choice[1] == '#') {
				if (loadData(p) == -1) {															// Should a save file not exist
					NewGame(getc, map, p, objList, foeList);
				}
				else {																				// If there is a save file,
					LoadGame(getc, map, p, objList, foeList);
				}
			}
			else {
				deleteData("save.dat");																// Save file is deleted
			}
			break;
		case 27: exit(1); break;
		}
	}
}