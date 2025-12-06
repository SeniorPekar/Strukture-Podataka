#pragma once

#ifdef __cplusplus
extern "C" {
#endif
#include "helpFun.h"

int options(billPosition billHead);
int printLists(billPosition headBill);
int highPriceBill(billPosition billHead);
int lowPriceBill(billPosition billHead);
int avgPriceBill(billPosition billHead);
int mostSoldArticle(billPosition billHead);
int mostProffitableArticle(billPosition billHead);
int getArticleDetails(billPosition billHead, char* articleName);
int getArticleDetailsInPeriod(billPosition billHead, char* articleName, datePosition startDate, datePosition endDate);
int printBillInPeriod(billPosition billHead, datePosition startDate, datePosition endDate);

#ifdef __cplusplus
}
#endif