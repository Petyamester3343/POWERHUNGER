#pragma once

#include "def_vals.h"

#include <stdio.h>
#include <windows.h>

static void saveData(Player* p) {
	FILE* fp = fopen("save.dat", "wb");

	if (fp == NULL) {
		fprintf(stderr, "Cannot open file!\n");
		return;
	}

	fwrite(p, sizeof(Player), 1, fp);
	fwrite(p->E.name, strlen(p->E.name), 1, fp);

	fclose(fp);
	printf("Save data created successfully!\n");
	
	Sleep(500);
}

static int loadData(Player* p) {
	FILE* fp = fopen("save.dat", "rb");

	if (fp == NULL) {
		fprintf(stderr, "Cannot open file!\nStarting new game...\n");
		Sleep(1000);
		return -1;
	}

	if (fseek(fp, 0, SEEK_END) < 0) {
		fclose(fp);
		return -1;
	}

	long val = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	val -= sizeof(Player);

	fread(p, sizeof(Player), 1, fp);
	p->E.name = (char*)calloc((size_t)val + 1, sizeof(char));
	if (p->E.name != NULL) {
		fread(p->E.name, (size_t)val, 1, fp);
	}

	PLAYER_MAX_HP += (p->lv - 1) * LVUP_VAL;
	PLAYER_MAX_MP += (p->lv - 1) * LVUP_VAL;
	PLAYER_ATK += (p->lv - 1) * LVUP_VAL;
	PLAYER_DEF += (p->lv - 1) * LVUP_VAL;

	p->remembers = true;
	
	p->E.dead = false;
	p->E.fled = false;

	fclose(fp);
	
	printf("Data loaded successfully!\n");
	Sleep(1000);

	return 0;
}

static void deleteData(const char* fileName) {
	if (remove(fileName) == 0) {
		printf("Save data deleted successfully!\n");
		Sleep(1000);
	}
	else {
		perror("FILE DELETION ERROR");
		Sleep(1000);
	}
}