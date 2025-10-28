#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHAR 15 //maksimalni broj sova za ime i prezime
#define ALLOC_ISSUE -1 //greška pri alokaciji memorije
#define FILE_ISSUE -2 // greška pri otvaranju datoteke
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
int addToEnd(Position head, char* name, char* surname, int birthYear);//dodavanje elementa na kraj liste

int printList(Position head); // ispis cijele liste

int addAfterSelected(Position head, char* name, char* surname, int birthYear, char* selectedSurname); //dodavanja el nakon odabranog
int addBeforeSelected(Position head, char* name, char* surname, int birthYear, char* selectedSurname); //dodavanje el prije odabranog

int sortListBySurname(Position head);//sortiraj po prezimenu

int printListInFile(Position head);// upisuje listu u datoteku
int getListFromFile(Position head);//isèitava listu iz datoteke

int findBySurname(Position head, char* surname);// traženje i ispis elementa po prezimenu
int deleteBySurname(Position head, char* surname);// brisanje elementa iz liste po prezimenu

int listMenu(Position head, int option);//korisnik bira što napraviti s listom
int deleteList(Position head); //oslobaða memoriju liste

int main() {
	int menu = 1, menuOption = 0; //deklaracija varijabli za navigiranje menu-a
	int fileCheck = 0;

	Person head = { .name = "", .surname = "", .birthYear = 0, .next = NULL }; //deklaracija prvog praznog elementa liste
	fileCheck = getListFromFile(&head); //uèitavanje postojeæe liste iz datoteke
	if (fileCheck != 0)
		return fileCheck;

	printf("Trenutna lista: \n");
	printList(&head);
	 //opcije za rad s listom
	printf("Sto zelite napraviti s listom: \n");
	printf("Ako zelite dodati element na pocetak upisite 1\n");
	printf("Ako zelite dodati element na kraj upisite 2\n");
	printf("Ako zelite ispisati listu upisite 3\n");
	printf("Ako zelite pronaci element po prezimenu upisite 4\n");
	printf("Ako zelite izbrisati element po prezimenu upisite 5\n");
	printf("Ako zelite dodati element nakon nekog odabranog upisite 6\n");
	printf("Ako zelite dodati element prije nekog odabrananog ipisite 7\n");
	printf("Ako zelite sortirati listu upisite 8\n");
	printf("Ako zelite završiti program upisite 9\n");

	while (menu == 1) { //korisnik bira što želi raditi (vrti se dok ga korisnik ne zaustavi stavljajuæi menu na vrijednost 0)
		scanf("%d", &menuOption);
		if (menuOption > 0 && menuOption < 10)
			menu = listMenu(&head, menuOption); //prosljeðivanje korisnikovog odabira u funkciju
		else
			printf("Nevaljan unos\n");
	}
	fileCheck = printListInFile(&head); //spremanje liste u datoteku prije izlaska iz programa
	if (fileCheck != 0)
		return fileCheck; //vraæa -2 ako je došlo do greške pri radu s datotekom
	return menu; // vraæa nula za uspiješan program, i -1 za grešku pri alokaciji
}

