#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "include/options.h" //header ovih funkcija
#include "include/helpFun.h" //header pomocnih funkcija

int options(billPosition billHead) {
	int a = 0; //ispiši opcije
	printf("Odaberite opciju:\n1. Ispis povijesti racuna\n2. Najveci racun\n3. Najmanji racun\n");
	printf("4. Prosjecni iznos racuna\n5. Najprodavaniji artikal\n6. Najprofitabilniji artikal\n");
	printf("7. Ukupni broj prodaja pojedinog artikla\n8. Broj prodaja pojedinogartikla u odredenom periodu\n");
	printf("9. Ispis racuna iz odredenog perioda\n10. Zavrsi program\n");
	while (true) { //vrti dok korisnik ne izabere izlaz
		scanf("%d", &a);
		switch (a) {
		case 1:
			printLists(billHead); //ispiši sve raèune
			break;
		case 2:
			highPriceBill(billHead); //ispiši najskuplji raèun
			break;
		case 3:
			lowPriceBill(billHead); // najjeftiniji raèun
			break;
		case 4:
			avgPriceBill(billHead); //prosjeèna cijena raèuna
			break;
		case 5:
			mostSoldArticle(billHead);// najprodavaniji artikal
			break;
		case 6:
			mostProffitableArticle(billHead); //najprofitabilniji artikal(donio najveæe prihode)
			break;
		case 7:
			// ukupni broj prodaja pojedinog artikla
			char artikal[MAX_CHAR] = "";
			printf("Koji artikal zelite provjeriti?\n ");
			scanf("%s", artikal); //Korisnik bira artikal
			trimBuffer(artikal); // ukloni višak iz stringa
			getArticleDetails(billHead, artikal);
			break;
		case 8:
			// broj prodaja artikla u odredenom periodu
			char artikal2[MAX_CHAR] = "";
			printf("Koji artikal zelite provjeriti?\n ");
			scanf("%s", artikal2);
			trimBuffer(artikal2);
			printf("Unesite pocetni datum (YYYY MM DD): ");

			datePosition startDate = (datePosition)malloc(sizeof(date)); //alociraj memoriju za pocetni datum
			if (!startDate) { printf("Neuspjela alokacija memorije za datum\n"); return ALLOC_ISSUE; }
			if (readDateFromUser(startDate) != 0) { free(startDate); break; } //korisnik unosi pocetni datum pozivajuci funkciju

			printf("Unesite zavrsni datum (YYYY MM DD): ");

			datePosition endDate = (datePosition)malloc(sizeof(date));
			if (!endDate) { printf("Neuspjela alokacija memorije za datum\n"); return ALLOC_ISSUE; }
			if (readDateFromUser(endDate) != 0) { free(startDate); free(endDate); break; }

			getArticleDetailsInPeriod(billHead, artikal2, startDate, endDate);
			free(startDate); //oslobodi memoriju
			free(endDate);
			break;
		case 9:
			// racuni iz odredenog perioda
			printf("Unesite pocetni datum (YYYY MM DD): ");

			datePosition startDate2 = (datePosition)malloc(sizeof(date));
			if (!startDate2) { printf("Neuspjela alokacija memorije za datum\n"); return ALLOC_ISSUE; }
			if (readDateFromUser(startDate2) != 0) { free(startDate2); break; }

			printf("Unesite zavrsni datum (YYYY MM DD): ");

			datePosition endDate2 = (datePosition)malloc(sizeof(date));
			if (!endDate2) { printf("Neuspjela alokacija memorije za datum\n"); return ALLOC_ISSUE; }
			if (readDateFromUser(endDate2) != 0) { free(startDate2); free(endDate2); break; }
			printBillInPeriod(billHead, startDate2, endDate2);
			free(startDate2);
			free(endDate2);
			break;
		case 10:
			// izlaz
			return 0;
			break;
		case 11:
			// unos racuna?
			break;
		default:
			printf("Nevaljan unos, pokusaj opet\n");
			break;
		}
	}
	return 0;
}

int printLists(billPosition headBill) {
	printf("Povijest racuna:\n");
	billPosition tempBill = headBill->nextBill;

	while (tempBill != NULL) {
		printf("Racun izdan: %04d-%02d-%02d \nSacuvan na lokaciji %s \nSadrzaj racuna:\n", tempBill->billDate->year, tempBill->billDate->month, tempBill->billDate->day, tempBill->location);
		articlePosition tempArticle = tempBill->headArticle->nextArticle;
		while (tempArticle != NULL) {
			printf("\t%s %d %.2f\n", tempArticle->name, tempArticle->amount, tempArticle->price);
			tempArticle = tempArticle->nextArticle;
		}
		tempBill = tempBill->nextBill;
	}


	return 0;
}

