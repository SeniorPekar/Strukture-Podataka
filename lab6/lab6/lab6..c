#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "include/loadList.h"
#include "include/options.h"
#include "include/helpFun.h"


/*6. Napisati program koji èita datoteku racuni.txt u kojoj su zapisani nazivi svih datoteka koji
predstavljaju pojedini raèun. Na poèetku svake datoteke je zapisan datum u kojem vremenu je
raèun izdat u formatu YYYY-MM-DD. Svaki sljedeæi red u datoteci predstavlja artikl u formatu
naziv, kolièina, cijena. Potrebno je formirati vezanu listu raèuna sortiranu po datumu. Svaki èvor
vezane liste sadržava vezanu listu artikala sortiranu po nazivu artikla. Nakon toga potrebno je
omoguæiti upit kojim æe korisnik saznati koliko je novaca sveukupno potrošeno na specifièni
artikl u odreðenom vremenskom razdoblju i u kojoj je kolièini isti kupljen.
*/

int main() {
	bill headBill = { .location = "", .nextBill = NULL, .headArticle = NULL, .billDate = NULL };
	loadPathFile(&headBill); // uèitaj sve raèune iz datoteke racuni.txt

	options(&headBill); // otvori meni

	freeLists(&headBill); // oslobodi svu memoriju
	return 0;
}