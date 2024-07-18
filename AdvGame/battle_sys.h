#pragma once

#include <stdio.h>
#include <windows.h>
#include <conio.h>

static void playerChoosesAction(Player* p, Foe* f);

// determines if there is a Foe in the Player's vicinity
static bool checkPlayerSurrounding(Foe* f, Player* p) {

	return
		(f->E.pos.col == p->E.pos.col + 1 && f->E.pos.row == p->E.pos.row) ||
		(f->E.pos.col == p->E.pos.col - 1 && f->E.pos.row == p->E.pos.row) ||
		(f->E.pos.col == p->E.pos.col && f->E.pos.row == p->E.pos.row + 1) ||
		(f->E.pos.col == p->E.pos.col && f->E.pos.row == p->E.pos.row - 1) ||
		(f->E.pos.col == p->E.pos.col + 1 && f->E.pos.row == p->E.pos.row - 1) ||
		(f->E.pos.col == p->E.pos.col - 1 && f->E.pos.row == p->E.pos.row + 1) ||
		(f->E.pos.col == p->E.pos.col + 1 && f->E.pos.row == p->E.pos.row + 1) ||
		(f->E.pos.col == p->E.pos.col - 1 && f->E.pos.row == p->E.pos.row - 1);
}

// Player attacks Foe
static void playerAttack(Player* p, Foe* f) {
	if (p->E.hp != 0 && f->E.hp != 0) {
		unsigned int pDMG = (p->E.atk > f->E.def) ? (p->E.atk - f->E.def) : 0;
		unsigned int multi = 2;
		unsigned int pCrit_DMG = pDMG * multi;
		int rnd = rand() % 50;
		printf("%s attacks!\n", p->E.name);
		if (rnd % 2 == 0) {
			if (rnd % 10 == 0) {
				if (pCrit_DMG <= f->E.hp) {
					if (pCrit_DMG > 0) {
						f->E.hp -= pCrit_DMG;
						printf("CRITICAL DAMAGE!\t(%s: %d\t|\t%s: %d)\n", p->E.name, p->E.hp, f->E.name, f->E.hp);
						Sleep(500);
					}
					else {
						f->E.hp -= 0;
						printf("%s tanked %s's attack!\n", f->E.name, p->E.name);
						Sleep(500);
					}
				}
				else {
					f->E.hp = 0;
					f->E.dead = true;
					printf("%s obliterated %s!\n", p->E.name, f->E.name);
					Sleep(500);
				}
			}
			else {
				if (pDMG <= f->E.hp) {
					if (pDMG > 0) {
						f->E.hp -= pDMG;
						printf("Attack successful!\t(%s: %d\t|\t%s: %d)\n", p->E.name, p->E.hp, f->E.name, f->E.hp);
						Sleep(500);
					}
					else {
						f->E.hp -= 0;
						printf("%s tanked %s's attack!\n", f->E.name, p->E.name);
						Sleep(500);
					}
				}
				else {
					f->E.hp = 0;
					f->E.dead = true;
					printf("%s smoked %s!\n", p->E.name, f->E.name);
					Sleep(500);
				}
			}
		}
		else {
			printf("%s missed!\n", p->E.name);
			Sleep(500);
		}
	}

	if (f->E.dead || p->E.dead) {
		if (f->E.dead) {
			xp_gain(f->E.xp, p, true);
			looting(p, f->loot);
			Sleep(500);
		}
		return;
	}
}