int highPriceBill(billPosition billHead) {
	double totalPrice = 0.0, max = 0.0;
	billPosition maxBill = NULL;
	billPosition tempBill = billHead->nextBill;
	while (tempBill != NULL) {
		articlePosition tempArticle = tempBill->headArticle->nextArticle;
		while (tempArticle != NULL) {
			totalPrice += tempArticle->price * tempArticle->amount;
			tempArticle = tempArticle->nextArticle;
		}
		if (totalPrice > max) { //ako je trenutni racun veci od max, postavi max na trenutni
			max = totalPrice;
			maxBill = tempBill;
		}
		totalPrice = 0.0; //resetiraj totalPrice za sljedeci racun
		tempBill = tempBill->nextBill;
	}
	if (maxBill != NULL) {
		printf("Najveci racun je izdan: %04d-%02d-%02d a iznos mu je %.2f\n", maxBill->billDate->year, maxBill->billDate->month, maxBill->billDate->day, max);
	}
	return 0;
}
int lowPriceBill(billPosition billHead) {
	double totalPrice = 0.0, min = 1000.0;
	billPosition minBill = NULL;
	billPosition tempBill = billHead->nextBill;
	while (tempBill != NULL) {
		articlePosition tempArticle = tempBill->headArticle->nextArticle;
		while (tempArticle != NULL) {
			totalPrice += tempArticle->price * tempArticle->amount;
			tempArticle = tempArticle->nextArticle;
		}
		if (totalPrice < min) {
			min = totalPrice;
			minBill = tempBill;
		}
		totalPrice = 0.0;
		tempBill = tempBill->nextBill;
	}
	if (minBill != NULL) {
		printf("Najmanji racun je izdan: %04d-%02d-%02d a iznos mu je %.2f\n", minBill->billDate->year, minBill->billDate->month, minBill->billDate->day, min);
	}
	return 0;
}

int avgPriceBill(billPosition billHead) {
	double totalPrice = 0.0;
	int billCount = 0;
	billPosition tempBill = billHead->nextBill;
	while (tempBill != NULL) {
		articlePosition tempArticle = tempBill->headArticle->nextArticle;
		while (tempArticle != NULL) {
			totalPrice += tempArticle->price * tempArticle->amount; //zbroji sve artikle na svim raèunima
			tempArticle = tempArticle->nextArticle;
		}
		billCount++; //prebroji raèune
		tempBill = tempBill->nextBill;
	}
	if (billCount > 0) {
		printf("Prosjecni iznos racuna je %.2f\n", (totalPrice / billCount)); 
	}
	return 0;
}

int mostSoldArticle(billPosition billHead) {
	int articleCount = 0, maxArticleCount = 0;
	char mostSoldArticleName[MAX_CHAR] = "";
	billPosition tempBill = billHead->nextBill;
	while (tempBill != NULL) {

		billPosition innerTempBill = tempBill;
		articlePosition tempArticle = tempBill->headArticle->nextArticle;
		while (tempArticle != NULL) {//za svaki artikal na trenutnom racunu, provjeri sve iduæe raèune
			innerTempBill = tempBill; 
			while (innerTempBill != NULL) {
				articlePosition checkArticle = innerTempBill->headArticle->nextArticle;

				while (checkArticle != NULL) {
					if (strcmp(tempArticle->name, checkArticle->name) == 0) {
						articleCount += checkArticle->amount; //zbroji kolièinu artikala
					}
					checkArticle = checkArticle->nextArticle;
				}
				innerTempBill = innerTempBill->nextBill;
			}
			if (articleCount > maxArticleCount) { //ako je trenutni artikal prodan više od max, postavi max na trenutni
				maxArticleCount = articleCount;
				strcpy(mostSoldArticleName, tempArticle->name); //zapamti ime
			}
			articleCount = 0; //resetiraj brojaè za sljedeæi artikal
			tempArticle = tempArticle->nextArticle;
		}
		tempBill = tempBill->nextBill;
	}
	printf("Najprodavaniji artikal je %s sa ukupno prodanih %d komada\n", mostSoldArticleName, maxArticleCount);

	return 0;
}

