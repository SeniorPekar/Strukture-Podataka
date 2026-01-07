#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>

typedef struct tree* treePos;

typedef struct tree {
	int value;
	treePos left;
	treePos right;
}tree;

/*Napisati program koji omoguæava rad s binarnim stablom pretraživanja. Treba omoguæiti unošenje novog elementa , 
ispis elemenata(inorder, postorder, preorder, postorder i level order), brisanje i pronalaženje nekog elemnta.*/

/*. Zadan je niz brojeva 2, 5, 7, 8, 11, 1, 4, 2, 3, 7 koji su spremljeni u èvorove binarnog stabla.
a) Napisati funkciju insert koja dodaje element u stablo tako da se pozivima te funkcije za
sve element zadanog niza brojeva stvori stablo kao na slici Slika 1. Funkcije vraæa
pokazivaè na korijen stabla.
b) Napisati funkciju replace koja æe svaki element stabla zamijeniti sumom elemenata u
njegovom lijevom i desnom podstablu (tj. sumom svih potomaka prije zamjene
vrijednosti u tim potomcima). Npr. stablo sa slike Slika 1 transformirat æe se u stablo na
slici Slika 2.
c) Prepraviti program na naèin da umjesto predefiniranog cjelobrojnog polja korištenjem
funkcije rand() generira sluèajne brojeve u rasponu <10, 90>. Takoðer, potrebno je
upisati u datoteku sve brojeve u inorder prolasku nakon korištenja funkcije iz a), zatim b)
dijela zadatka.
*/

int main() {
	tree root = { .value = 0, .left = NULL, .right = NULL };


	return 0;
}