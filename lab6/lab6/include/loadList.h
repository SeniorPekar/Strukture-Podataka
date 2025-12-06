#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/* loadList depends on the shared types & constants in helpFun.h */
#include "helpFun.h"

int loadPathFile(billPosition headBill); //èita lokacije raèuna iz datoteke racuni.txt
int loadBillFiles(billPosition headBill, char* p); //uèitava pojedinaène raèune iz datoteka proèitanih u prijašnjoj funkciji
int freeLists(billPosition headBill); //oslobodi memoriju svih listi
billPosition addSortedBill(billPosition headBill, datePosition newDate, char* p); //Kad se uèita novi raèun, sortira ga po datumu i dodaje na odgovarajuæe mjesto u listi raèuna

#ifdef __cplusplus
}
#endif