#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHAR 15 //maksimalni broj sova za ime i prezime
#define ALLOC_ISSUE -1 //greška pri alokaciji memorije
/*
Zadatak 2:
2. Definirati strukturu osoba (ime, prezime, godina roðenja) i napisati program koji:
A. dinamièki dodaje novi element na poèetak liste,
B. ispisuje listu,
C. dinamièki dodaje novi element na kraj liste,
D. pronalazi element u listi (po prezimenu),
E. briše odreðeni element iz liste,
U zadatku se ne smiju koristiti globalne varijable.
*/

typedef struct Person* Position;

typedef struct Person {
	char name[MAX_CHAR];
	char surname[MAX_CHAR];
	int birthYear;
	Position next;
} Person;

int cinStruct(Position q, char* name, char* surname, int birthYear); //funkcija za unos varijabli elementa
int addToBeginning(Position head, char* name, char* surname, int birthYear);// dodavanje elementa na poèetak liste
int printList(Position head); // ispis cijele liste
int addToEnd(Position head, char* name, char* surname, int birthYear);//dodavanje elementa na kraj liste
int findBySurname(Position head, char* surname);// traženje i ispis elementa po prezimenu
int deleteBySurname(Position head, char* surname);// brisanje elementa iz liste po prezimenu
int listMenu(Position head, int option);//korisnik bira što napraviti s listom

int main() {
	int menu = 1, menuOption = 0; //deklaracija varijabli za navigiranje menu-a

	Person head = { .name = "", .surname = "", .birthYear = 0, .next = NULL }; //deklaracija prvog praznog elementa liste

	addToBeginning(&head, "Nikola", "Bareta", 04); //unos i ispis poèetnih elemenata liste
	addToBeginning(&head, "Ante", "Pantelija", 07);
	addToEnd(&head, "Tomislav", "Renoult", 05);
	addToEnd(&head, "Rade", "Mander", 76);
	printf("Trenutna lista: \n");
	printList(&head);

	while (menu == 1) { //korisnik bira što želi raditi (vrti se dok ga korisnik ne zaustavi stavljajuæi menu na vrijednost 0)
		printf("Sto zelite napraviti s listom: \n");
		printf("Ako zelite dodati element na pocetak upisite 1\n");
		printf("Ako zelite dodati element na kraj upisite 2\n");
		printf("Ako zelite ispisati listu upisite 3\n");
		printf("Ako zelite pronaci element po prezimenu upisite 4\n");
		printf("Ako zelite izbrisati element po prezimenu upisite 5\n");
		printf("Ako zelite završiti program upisite 6\n");
		scanf("%d", &menuOption);
		menu = listMenu(&head, menuOption); //prosljeðivanje korisnikovog odabira u funkciju
	}
	return menu; // vraæa nula za uspiješan program, i -1 za grešku pri alokaciji
}

int listMenu(Position head, int option) { 
	int tempYear = 0, allocCheck = 1;
	char tempName[MAX_CHAR] = "", tempSurname[MAX_CHAR] = ""; //lokalne varijable za prosljediti korisnikov unos
	switch (option) {//case-ovi za korisnikov unos
	case 1: //korisnik unosi element i prosljeðuje je u addToBeginning
		printf("Upisite ime, prezime te godinu rodenja novog elementa (format: Ime Prezime godina(XX)):\n");
		scanf("%s %s %d", tempName, tempSurname,&tempYear);
		allocCheck = addToBeginning(head, tempName, tempSurname, tempYear);
		break;
	case 2: //korisnik unosi element i prosljeðuje u addToEnd
		printf("Upisite ime, prezime te godinu rodenja novog elementa (format: Ime Prezime godina(XX)):\n");
		scanf("%s %s %d", tempName, tempSurname, &tempYear);
		allocCheck = addToEnd(head, tempName, tempSurname, tempYear); 
		break;
	case 3: //pozivanje funkcije za ispis
		printList(head);
		break;
	case 4: //korisnik unosi prezime koje se prosljeðuje funkciji koja ga traži u listi
		printf("Unesite prezime elementa kojeg zelite naci: \n");
		scanf("%s", tempSurname);
		findBySurname(head, tempSurname);
		break;
	case 5://korisnik unosi prezime koje se prenosi u funkciju koja ga briše
		printf("Unesite prezime elementa kojeg zelite izbrisati: \n");
		scanf("%s", tempSurname);
		deleteBySurname(head, tempSurname);
		break;
	case 6: // vraæa u main vrijednost koja završava program
		return 0;
		break;
	}
	return allocCheck; 
}
int cinStruct(Position q, char* name, char* surname, int birthYear) { //funkcija unosi prosljeðene vrijenosti u element
	strcpy(q->name, name);
	strcpy(q->surname, surname);
	q->birthYear = birthYear;
	return 0;
}

int addToBeginning(Position head, char* name, char* surname, int birthYear) {
	Position newPerson = (Position)malloc(sizeof(Person));//dinamièki alocira memoriju za novi element
	if (!newPerson)
		return ALLOC_ISSUE;//provjera alokacije
	cinStruct(newPerson, name, surname, birthYear);//poziva funkciju za unos podataka
	newPerson->next = head->next; //novi el. pokazuje na prijašnje prvi
	head->next = newPerson;//head pokazuje na novi el.
	return 1;
}

int printList(Position head) {
	printf("Ispis osoba: \n");
	for (Position q = head->next; q != NULL; q = q->next) {//ispis liste u for loop-u
		printf("%s %s %d \n", q->name, q->surname, q->birthYear);
	}
	return 0;
}

int addToEnd(Position head, char* name, char* surname, int birthYear) {
	Position newPerson = (Position)malloc(sizeof(Person));//dinamièka alokacija momorije za novi el.
	if (!newPerson)
		return ALLOC_ISSUE;//provjera alokacije
	Position q = head->next;
	while (q->next != NULL) //traženje zadnjeg el.
		q = q->next;
	q->next = newPerson; //prijašnje zadnji el. pokazuje na novi
	newPerson->next = NULL;// novi ne prikazuje na ništa
	cinStruct(newPerson, name, surname, birthYear); //pozivanje za unos vrijednosti novog el.
	return 1;
}

int findBySurname(Position head, char* surname) {
	Position temp = NULL, q = head->next;
	for (q; q != NULL; q = q->next) { //traženje elementa po prezimenu
		if (strcmp(q->surname, surname)==0)
			temp = q;
	}
	if (temp == NULL) //ako se nikoji element ne podudara prijavi korisniku
		printf("Ta osoba ne postoji");
	else { //ispis naðenog elementa
		printf("Tražena osoba:\n");
		printf("%s %s %d\n", temp->name, temp->surname, temp->birthYear);
	}
	return 0;
}

int deleteBySurname(Position head, char* surname) {
	Position temp = NULL, q= head;
	for (q; q->next != NULL; q = q->next) {
		if (strcmp(q->next->surname, surname)==0) { //traženje elementa
			temp = q->next;
			q->next = temp->next;//element prije traženog sada pokazuje na el. sada pokazuje na el. iza traženog
			free(temp); //oslobaðanje memorije traženog el.
			printf("Brisanje uspjesno\n");
		}
		if (q->next->next == NULL) {
			printf("Ta osoba ne postoji\n"); //prijavi korisniku ako traženi element nije naðen
		}
	}
	return 0;
}