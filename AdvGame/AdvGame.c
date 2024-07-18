#include "game_loop.h"

#define MAP_SIZE 15

void main(void)
{
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



	MainMenu(c, map, p, objList, foeList);

	destroyFoeList(foeList);
	destroyObjList(objList);

	free(p);
}