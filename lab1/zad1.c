#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>

#define MAX_CHAR 15
#define MAX_POINTS 115.0
#define LOAD_ISSUE -1
/* Zadatak 1:
Napisati program koji prvo proèita koliko redaka ima datoteka, tj. koliko ima studenata
zapisanih u datoteci. Nakon toga potrebno je dinamièki alocirati prostor za niz struktura
studenata (ime, prezime, bodovi) i uèitati iz datoteke sve zapise. Na ekran ispisati ime,
prezime, apsolutni i relativni broj bodova.
Napomena: Svaki redak datoteke sadrži ime i prezime studenta, te broj bodova na kolokviju.
relatvan_br_bodova = br_bodova/max_br_bodova*100      
*/

typedef struct Student {
	char name[MAX_CHAR]; //privaèa imena i prezimena do 15 znakova
	char surname[MAX_CHAR];
	int absPoints;
	double percentage;
}Student;
// prototypes
int countLines();
int loadInfo(Student*, int);

int main() {
	int numOfLines = countLines(); // dobavljanje broja studenata iz datoteke funkcijom
	Student* students = malloc(numOfLines * sizeof(*students)); //dinamièka alokacija niza struktura

	if(numOfLines <= 0) {
		printf("Doslo je do pogreške pri ucitavanju podataka(broj linija)"); return -1; //vraèanje -1 za krivo uèitavanje u 1. funkciji
	}else
		printf("Zapisano je %d studenata\n", numOfLines);
	int loadCheck = loadInfo(students, numOfLines);// uèitavanje podataka o studentima funkcijom
	if (loadCheck == 0)
		printf("Ucitavanje podataka uspjesno");
	else{
		printf("Doslo je do pogreske pri ucitavanju podataka (podaci o studentu"); return -2;
	}// vraæanje -2 za krivo uèitavanje u 2. funkciji
	
	for (int i = 0; i < numOfLines; i++) //ispis podataka o studentima
		printf("\n%s %s %d od %0.0lf (%0.2lf) ", students[i].name, students[i].surname, students[i].absPoints, MAX_POINTS, students[i].percentage);

	free(students); //oslobadjanje memorije
	return 0;
}

int countLines() {
	int counter = 0;
	FILE* file = fopen("studenti.txt", "r");

	if (!file) 
		return LOAD_ISSUE; //provjera otvaranja datoteke

	while (!feof(file)) //prebaci u buffer
		if (fgetc(file) == '\n')
			counter++;		//brojanje redaka
		
	fclose(file); //provjera potrebna?
		return counter;
}

int loadInfo( Student* students, int numOfLines) {
	FILE* file = fopen("studenti.txt", "r");
	if (!file)
		return LOAD_ISSUE; //provjera otvaranja datoteke

	for (int i = 0; i < numOfLines; i++) {
		fscanf(file, "%s %s %d", students[i].name, students[i].surname, &students[i].absPoints); //spremanje podataka u strukturu
		students[i].percentage = ((double)students[i].absPoints / MAX_POINTS) * 100.0; //ako je ispit od 115 bodova
	}
	fclose(file);
		return 0;
}