// Foe attacks Player
static void foeAttack(Player* p, Foe* f) {
	if (p->E.hp != 0 && f->E.hp != 0) {
		unsigned int fDMG = (f->E.atk < p->E.def) ? 0 : (f->E.atk - p->E.def);
		int multi = 2;
		unsigned int fCrit_DMG = fDMG * multi;

		int rnd = rand() % 50;
		printf("%s attacks!\n", f->E.name);
		if (rnd % 2 == 0) {
			if (rnd % 10 == 0) {
				if (fCrit_DMG <= p->E.hp) {
					if (fCrit_DMG > 0) {
						p->E.hp -= fCrit_DMG;
						printf("CRITICAL DAMAGE!\t(%s: %d\t|\t%s: %d)\n", p->E.name, p->E.hp, f->E.name, f->E.hp);
						Sleep(500);
					}
					else {
						p->E.hp -= 0;
						printf("%s tanked %s's attack!\n", p->E.name, f->E.name);
						Sleep(500);
					}
				}
				else {
					p->E.hp = 0;
					p->E.dead = true;
					printf("%s roasted %s!\n", f->E.name, p->E.name);
					Sleep(500);
				}
			}
			else {
				if (fDMG <= p->E.hp) {
					if (fDMG > 0) {
						p->E.hp -= fDMG;
						printf("Attack successful!\t(%s: %d\t|\t%s: %d)\n", p->E.name, p->E.hp, f->E.name, f->E.hp);
						Sleep(500);
					}
					else {
						p->E.hp -= 0;
						printf("%s tanked %s's attack!\n", p->E.name, f->E.name);
						Sleep(500);
					}
				}
				else {
					p->E.hp = 0;
					p->E.dead = true;
					printf("%s ate %s for breakfast!\n", f->E.name, p->E.name);
					Sleep(500);
				}
			}
		}
		else {
			printf("%s missed!\n", f->E.name);
			Sleep(500);
		}
	}

	if (p->E.dead || f->E.dead) {
		return;
	}
}

// Player casts magic on a Foe in exchange for 20 MP
static void playerCastsMagic(Player* p, Foe* f, Magia m) {
	if (p->E.hp != 0 && f->E.hp != 0) {
		unsigned int multi = 2;
		unsigned int pCrit_MAG_DMG = m.magATK * multi;
		int rnd = rand() % 50;
		if (p->mp >= m.magCost) {
			p->mp -= m.magCost;
			printf("%s sacrifices %d MP to cast %s on %s! (MP: %d)\n", p->E.name, m.magCost, m.magName, f->E.name, p->mp);
			if (rnd % 2 == 0) {
				if (rnd % 10 == 0) {
					if (pCrit_MAG_DMG > f->E.def) {
						if (pCrit_MAG_DMG <= f->E.hp) {
							f->E.hp -= pCrit_MAG_DMG;
						}
						else {
							f->E.hp = 0;
							f->E.dead = true;
						}
					}
					else {
						f->E.hp -= 0;
						printf("%s tanked %s's attack!\n", f->E.name, p->E.name);
					}

					printf("CRITICAL DAMAGE!\t(%s: %d\t|\t%s: %d)\n", p->E.name, p->E.hp, f->E.name, f->E.hp);
					Sleep(500);
				}
				else {
					if (m.magATK > f->E.def) {
						if (m.magATK <= f->E.hp) {
							f->E.hp -= m.magATK;
							printf("Attack successful!\t(%s: %d\t|\t%s: %d)\n", p->E.name, p->E.hp, f->E.name, f->E.hp);
							Sleep(500);
						}
						else {
							f->E.hp = 0;
							f->E.dead = true;
							printf("%s smoked %s!\n", p->E.name, f->E.name);
							Sleep(500);
						}
					}
					else {
						f->E.hp -= 0;
						printf("%s tanked %s's attack!\n", f->E.name, p->E.name);
					}
				}
			}
			else {
				printf("%s missed!\n", p->E.name);
				Sleep(500);
			}
		}
		else {
			printf("Not enough MP!\n");
			Sleep(500);
		}
	}

	if (f->E.dead) {
		xp_gain(f->E.xp, p, true);
		looting(p, f->loot / 2);
		return;
	}

	Sleep(500);
}

