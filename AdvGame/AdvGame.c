#include "game_loop.c"

void main(void)
{
	// time seed for foes that aren't Jimmy, and loot
	srand((unsigned)time(NULL));

	// character map
	char map[MAP_SIZE][MAP_SIZE];
	initMap(map);

	// character input for player movement
	char c = ' ';

	// player declaration and allocation
	Player* p = (Player*)malloc(sizeof(Player));

	// declaration of lists
	OBJ_DLL* objList = createObjList();
	FOE_DLL* foeList = createFoeList();

	// start with the main menu
	MainMenu(c, map, p, objList, foeList);

	// upon exiting
	destroyFoeList(foeList);
	destroyObjList(objList);
	free(p);
}