int listMenu(Position head, int option) {
	int tempYear = 0, issueCheck = 1;
	char tempName[MAX_CHAR] = "", tempSurname[MAX_CHAR] = "", selectedSurname[MAX_CHAR] = ""; //lokalne varijable za prosljediti korisnikov unos
	switch (option) {//case-ovi za korisnikov unos
	case 1: //korisnik unosi element i prosljeðuje je u addToBeginning
		printf("Upisite ime, prezime te godinu rodenja novog elementa (format: Ime Prezime godina(XX)):\n");
		scanf("%s %s %d", tempName, tempSurname, &tempYear);
		issueCheck = addToBeginning(head, tempName, tempSurname, tempYear);
		break;
	case 2: //korisnik unosi element i prosljeðuje u addToEnd
		printf("Upisite ime, prezime te godinu rodenja novog elementa (format: Ime Prezime godina(XX)):\n");
		scanf("%s %s %d", tempName, tempSurname, &tempYear);
		issueCheck = addToEnd(head, tempName, tempSurname, tempYear);
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
	case 6: //korisnik dodaje el. nakon odabranog
		printf("Unesite prezime elementa nakon kojeg želite dodati novi:\n");
		scanf("%s", selectedSurname);
		printf("Upisite ime, prezime te godinu rodenja novog elementa (format: Ime Prezime godina(XX)):\n");
		scanf("%s %s %d", tempName, tempSurname, &tempYear);
		issueCheck = addAfterSelected(head, tempName, tempSurname, tempYear, selectedSurname);
		break;
	case 7: //korisnik dodaje el. prije odabranoga
		printf("Unesite prezime elementa prije kojeg želite dodati novi:\n");
		scanf("%s", selectedSurname);
		printf("Upisite ime, prezime te godinu rodenja novog elementa (format: Ime Prezime godina(XX)):\n");
		scanf("%s %s %d", tempName, tempSurname, &tempYear);
		issueCheck = addBeforeSelected(head, tempName, tempSurname, tempYear, selectedSurname);
		break;
	case 8: //sortiranje
		issueCheck = sortListBySurname(head);
		break;
	case 9: // vraæa u main vrijednost koja završava program
		return 0;
		break;
	}
	return issueCheck;
}
int cinStruct(Position q, char* name, char* surname, int birthYear) { //funkcija unosi prosljeðene vrijenosti u element
	strcpy(q->name, name);
	strcpy(q->surname, surname);
	q->birthYear = birthYear;
	return 0;
}