static void playerChoosesMagic(Player* p, Foe* f) {
	char magChoice[6] = { '>', ' ', ' ', ' ', ' ', ' ' };
	bool chose = false;

	while (p->E.fled == false && !p->E.dead) {
		system("cls");
		printf("Choose your spell:\n\t%c %s\n\t%c %s\n\t%c %s\n\t%c %s\n\t%c %s\n\t%c EXIT\n",
			magChoice[0], p->M[0].magName, magChoice[1], p->M[1].magName, magChoice[2], p->M[2].magName, magChoice[3], p->M[3].magName, magChoice[4], p->M[4].magName, magChoice[5]);
		char in = getch();
		switch (in) {
		case 'w':
			if (magChoice[0] == '>') {
				magChoice[5] = '>';
				magChoice[0] = ' ';
			}
			else if (magChoice[1] == '>') {
				magChoice[0] = '>';
				magChoice[1] = ' ';
			}
			else if (magChoice[2] == '>') {
				magChoice[1] = '>';
				magChoice[2] = ' ';
			}
			else if (magChoice[3] == '>') {
				magChoice[2] = '>';
				magChoice[3] = ' ';
			}
			else if (magChoice[4] == '>') {
				magChoice[3] = '>';
				magChoice[4] = ' ';
			}
			else {
				magChoice[4] = '>';
				magChoice[5] = ' ';
			}
			break;
		case 's':
			if (magChoice[0] == '>') {
				magChoice[1] = '>';
				magChoice[0] = ' ';
			}
			else if (magChoice[1] == '>') {
				magChoice[2] = '>';
				magChoice[1] = ' ';
			}
			else if (magChoice[2] == '>') {
				magChoice[3] = '>';
				magChoice[2] = ' ';
			}
			else if (magChoice[3] == '>') {
				magChoice[4] = '>';
				magChoice[3] = ' ';
			}
			else if (magChoice[4] == '>') {
				magChoice[5] = '>';
				magChoice[4] = ' ';
			}
			else {
				magChoice[0] = '>';
				magChoice[5] = ' ';
			}
			break;
		case 13:
			chose = true;
			if (magChoice[0] == '>') {
				if (p->lv >= p->M[0].magLVReq) {
					playerCastsMagic(p, f, p->M[0]);
					return;
				}
				else {
					printf("%s hasn't unlocked this spell yet!\n", p->E.name);
				}
			}
			else if (magChoice[1] == '>') {
				if (p->lv >= p->M[1].magLVReq) {
					playerCastsMagic(p, f, p->M[1]);
					return;
				}
				else {
					printf("%s hasn't unlocked this spell yet!\n", p->E.name);
				}
			}
			else if (magChoice[2] == '>') {
				if (p->lv >= p->M[2].magLVReq) {
					playerCastsMagic(p, f, p->M[2]);
					return;
				}
				else {
					printf("%s hasn't unlocked this spell yet!\n", p->E.name);
				}
			}
			else if (magChoice[3] == '>') {
				if (p->lv >= p->M[3].magLVReq) {
					playerCastsMagic(p, f, p->M[3]);
					return;
				}
				else {
					printf("%s hasn't unlocked this spell yet!\n", p->E.name);
				}
			}
			else if (magChoice[4] == '>') {
				if (p->lv >= p->M[4].magLVReq) {
					playerCastsMagic(p, f, p->M[4]);
					return;
				}
				else {
					printf("%s hasn't unlocked this spell yet!\n", p->E.name);
				}
			}
			else return;
			Sleep(500);
			break;
		}
	}
}

// Foe acts
static void foeChoosesAction(Player* p, Foe* f) {
	f->E.fled = false;
	bool defChanged = false;

	/*
	1. elem.: ATTACK
	2. elem.: DEFEND
	3. elem.: FLEE
	*/

	while ((f->E.fled == false && p->E.fled == false) && (f->E.hp != 0 && p->E.hp != 0)) {
		switch (rand() % 50) {
		default:
			if (!f->E.fled && !p->E.fled)
				foeAttack(p, f);
			break;
		case 12:
			f->E.def += 2;
			defChanged = true;
			printf("%s hardened its skin!\n", f->E.name);
			Sleep(500);
			break;
		case 25: f->E.fled = true; break;
		}

		if (p->E.def != PLAYER_DEF)
			p->E.def = PLAYER_DEF;

		if (!p->E.fled || !f->E.fled) {
			playerChoosesAction(p, f);
			if (defChanged) f->E.def -= 2;
		}
		else {
			printf("%s has fled the scene!\n", f->E.name);
			f->E.fled = true;
			return;
		}
	}
}

