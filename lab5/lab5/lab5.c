#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ALLOC_ISSUE -1
#define FILE_ISSUE -2
/*5. Napisati program koji iz datoteke čita postfiks izraz i zatim korištenjem stoga računa
rezultat. Stog je potrebno realizirati preko vezane liste*/

typedef struct Num* Position;

typedef struct Num {
	double elNum;
	Position next;
} Num;

int push(Position head, double value);// dodavanje elementa na poèetak liste
double pop(Position head); // uklanjanje elementa s pocetka liste
int getListFromFile(Position head); //ucitava postfiks izraz iz datoteke i racuna rezultat
int freeList(Position head); //oslobadja memoriju zauzetu za stog

int main() {
	Num head = { .elNum = 0, .next = NULL };
	int issueCheck = getListFromFile(&head);
	if (issueCheck == FILE_ISSUE)
		return FILE_ISSUE;
	if (issueCheck == ALLOC_ISSUE)
		return ALLOC_ISSUE;

	printf("Rezultat postfiks izraza je: %.2f\n", pop(&head));
	freeList(&head);//oslobodi memoriju stoga na kraju programa
	return 0;
}

int getListFromFile(Position head) {
	FILE* file = fopen("postfiks.txt", "r");
	if (!file)
		return FILE_ISSUE;
	char buffer[40];
	fgets(buffer, sizeof(buffer), file); //učitava postfiks izraz iz datoteke u buffer
	fclose(file);

	char* p = buffer;
	char current[8] = ""; //buffer za trenutni broj ili operator
	while(*p){
		int i = 0;
		while (*p != ' ' && *p != '\0') { //učitaj cijeli broj/operator izmedju razmaka
			current[i++] = *p;
			p++;
		}
		for(i; i < (int)sizeof(current) - 1; i++) //isprazni ostatak buffera u slućaju ostataka
			current[i] = '\0';

		if (*p == ' ') p++;        //možda nepotrebno

		if (strcmp(current, "+") == 0) { //provjeri koji je operator ili broj
			double b = pop(head); double a = pop(head); //"pop-aj" zadanja dva broja sa stoga
			if (push(head, a + b) == ALLOC_ISSUE) { //izračunaj i "push-aj" rezultat natrag na stog
				freeList(head); return ALLOC_ISSUE; //oslobodi memoriju i javi problem s alokacijom
			}
		}
		else if (strcmp(current, "-") == 0) {
			double b = pop(head); double a = pop(head);
			if(push(head, a - b) == ALLOC_ISSUE) {
				freeList(head); return ALLOC_ISSUE;
			}
		}
		else if (strcmp(current, "*") == 0) {
			double b = pop(head); double a = pop(head);
			if(push(head, a * b) == ALLOC_ISSUE) {
				freeList(head); return ALLOC_ISSUE;
			}
		}
		else if (strcmp(current, "/") == 0) {
			double b = pop(head); double a = pop(head);
			if(push(head, a / b) == ALLOC_ISSUE) {
				freeList(head); return ALLOC_ISSUE;
			}
		}
		else if (current[0] != '\0') {  //ako je broj dodaj na stog
			if (push(head, atof(current)) == ALLOC_ISSUE) {
				freeList(head); return ALLOC_ISSUE;
			}
		}
	}
	return 0;
}

int push(Position head, double value) {
	Position newNum = (Position)malloc(sizeof(Num));//dinamièki alocira memoriju za novi element
	if (!newNum) 
		return ALLOC_ISSUE;//provjera alokacije

	Position q = head;
	newNum->elNum = value;
	newNum->next = q->next; //novi el. pokazuje na prijašnje prvi
	q->next = newNum;//head pokazuje na novi el.

	return 0;
}

double pop(Position head) {
	if(head->next == NULL){
		printf("Prazan stog!\n");
		return 0;
	}
	double x = 0.0;
	Position q = head->next;
	x = q->elNum;
	head->next = q->next;
	q->next = NULL;
	free(q); //oslobodi memoriju uklonjenog elementa
	return x; //vrati vrijednost uklonjenog elementa
}

int freeList(Position head) {
	Position q = head->next;
	while (q != NULL) {
		head = q->next;
		q->next = NULL;
		free(q);
		q = head;
	}

	return 0;
}