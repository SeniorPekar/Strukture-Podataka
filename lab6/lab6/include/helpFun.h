#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

/* shared constants used across modules */
#define MAX_CHAR 150
#define FILE_ISSUE -1
#define ALLOC_ISSUE -2

/* core types (single place to avoid re-definition/circular deps) */
typedef struct date {
	int year;
	int month;
	int day;
} date;
typedef date* datePosition;

typedef struct article article;
typedef article* articlePosition;

typedef struct bill bill;
typedef bill* billPosition;

struct article {
	char name[MAX_CHAR];
	int amount;
	double price;
	articlePosition nextArticle;
	datePosition billDate;
};

struct bill {
	char location[MAX_CHAR];
	articlePosition headArticle;
	billPosition nextBill;
	datePosition billDate;
};

/* helper function prototypes */
bool compareDates(datePosition date1, datePosition date2);
int trimBuffer(char* buffer);
int readDateFromUser(datePosition outDate);

#ifdef __cplusplus
}
#endif