int mostProffitableArticle(billPosition billHead) { //isto kao prosla ali zbroji profit umjesto kolièine
	if (!billHead) return 0;

	double maxArticleProfit = 0.0;
	char mostProfitableArticleName[MAX_CHAR] = "";

	billPosition outerBill = billHead->nextBill;
	while (outerBill != NULL) {
		articlePosition outerArticle = NULL;

		for (outerArticle = outerBill->headArticle->nextArticle; outerArticle != NULL; outerArticle = outerArticle->nextArticle) {
			double articleProfit = 0.0;
			billPosition innerBill = billHead->nextBill;
			while (innerBill != NULL) {
				articlePosition checkArticle = NULL;
				for (checkArticle = innerBill->headArticle->nextArticle; checkArticle != NULL; checkArticle = checkArticle->nextArticle) {
					if (strcmp(outerArticle->name, checkArticle->name) == 0) {
						articleProfit += (double)checkArticle->amount * checkArticle->price;
					}
				}
				innerBill = innerBill->nextBill;
			}

			if (articleProfit > maxArticleProfit) {
				maxArticleProfit = articleProfit;
				strncpy(mostProfitableArticleName, outerArticle->name, MAX_CHAR - 1);
				mostProfitableArticleName[MAX_CHAR - 1] = '\0';
			}
		}

		outerBill = outerBill->nextBill;
	}
	printf("Najprofitabilniji artikal je %s sa ukupnim profitom od %.2f\n", mostProfitableArticleName, maxArticleProfit);

	return 0;
}

int getArticleDetails(billPosition billHead, char* articleName) {
	int totalAmount = 0;
	double totalSpent = 0.0;
	billPosition tempBill = billHead->nextBill;
	while (tempBill != NULL) {
		articlePosition tempArticle = tempBill->headArticle->nextArticle;
		while (tempArticle != NULL) {
			if (strcmp(tempArticle->name, articleName) == 0) {
				totalAmount += tempArticle->amount; //zbroji kolièinu i ukupno potrošeno za artikal na svim raèunima
				totalSpent += tempArticle->price * tempArticle->amount;
			}
			tempArticle = tempArticle->nextArticle;
		}
		tempBill = tempBill->nextBill;
	}
	if (totalAmount == 0) {
		printf("Artikal %s nije prodavan\n", articleName);
		return 0;
	}
	else {
		printf("%s je prodana %d puta za ukupno %.2f eura\n", articleName, totalAmount, totalSpent);
	}
	return 0;
}

int getArticleDetailsInPeriod(billPosition billHead, char* articleName, datePosition startDate, datePosition endDate) {
	int totalAmount = 0;
	double totalSpent = 0.0;
	billPosition tempBill = billHead->nextBill;
	while (tempBill != NULL) {
		if (!compareDates(tempBill->billDate, startDate) && !compareDates(endDate, tempBill->billDate)) { //provjeri je li datum racuna u zadanom periodu te zbroji kao i prije
			articlePosition tempArticle = tempBill->headArticle->nextArticle;
			while (tempArticle != NULL) {
				if (strcmp(tempArticle->name, articleName) == 0) {
					totalAmount += tempArticle->amount;
					totalSpent += tempArticle->price * tempArticle->amount;
				}
				tempArticle = tempArticle->nextArticle;
			}
		}
		tempBill = tempBill->nextBill;
	}
	if (totalAmount == 0) {
		printf("Artikal %s nije prodavan u zadanom periodu\n", articleName);
		return 0;
	}
	else {
		printf("%s je prodana %d puta za ukupno %.2f eura u zadanom periodu\n", articleName, totalAmount, totalSpent);
	}
	return 0;
}

int printBillInPeriod(billPosition billHead, datePosition startDate, datePosition endDate) {
	printf("Racuni u zadanom periodu:\n");
	billPosition tempBill = billHead->nextBill;
	while (tempBill != NULL) {
		if (!compareDates(tempBill->billDate, startDate) && !compareDates(endDate, tempBill->billDate)) { //provjeri je li datum racuna u zadanom periodu te ga ispiši
			printf("Racun izdan: %04d-%02d-%02d \nSacuvan na lokaciji %s \nSadržaj racuna:\n", tempBill->billDate->year, tempBill->billDate->month, tempBill->billDate->day, tempBill->location);
			articlePosition tempArticle = tempBill->headArticle->nextArticle;
			while (tempArticle != NULL) {
				printf("\t%s %d %.2f\n", tempArticle->name, tempArticle->amount, tempArticle->price);
				tempArticle = tempArticle->nextArticle;
			}
		}
		tempBill = tempBill->nextBill;
	}
	return 0;
}