// character can initiate attack after they moved
static void impendingDoom(Player* p, FOE_DLL* foeList) {
	FoeNode* curr = foeList->head;
	while (curr != NULL) {
		if (checkPlayerSurrounding(curr->f, p)) {
			printf("%s noticed an instance of %s skulking around him (X: %d, Y: %d)\n", p->E.name, curr->f->E.name, curr->f->E.pos.col + 1, curr->f->E.pos.row + 1);
			playerChoosesAction(p, curr->f);
		}
		curr = curr->next;
	}
}

// Player chooses an action
static void playerChoosesAction(Player* p, Foe* f) {
	char in = ' ';
	char actChoice[5] = { '>', ' ', ' ', ' ', ' ' };
	p->E.fled = false;
	bool chose = false;

	unsigned int tempDEF = p->E.def;
	unsigned int enhDEF = p->E.def + p->E.atk;

	/*
	1. elem.: ATTACK
	2. elem.: DEFEND
	3. elem.: MAGIC
	4. elem.: HEAL
	5. elem.: FLEE
	*/


	system("cls");
	printf("Choose an action:\n\t%c ATTACK\n\t%c DEFEND\n\t%c MAGIC\n\t%c HEAL\n\t%c FLEE\n\n", actChoice[0], actChoice[1], actChoice[2], actChoice[3], actChoice[4]);

	// choose an action
	while (p->E.fled == false && !p->E.dead && !f->E.dead) {
		in = getch();
		switch (in) {
		case 'w':
			if (actChoice[0] == '>') {
				actChoice[4] = '>';
				actChoice[0] = ' ';
			}
			else if (actChoice[1] == '>') {
				actChoice[0] = '>';
				actChoice[1] = ' ';
			}
			else if (actChoice[2] == '>') {
				actChoice[1] = '>';
				actChoice[2] = ' ';
			}
			else if (actChoice[3] == '>') {
				actChoice[2] = '>';
				actChoice[3] = ' ';
			}
			else {
				actChoice[3] = '>';
				actChoice[4] = ' ';
			}
			break;
		case 's':
			if (actChoice[0] == '>') {
				actChoice[1] = '>';
				actChoice[0] = ' ';
			}
			else if (actChoice[1] == '>') {
				actChoice[2] = '>';
				actChoice[1] = ' ';
			}
			else if (actChoice[2] == '>') {
				actChoice[3] = '>';
				actChoice[2] = ' ';
			}
			else if (actChoice[3] == '>') {
				actChoice[4] = '>';
				actChoice[3] = ' ';
			}
			else {
				actChoice[0] = '>';
				actChoice[4] = ' ';
			}
			break;
		case 13:
			chose = true;
			if (actChoice[0] == '>') {
				playerAttack(p, f);
			}
			else if (actChoice[1] == '>') {
				printf("%s strengthtened his defense!", p->E.name);
				if (p->E.def == tempDEF)
					p->E.def = enhDEF;
				Sleep(500);
			}
			else if (actChoice[2] == '>') {
				unsigned int mana = p->mp;
				unsigned int prevMana = mana;
				playerChoosesMagic(p, f);
				mana = p->mp;
				if (mana == prevMana)
					chose = false;
			}
			else if (actChoice[3] == '>') {
				printf("%s ate some bread! HP +20", p->E.name);
				if (p->E.hp < PLAYER_MAX_HP)
					p->E.hp += 20;
				else
					p->E.hp = PLAYER_MAX_HP;
				Sleep(500);
			}
			else {
				p->E.fled = true;
			}
			break;
		}
		system("cls");
		if (!chose)
			printf("Choose an action:\n\t%c ATTACK\n\t%c DEFEND\n\t%c MAGIC\n\t%c HEAL\n\t%c FLEE\n\n", actChoice[0], actChoice[1], actChoice[2], actChoice[3], actChoice[4]);

		if (p->E.fled) {
			printf("%s has fled the scene!\n", p->E.name);
			Sleep(500);
			return;
		}

		if (!f->E.dead && (!f->E.fled || !p->E.fled) && chose) {
			system("cls");
			foeChoosesAction(p, f);
		}
	}

	return;
}