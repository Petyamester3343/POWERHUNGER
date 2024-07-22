#pragma once

#include "entities.h"

static const int armor_price = 5000;
static const int plasma_gun_price = 5000;
static const int invigorator_price = 3000;

static void go2Shop(Player* p) {
	char choice[5] = { '>', ' ', ' ', ' ', ' ' };
	char exitChoice[2] = { ' ', '>' };

	while (1) {
		system("cls");
		printf("Welcome, weary traveler!\nFeel free to browse through my wares!\n");
		printf("\t%c %s\t->\t\t$%d\n", choice[0], AKARI, RSOJ_PRICE);
		printf("\t%c %s\t->\t\t$%d\n", choice[1], "Aegis Automatized Armor", armor_price);
		printf("\t%c %s\t\t->\t\t$%d\n", choice[2], "Plasma Railgun", plasma_gun_price);
		printf("\t%c %s\t\t->\t\t$%d\n", choice[3], "Vigor Enhancer", invigorator_price);
		printf("\t%c %s\n\n", choice[4], "EXIT SHOP");

		bool staying = false;
		char input = getch();
		switch (input) {
		case 'w':
			if (choice[0] == '>') {
				choice[4] = '>';
				choice[0] = ' ';
			}
			else if (choice[1] == '>') {
				choice[0] = '>';
				choice[1] = ' ';
			}
			else if (choice[2] == '>') {
				choice[1] = '>';
				choice[2] = ' ';
			}
			else if (choice[3] == '>') {
				choice[2] = '>';
				choice[3] = ' ';
			}
			else if (choice[4] == '>'){
				choice[3] = '>';
				choice[4] = ' ';
			}
			break;
		case 's':
			if (choice[0] == '>') {
				choice[1] = '>';
				choice[0] = ' ';
			}
			else if (choice[1] == '>') {
				choice[2] = '>';
				choice[1] = ' ';
			}
			else if (choice[2] == '>') {
				choice[3] = '>';
				choice[2] = ' ';
			}
			else if (choice[3] == '>') {
				choice[4] = '>';
				choice[3] = ' ';
			}
			else if (choice[4] == '>') {
				choice[0] = '>';
				choice[4] = ' ';
			}
			break;
		case 13:
			if (choice[0] == '>' && p->money >= RSOJ_PRICE) {
				p->money -= RSOJ_PRICE;
				p->M[5].magName = strdup(AKARI);
				p->M[5].magATK = RSOJ_ATK;
				p->M[5].magCost = RSOJ_COST;
				p->M[5].magLVReq = RSOJ_LV_REQ;
				p->M[5].acquired = true;
			}
			if (choice[1] == '>' && p->money >= armor_price) {
				p->money -= armor_price;
				p->E.def += 30;
				printf("%s has purchased the Aegis Automatized Armor! DEF +30\n", p->E.name);
				break;
			}
			if (choice[2] == '>' && p->money >= plasma_gun_price) {
				p->money -= plasma_gun_price;
				p->E.atk += 30;
				printf("%s has purchased the Plasma Railgun! ATK +30\n", p->E.name);
				break;
			}
			if (choice[3] == '>' && p->money >= invigorator_price) {
				p->money -= invigorator_price;
				PLAYER_MAX_HP += 30;
				p->E.hp = PLAYER_MAX_HP;
				PLAYER_MAX_MP += 30;
				p->mp = PLAYER_MAX_MP;
				printf("%s has purchased the Vigor Enhancer! HP & MP +30 PERMANENTLY!\n", p->E.name);
				break;
			}
			if (choice[4] == '>') {
				while (!staying) {
					system("cls");
					printf("Are you sure you want to leave the Shop?\n\t%c YES\n\t%c NO", exitChoice[0], exitChoice[1]);
					input = getch();
					switch (input) {
					case 'w':
						if (exitChoice[1] == '>') {
							exitChoice[0] = '>';
							exitChoice[1] = ' ';
						}
						else if (exitChoice[0] == '>') {
							exitChoice[1] = '>';
							exitChoice[0] = ' ';
						}
						break;
					case 's':
						if (exitChoice[0] == '>') {
							exitChoice[1] = '>';
							exitChoice[0] = ' ';
						}
						else if (exitChoice[1] == '>') {
							exitChoice[0] = '>';
							exitChoice[1] = ' ';
						}
						break;
					case 13:
						if (exitChoice[0] == '>') {
							return;
						}
						if (exitChoice[1] == '>') {
							staying = true;
						}
					}
				}
			}
		}
	}

}
