#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#define LIST_ISSUE -3
#define FILE_ISSUE -2
#define ALLOC_ISSUE -1
/*4. Napisati program za zbrajanje i množenje polinoma. Koeficijenti i eksponenti se

čitaju iz datoteke.

Napomena: Eksponenti u datoteci nisu nužno sortirani.
*/

typedef struct Polinom * Position; //pokazivač na strukturu polinoma
typedef struct Polinom {
	int koef;
	int pow;
	Position next;
} Polinom;

int printPoliInFile(Position head, int resultCounter); //funk. koja upisuje sumu i produkt u datoteku
int getListFromFile(Position head, int lineNum); // funk. koja dohvaća vrijednosti iz datoteke
int addSorted(Position head, int koef, int pow); // funk. koja sortira  i upisuje dohvaćene vrijednosti u listu
int deleteList(Position head); // oslobađa memoriju liste
int sumPoly(Position firstPoly, Position secondPoly, Position result); // zbroji dva polinoma i spremi rezultat u odvojenu listu
int multiplyPoly(Position firstPoly, Position secondPoly, Position result); // pomnoži dva polinoma i spremi u odvojenu listu
int resetFile(Position firstPoly, Position secondPoly);

int main() {
	Polinom result = { .koef = 0, .pow = 0, .next = NULL }; //rezultat
	Polinom firstPoly = { .koef = 0, .pow = 0, .next = NULL }; //prvi polinom
	Polinom secondPoly = { .koef = 0, .pow = 0, .next = NULL }; //drugi polinom
	int resultCounter = 0;

	int issueCheck = getListFromFile(&firstPoly, 1); //dohvati prvi polinom iz prvog retka i provjeri alokaciju i datoteku
	if(issueCheck == FILE_ISSUE)
		return FILE_ISSUE;
	if (issueCheck == ALLOC_ISSUE)
		return ALLOC_ISSUE;
	if(issueCheck == LIST_ISSUE)
		return LIST_ISSUE;

	issueCheck = getListFromFile(&secondPoly, 2); // -||-
	if (issueCheck == FILE_ISSUE)
		return FILE_ISSUE;
	if (issueCheck == ALLOC_ISSUE) 
		return ALLOC_ISSUE;
	if (issueCheck == LIST_ISSUE)
		return LIST_ISSUE;

	issueCheck = resetFile(&firstPoly, &secondPoly); // resetiramo datoteku da obrišemo stare rezultate
		if(issueCheck == LIST_ISSUE)
			return LIST_ISSUE;

	printf(" Poly a: "); //ispiši prvi polinom u konzoli
	Position q = firstPoly.next;
	while (q != NULL) {
		printf("%dx^%d ", q->koef, q->pow);
		q = q->next;
	}

	printf("\n Poly b: "); //ispiši drugi polinom u konzoli
	q = secondPoly.next;
	while (q != NULL) {
		printf("%dx^%d ", q->koef, q->pow);
		q = q->next;
	}

	issueCheck = sumPoly(&firstPoly, &secondPoly, &result); //zbroji polinome u treću liustu i provjeri alokaciju
	if (issueCheck == ALLOC_ISSUE)
		return ALLOC_ISSUE;
	if (issueCheck == LIST_ISSUE)
		return LIST_ISSUE;

	issueCheck = printPoliInFile(&result, resultCounter); resultCounter++; //upiši rezultat u datoteku i provjeri otvaranje datoteke
	if (issueCheck == FILE_ISSUE)
		return FILE_ISSUE;
	if (issueCheck == LIST_ISSUE)
		return LIST_ISSUE;

	printf("\n Sum(a + b): "); // ispiši zbroj u konzoli
	q = result.next;
	while(q!= NULL) {
		printf("%dx^%d ", q->koef, q->pow);
		q = q->next;
	}
	issueCheck = deleteList(&result); // oslobodi treću listu da bi se mogao upisati produkt
	if (issueCheck == LIST_ISSUE)
		return LIST_ISSUE;

	issueCheck = multiplyPoly(&firstPoly, &secondPoly, &result); //pomnoži polinome i spremi rezultat u treću listu, provjeri alokaciju
	if(issueCheck == ALLOC_ISSUE)
		return ALLOC_ISSUE;
	if (issueCheck == LIST_ISSUE)
		return LIST_ISSUE;

	issueCheck = printPoliInFile(&result, resultCounter);//upiši rezultat u datoteku, provjeri otvaranje datoteke
	if (issueCheck == FILE_ISSUE)
		return FILE_ISSUE;
	if (issueCheck == LIST_ISSUE)
		return LIST_ISSUE;

	printf("\n Product(a * b): ");//ispiši produkt u konzoli
	q = result.next;
	while (q != NULL) {
		printf("%dx^%d ", q->koef, q->pow);
		q = q->next;
	}

	issueCheck = deleteList(&firstPoly); //oslobodi svu memoriju
	if (issueCheck == LIST_ISSUE)
		return LIST_ISSUE;

	issueCheck = deleteList(&secondPoly);
	if (issueCheck == LIST_ISSUE)
		return LIST_ISSUE;

	issueCheck = deleteList(&result);
	if (issueCheck == LIST_ISSUE)
		return LIST_ISSUE;

	return 0;
}
int printPoliInFile(Position result, int resultCounter) {
	FILE* file = fopen("polinomi.txt", "a");
	if (!file)
		return FILE_ISSUE;		

	if(resultCounter > -1) // prebaci redak u svakom slučaju osim za prvi polinom u resetFile funk.
		fprintf(file, "\n");

	if(resultCounter == 0) //prosljeđeno iz maina da ljepše izgleda dat
		fprintf(file, "Suma: ");

	if (resultCounter == 1)
		fprintf(file, "Produkt: ");

	Position q = result->next;
	while (q != NULL) {
		fprintf(file, "%d %d ", q->koef, q->pow); //upisivanje podataka u datoteku
		q = q->next;
	}
	fclose(file);
	return 0;
}
int getListFromFile(Position head, int lineNumber) {
	if (!head) return LIST_ISSUE; //provjeri je li lista uredno proslijeđena 
	FILE* file = fopen("polinomi.txt", "r");
	if (!file)
		return FILE_ISSUE;

	char buffer[1024]; //buffer za liniju

	for (int i = 1; i <= lineNumber; ++i) {//preskaći linije do odabrane(lineNum)
		if (!fgets(buffer, sizeof(buffer), file)) { //provjeri je li trenutna linija prazna
			fclose(file);
			return FILE_ISSUE; 
		}
	}
	fclose(file); //linija učitana, datoteka je sada nepotrebna

	int koef = 0, pow = 0; //temp varijable
	char* p = buffer;
	int n = 0, issueCheck = 0;

	while (sscanf(p, "%d %d%n", &koef, &pow, &n) == 2) { //učitavaj parove dok linija ne završi
		issueCheck = addSorted(head, koef, pow);
		if (issueCheck == ALLOC_ISSUE) //provjeri alokaciju
			return ALLOC_ISSUE;

		if(issueCheck == LIST_ISSUE)
			return LIST_ISSUE; //provjeri prenošenje liste

		p += n; //inkrementiraj pokazivač u liniji za učitani par
		while (*p && isspace((unsigned char)*p)) //preskoći razmake između parova
			p++;
	}
	return 0;
}

	int addSorted(Position head, int koef, int pow) {
		if(!head)
			return LIST_ISSUE; //provjeri prenošenje liste
		if (koef != 0) { //nemoj učitati nul parove
			Position q = head;
			while (q->next != NULL && pow < q->next->pow) //sortiratj novi član u listu
				q = q->next;

			if (q->next != NULL && pow == q->next->pow) { //ako postoji član s istom potencijom odmah ga zbroji
				q->next->koef += koef;
				return 0;
			}

			Position newPoly = (Position)malloc(sizeof(Polinom)); // din. alokacija novog člana
			if (!newPoly)
				return ALLOC_ISSUE; //provjeri alokaciju

			newPoly->koef = koef; //učlani u listu
			newPoly->pow = pow;
			newPoly->next = q->next;
			q->next = newPoly;
		}
		return 0;
	}
	int deleteList(Position head) {
		if (!head)
			return LIST_ISSUE; //provjeri prenošenje liste

		Position current = head->next;
		while (current != NULL) {
			Position temp = current;
			current = current->next;
			free(temp); //oslobodi prijašnjeg
		}
		head->next = NULL;
		return 0;
	}

	int sumPoly(Position firstPoly, Position secondPoly, Position result) {
		if(!firstPoly || !secondPoly || !result)
			return LIST_ISSUE; //provjeri prenošenje lista

		int issueCheck = 0;
		Position i = firstPoly->next;
		while(i != NULL) {
			issueCheck = addSorted(result, i->koef, i->pow);
			if (issueCheck == ALLOC_ISSUE) //sortirano dodavanje iz prvog polinoma
				return ALLOC_ISSUE;
			if(issueCheck == LIST_ISSUE)
				return LIST_ISSUE;
			i = i->next;
		}
		i = secondPoly->next;
		while(i != NULL) {
			issueCheck = addSorted(result, i->koef, i->pow);
			if (issueCheck == ALLOC_ISSUE) // sortirano dodavanje iz drugog polinoma(automatski zbroji ako postoji isti eksponent)
				return ALLOC_ISSUE;
			if(issueCheck == LIST_ISSUE)
				return LIST_ISSUE;
			i = i->next;
		}
		return 0;
	}

	int multiplyPoly(Position firstPoly, Position secondPoly, Position result) {
		Position i = firstPoly->next;
		int issueCheck = 0;
		while (i != NULL) {
			Position j = secondPoly->next;
			while (j != NULL) { //pomnoži svaki član prvog polinoma sa svakim članom drugog polinoma
				int newKoef = i->koef * j->koef;
				int newPow = i->pow + j->pow;
				issueCheck = addSorted(result, newKoef, newPow);
				if (issueCheck == ALLOC_ISSUE) //sortirano dodavanje rezultata u listu (automatski zbroji ako postoji isti eksponent)
					return ALLOC_ISSUE; //provjeri alokaciju
				if (issueCheck == LIST_ISSUE)
					return LIST_ISSUE; //provjeri prenošenje liste
				j = j->next;
			}
			i = i->next;
		}
		return 0;
	}

	int resetFile(Position firstPoly, Position secondPoly) {
		FILE* file = fopen("polinomi.txt", "w"); //otvaramo s "w" naredbom tako da se cijela datoteka briše
		if (!file)
			return FILE_ISSUE; //provjera otvaranja dat.
		fclose(file);

		int issueCheck = printPoliInFile(firstPoly, -1); //ponovno upisujemo prvi polinom i provjera datoteke
		if (issueCheck == FILE_ISSUE)
			return FILE_ISSUE;

		issueCheck = printPoliInFile(secondPoly, 3); //-||- drugi polinom
		if (issueCheck == FILE_ISSUE)
			return FILE_ISSUE;

		return 0;
	}