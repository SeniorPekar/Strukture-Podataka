#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "include/helpFun.h" // header pomocnih funkcija

#define FILE_ISSUE -1
#define ALLOC_ISSUE -2
#define MAX_CHAR 150

int trimBuffer(char* buffer) {
	int len = strlen(buffer);
	while (len > 0 && (buffer[len - 1] == '\n' || buffer[len - 1] == '\r' || buffer[len - 1] == ' ')) {
		buffer[--len] = '\0'; //provjeri zadnji znak i ukloni ga ako  je \n, \r ili razmak
	}
	return 0;
}

bool compareDates(datePosition date1, datePosition date2) {
	if (date1->year < date2->year) //usporedi godine
		return true;
	else if (date1->year == date2->year) { //ako su godine jednake, usporedi mjesece
		if (date1->month < date2->month)
			return true;
		else if (date1->month == date2->month) { //ako su mjeseci jednaki, usporedi dane
			if (date1->day < date2->day)
				return true;
		}
	}
	return false; //ako je prvi veci vrati false
}
int readDateFromUser(datePosition outDate) {
	char buf[128];
	int y, m, d;
	if (!outDate) return -1; //provjeri je li pokazivac valjan
	printf("\nUnos æe se ponavljati dok se ne unese valjani datum\n");

	while (true) {
		if (!fgets(buf, sizeof buf, stdin)) return -1; //procitaj unos korisnika
		trimBuffer(buf); //ukloni visak iz stringa

		char extra;
		if (sscanf(buf, "%d %d %d %c", &y, &m, &d, &extra) != 3) { //pokusaj parsirati unos
			continue;
		}

		if (y < 1900 || y > 2100) { //provjeri unesenu godinu, mjesec i dan
			printf("Nevaljana vrijednost godine, pokusajte ponovo (1900-2100)\n");
			continue;
		}
		if (m < 1 || m > 12) {
			printf("Nevaljana vrijednost mjeseca, pokusajte ponovo (1-12)\n");
			continue;
		}
		if (d < 1 || d > 31) {
			printf("Nevaljana vrijednost dana, pokusajte ponovo (1-31)\n");
			continue;
		}

		outDate->year = y; //unesi provjerene vrijednosti u strukturu
		outDate->month = m;
		outDate->day = d;
		return 0;
	}
}