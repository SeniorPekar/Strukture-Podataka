#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "include/loadList.h" //header ovih funkcija
#include "include/helpFun.h"//header pomocnih funkcija

int loadPathFile(billPosition headBill) {
	char buffer[512] = "";
	FILE* file = fopen("racuni\\racuni.txt", "r");
	if (!file) {
		printf("Greškra pri otvaranju\n");
		return FILE_ISSUE;
	}

	while (fgets(buffer, sizeof(buffer), file)) {
		if (buffer[0] != '\n') {
			trimBuffer(buffer);
			loadBillFiles(headBill, buffer); //uèitaj pojedini raèun, izbriši višak iz stringa i prosljedi ga
		}
	}
	fclose(file);

	return 0;
}

int loadBillFiles(billPosition headBill, char* p) {
	FILE* f = fopen(p, "r");
	if (!f) {
		printf("Greška pri otvaranju datoteke: %s\n", p);
		return FILE_ISSUE;
	}

	datePosition newDate = (datePosition)malloc(sizeof(date)); //alociraj memoriju za novi datum
	if (!newDate) {
		fclose(f);
		free(newDate); return ALLOC_ISSUE;
	}
	if (fscanf(f, "%d-%d-%d", &newDate->year, &newDate->month, &newDate->day) != 3) { //izèitaj datum iz datoteke
		free(newDate); newDate = NULL;
	}
	billPosition newBill = addSortedBill(headBill, newDate, p); //dodaj novi raèun u sortiranu listu raèuna

	char articleBuffer[150] = "";
	while (fgets(articleBuffer, sizeof(articleBuffer), f)) { //uèitaj svaki artikal iz datoteke
		trimBuffer(articleBuffer);
		if (articleBuffer[0] == '\0') continue;

		char nameBuf[MAX_CHAR];
		int amt = 0;
		double pr = 0.0;
		if (sscanf(articleBuffer, "%149s %d %lf", nameBuf, &amt, &pr) != 3)
			continue;

		articlePosition newArticle = malloc(sizeof(*newArticle)); //alociraj memoriju za novi artikal
		if (!newArticle) return ALLOC_ISSUE;
		strncpy(newArticle->name, nameBuf, MAX_CHAR - 1);
		newArticle->amount = amt;
		newArticle->price = pr;
		newArticle->nextArticle = NULL;
		newArticle->billDate = newDate;

		articlePosition tail = newBill->headArticle;
		while (tail->nextArticle) tail = tail->nextArticle;
		tail->nextArticle = newArticle;
	}

	fclose(f);
	return 0;
}

billPosition addSortedBill(billPosition headBill, datePosition newDate, char* p) {
	billPosition newBill = (billPosition)malloc(sizeof(bill)); //alociraj memoriju za novi raèun
	articlePosition dummyArticle = (articlePosition)malloc(sizeof(article)); //prazni head artikal
	dummyArticle->nextArticle = NULL; dummyArticle->name[0] = '\0'; dummyArticle->amount = 0; dummyArticle->price = 0.0;
	if (!newBill || !dummyArticle) {
		free(newBill); free(dummyArticle);
		return NULL; //ako alokacija nije uspjela, oslobodi memoriju i vrati NULL
	}

	newBill->billDate = newDate; newBill->nextBill = NULL; newBill->headArticle = dummyArticle;
	strncpy(newBill->location, p, MAX_CHAR - 1); 
	newBill->location[MAX_CHAR - 1] = '\0';

	billPosition prevBill = headBill;
	billPosition curBill = headBill->nextBill;
	while ((curBill != NULL) && compareDates(curBill->billDate, newBill->billDate)) { //trazi mjesto za novi raèun
		prevBill = curBill;
		curBill = curBill->nextBill;
	}
	prevBill->nextBill = newBill;
	newBill->nextBill = curBill;

	return newBill;
}


int freeLists(billPosition headBill) {
	if (!headBill) return 0;
	billPosition tempBill = NULL;
	billPosition currBill = headBill->nextBill;

	while (currBill != NULL) {
		articlePosition currArticle = currBill->headArticle->nextArticle;
		articlePosition tempArticle = NULL;
		while (currArticle != NULL) {
			tempArticle = currArticle;
			currArticle = currArticle->nextArticle;
			tempArticle->billDate = NULL; tempArticle->nextArticle = NULL;
			free(tempArticle);
		}
		currBill->headArticle->nextArticle = NULL;
		free(currBill->headArticle);
		tempBill = currBill;
		currBill = currBill->nextBill;
		free(tempBill->billDate);
		tempBill->billDate = NULL; tempBill->headArticle = NULL; tempBill->nextBill = NULL;
		free(tempBill);
	}
	headBill->nextBill = NULL;
	printf("Svi racuni su uspješno obrisani iz memorije.\n");

	return 0;
}