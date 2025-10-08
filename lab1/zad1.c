#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>

const int MAX_CHAR = 15;
/* Zadatak 1:
Napisati program koji prvo proèita koliko redaka ima datoteka, tj. koliko ima studenata
zapisanih u datoteci. Nakon toga potrebno je dinamièki alocirati prostor za niz struktura
studenata (ime, prezime, bodovi) i uèitati iz datoteke sve zapise. Na ekran ispisati ime,
prezime, apsolutni i relativni broj bodova.
Napomena: Svaki redak datoteke sadrži ime i prezime studenta, te broj bodova na kolokviju.
relatvan_br_bodova = br_bodova/max_br_bodova*100      
*/

int countLines();

struct Student {
	char name[15];
	char surname[15];
	int absPoints;
	double percentage;
}; //typedef potreban?

int loadInfo(struct Student*, int);

int main() {
	int numOfLines = countLines();
	struct Student* students = malloc(numOfLines * sizeof(*students));

	printf("Zapisano je %d studenata", numOfLines);

	int loadCheck = loadInfo(students, numOfLines);
	if (loadCheck == 0)
		printf("Uèitavanje podataka uspješno");

	return 0;
}

int countLines() {
	int counter = 0;
	FILE* file = fopen("studenti.txt", "r");

	if (file == NULL) 
		return -1;

	while (!feof(file))
		if (fgetc(file) == '\n')
			 counter++;
		
	fclose(file);
	if (file == NULL)
		return counter;
	else
		return -2;
}

int loadInfo(struct Student* students, int numOfLines) {
	FILE* file = fopen("studenti.txt", "r");
	if (file == NULL)
		return -1;

	for (int i = 0; i < numOfLines; i++) {

	}
	fclose(file);
	return 0;
}