int printList(Position head) {
	printf("Ispis osoba: \n");
	for (Position q = head->next; q != NULL; q = q->next) {//ispis liste u for loop-u
		printf("%s %s %d \n", q->name, q->surname, q->birthYear);
	}
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

int addAfterSelected(Position head, char* name, char* surname, int birthYear, char* slectedSurname) {
	Position newPerson = (Position)malloc(sizeof(Person)); //din. alokacije memorije za novi el.
	if (!newPerson)
		return ALLOC_ISSUE; //provjera alokacije
	for (Position q = head->next; q != NULL; q = q->next) {
		if (strcmp(q->surname, slectedSurname) == 0) { // traženje odabranog elementa
			newPerson->next = q->next; //novi el. pokazuje na el. nakon odabranog
			q->next = newPerson; // odabrani el. pokazuje na novi
			cinStruct(newPerson, name, surname, birthYear); //fun. za unos podataka u novi el.
			printf("Dodavanje uspjesno\n"); // obavijesti studenta ako je dodavanje el. uspiješno
			break;
		}
		if (q->next == NULL)
			printf("Odabrana osoba ne postoji\n"); //obavijesti korisnika ako traženi el. nije naðen
	}
	return 1;
}

int addToEnd(Position head, char* name, char* surname, int birthYear) {
	Position newPerson = (Position)malloc(sizeof(Person));//dinamièka alokacija momorije za novi el.
	if (!newPerson)
		return ALLOC_ISSUE;//provjera alokacije
	Position q = head;
	while (q->next != NULL) //traženje zadnjeg el.
		q = q->next;
	q->next = newPerson; //prijašnje zadnji el. pokazuje na novi
	newPerson->next = NULL;// novi ne prikazuje na ništa
	cinStruct(newPerson, name, surname, birthYear); //pozivanje za unos vrijednosti novog el.
	return 1;
}

int addBeforeSelected(Position head, char* name, char* surname, int birthYear, char* slectedSurname) {
	Position newPerson = (Position)malloc(sizeof(Person));//din. alokacija memorije za novi el.
	if (!newPerson)
		return ALLOC_ISSUE; //provjera alokacije
	for (Position q = head; q->next != NULL; q = q->next) { 
		if (strcmp(q->next->surname, slectedSurname) == 0) { //traženje odabrang el
			newPerson->next = q->next; // novi el. pokazuje na odabrani
			q->next = newPerson; //el. prije odabranog pokazuje na novi
			cinStruct(newPerson, name, surname, birthYear); // pozivanje fun. za unos podataka u el.
			printf("Dodavanje uspjesno\n"); // obaviejsti korisnika ako je sve prošlo ok
			break;
		}
		if (q->next->next == NULL) //obavijeti korisnika ako odabrani el. nije naðen
			printf("Odabrana osoba ne postoji\n");
	}
	return 1;
}

int findBySurname(Position head, char* surname) {
	Position temp = NULL, q = head->next;
	for (q; q != NULL; q = q->next) { //traženje elementa po prezimenu
		if (strcmp(q->surname, surname) == 0)
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
	Position temp = NULL, q = head;
	for (q; q != NULL; q = q->next) {
		if (q->next == NULL) {
			printf("Ta osoba ne postoji\n"); //prijavi korisniku ako traženi element nije naðen
		}
		if (strcmp(q->next->surname, surname) == 0) { //traženje elementa
			temp = q->next;
			q->next = temp->next;//element prije traženog sada pokazuje na el. sada pokazuje na el. iza traženog
			free(temp); //oslobaðanje memorije traženog el.
			printf("Brisanje uspjesno\n");
		}
	}
	return 0;
}

int sortListBySurname(Position head) {
	int tempYear = 0;
	char  tempName[MAX_CHAR] = "", tempSurname[MAX_CHAR] = ""; //privremene varijablu za razmjenu
	for (Position i = head->next; i != NULL; i = i->next) {
		for (Position j = i->next; j != NULL; j = j->next) { //usporedba svakog elementa s ostalima
			if (strcmp(i->surname, j->surname) > 0) { //usporedba prezimena
				//zamjena prezimena
				strcpy(tempSurname, i->surname);
				strcpy(i->surname, j->surname);
				strcpy(j->surname, tempSurname);
				//zamjena imena
				strcpy(tempName, i->name);
				strcpy(i->name, j->name);
				strcpy(j->name, tempName);
				//zamjena godina roðenja
				tempYear = i->birthYear;
				i->birthYear = j->birthYear;
				j->birthYear = tempYear;
			}
		}
	}

	printf("Sortiranje uspjesno\n");
	return 1;
}

int printListInFile(Position head) {
	FILE* file = fopen("osobe.txt", "w"); //otvaranje datoteke
	if (!file)
		return FILE_ISSUE; //provjera otvaranja

	Position q = head->next;
	while (q != NULL) {
		fprintf(file, "%s %s %d\n", q->name, q->surname, q->birthYear); //upisivanje podataka u datoteku
		q = q->next;
	}
	deleteList(head); // poziva funkciju koja oslobaða memoriju liste
	fclose(file); //zatvaranje datotek
	return 0;
}

int deleteList(Position head) {
	Position temp = NULL;
	head = head->next; //poèni od prvog el.
	while (head != NULL) {
		temp = head;
		head = head->next;
		free(temp); //oslobodi memoriju prijašnjeg el.
	}
	return 0;
}

int getListFromFile(Position head) {
	FILE* file = fopen("osobe.txt", "r"); // otvaranje datoteke
	if (file == NULL)
		return FILE_ISSUE; //provjera otvaranja

	char tempName[MAX_CHAR], tempSurname[MAX_CHAR];
	int tempYear;

	while (fscanf(file, "%14s %14s %d", tempName, tempSurname, &tempYear) == 3) //vrti se dok ima podataka za èitanje
		if (addToEnd(head, tempName, tempSurname, tempYear) == ALLOC_ISSUE) //dodavanje elemenata u listu funkcijom addToEnd
			return ALLOC_ISSUE;

	fclose(file); //zatvarnaje datoteke
	return 0;
}