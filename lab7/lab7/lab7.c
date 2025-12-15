#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_CHAR 50
#define ALLOC_ISSUE -1


typedef struct stack stack;
typedef struct dir dir;
typedef stack* stackPosition;
typedef dir* dirPosition;

struct stack {
	stackPosition next; //pokazivac na iduci direktorij 
	dirPosition dirLvl; //pokazivac na direktorij trenutne razine
};

struct dir {
	char name[MAX_CHAR];
	dirPosition nextDir; /* brat */
	dirPosition subDir;  /* dijete */
};

int push(stackPosition stackHead, dirPosition newDir);
int pop(stackPosition stackHead);
int freeAll(stackPosition stackHead);
static int freeDirectories(dirPosition current);
int addSorted(dirPosition *head, dirPosition newDir);

int makeDir(stackPosition stackHead, char* name);
int changeDir(stackPosition stackHead, char* destination);
int printDir(dirPosition current);
static int printDirRecursive(dirPosition current, int depth);
int pwd(stackPosition stackHead);
int menu(stackPosition stackHead);

int main() {
	dir *root = (dir*)malloc(sizeof(dir));
	if (!root) {
		printf("Greska pri alokaciji memorije\n");
		return ALLOC_ISSUE;
	}
	strncpy(root->name, "C:", MAX_CHAR - 1);
	root->name[MAX_CHAR - 1] = '\0';
	root->nextDir = NULL;
	root->subDir = NULL;

	stack stackHead = { .next = NULL, .dirLvl = root };
	int status = menu(&stackHead);

	freeAll(&stackHead); 

	if (status != 0) {
		printf("Greska pri radu programa\n");
		return status;
	}
	return 0;
}

int push(stackPosition stackHead, dirPosition newDir) {
	stackPosition newStackNode = (stackPosition)malloc(sizeof(stack));
	if (!newStackNode)
		return ALLOC_ISSUE;

	newStackNode->dirLvl = stackHead->dirLvl;
	newStackNode->next = stackHead->next;
	stackHead->next = newStackNode;

	stackHead->dirLvl = newDir;
	return 0;
}

int pop(stackPosition stackHead) {
	if (stackHead->next == NULL)
		return -2; 

	stackPosition top = stackHead->next;
	stackHead->dirLvl = top->dirLvl; 
	stackHead->next = top->next;
	free(top);
	return 0;
}

int menu(stackPosition stackHead) {
	char line[2 * MAX_CHAR];
	char *cmd = NULL;
	char *arg = NULL;
	int status = 0;

	printf("Supported commands: md <name>, cd <name>, cd .. (or cd..), dir, pwd, exit\n");

	while (1) {
		printf("> ");
		if (!fgets(line, sizeof(line), stdin)) {
			clearerr(stdin);
			printf("Input error, try again\n");
			continue;
		}

		line[strcspn(line, "\r\n")] = '\0';

		/* zanemari prazne unose */
		if (line[0] == '\0')
			continue;

		/* parsiraj naredbu i argument */
		cmd = strtok(line, " \t");
		if (!cmd)
			continue;
		arg = strtok(NULL, " \t");

		if (strcmp(cmd, "cd..") == 0) {
			status = pop(stackHead);
			if (status == ALLOC_ISSUE) return status;
			if (status == -2) printf("Vec ste u C: \n");
			continue;
		}

		if (strcmp(cmd, "md") == 0) {
			if (!arg) {
				printf("Usage: md <name>\n");
				continue;
			}
			status = makeDir(stackHead, arg);
			if (status == ALLOC_ISSUE) return status;
			if (status != 0) printf("Greska pri stvaranju direktorija\n");
		}
		else if (strcmp(cmd, "cd") == 0) {
			if (!arg) {
				printf("Usage: cd <name> or cd ..\n");
				continue;
			}
			if (strcmp(arg, "..") == 0) {
				status = pop(stackHead);
				if (status == ALLOC_ISSUE) return status;
				if (status == -2) printf("Vec ste u C: \n");
				continue;
			}
			status = changeDir(stackHead, arg);
			if (status == ALLOC_ISSUE) return status;
			
		}
		else if (strcmp(cmd, "dir") == 0) {
			status = printDir(stackHead->dirLvl);
			if (status != 0) return status;
		}
		else if (strcmp(cmd, "pwd") == 0) {
			status = pwd(stackHead);
			if (status != 0) return status;
		}
		else if (strcmp(cmd, "exit") == 0) {
			return 0;
		}
		else {
			printf("Nevaljan unos, pokusajte ponovo\n");
		}
	}
	return 0;
}

int makeDir(stackPosition stackHead, char* name) {

	dirPosition newDir = (dirPosition)malloc(sizeof(dir));
	if (!newDir)
		return ALLOC_ISSUE;

	strncpy(newDir->name, name, MAX_CHAR - 1);
	newDir->name[MAX_CHAR - 1] = '\0';
	newDir->subDir = NULL;
	newDir->nextDir = NULL;

	int r = addSorted(&stackHead->dirLvl->subDir, newDir);
	if (r != 0) {
		free(newDir);
		return r;
	}
	return 0;
}

int addSorted(dirPosition *head, dirPosition newDir){

	if (*head == NULL) {
		*head = newDir;
		newDir->nextDir = NULL;
		return 0;
	}

	dirPosition prev = NULL;
	dirPosition cur = *head;

	while (cur != NULL && strcmp(cur->name, newDir->name) < 0) {
		prev = cur;
		cur = cur->nextDir;
	}
	/* provjeri ako direktorij s tim imenom vec postiji */
	if (cur != NULL && strcmp(cur->name, newDir->name) == 0) {
		return -5; 
	}

	if (prev == NULL) {
		newDir->nextDir = *head;
		*head = newDir;
	}
	else {
		newDir->nextDir = prev->nextDir;
		prev->nextDir = newDir;
	}
	return 0;
}

int changeDir(stackPosition stackHead, char* destination) {
	dirPosition temp = stackHead->dirLvl->subDir;
	if (strcmp(destination, "..") == 0) { //provjeri ako je unos za povratak u roditeljski direktorij
		return pop(stackHead);
	}
	while ((temp != NULL) && (strcmp(temp->name, destination) != 0)) { //trazi poddirektorij s upisanim imenom
		temp = temp->nextDir;
	}
	if (temp == NULL) {
		printf("Direktorij ne postoji\n");
		return -3; 
	}
	return push(stackHead, temp);
}

int printDir(dirPosition current) {
	//ako nema poddirektorija prijavi
	if (current->subDir == NULL) {
		printf("Prazan direktorij\n");
		return 0;
	}

	printDirRecursive(current->subDir, 0);
	return 0;
}

static int printDirRecursive(dirPosition current, int depth)
{
	while (current != NULL) {
		//"tab" razmaci za dubinu od pocetnog direktorija
		for (int i = 0; i < depth+1; ++i)
			printf("  ");
		printf("%s\n", current->name);

		//rekurziraj za djecu
		if (current->subDir)
			printDirRecursive(current->subDir, depth + 1);

		//prijeði na brata
		current = current->nextDir;
	}
	return 0;
}

int pwd(stackPosition stackHead) {
	stackPosition temp = stackHead;
	stackPosition end = NULL;
	while (temp->next != NULL)
		temp = temp->next; //nadji root

	end = temp; //zapamti ga
	stackPosition prev = NULL;

	do {
		temp = stackHead; //ispisi ime i dekrementiraj end pomocu prev
		prev = temp;
		while (temp !=end) {
			prev = temp;
			temp = temp->next;
		}
		printf("%s/", end->dirLvl->name);
		end = prev;
	} while (temp != end);
	printf("\n");
	return 0;
}

int freeAll(stackPosition stackHead) {
	/* nadji root (tjst. C:)*/
	dirPosition root = NULL;
	if (stackHead->next == NULL) {
		root = stackHead->dirLvl;
	} else {
		stackPosition cur = stackHead->next;
		while (cur->next != NULL)
			cur = cur->next;
		root = cur->dirLvl;
	}
	freeDirectories(root); //rekurzivno oslobodi direktorije

	/* oslobodi stack */
	while (stackHead->next != NULL) {
		stackPosition temp = stackHead->next;
		stackHead->next = temp->next;
		free(temp);
	}

	return 0;
}
static int freeDirectories(dirPosition current)
{
	if (current == NULL) //rekurzirajno oslobaðaj dok svaka ruta ne dodje do null
		return 0;
	freeDirectories(current->subDir); //prvo oslobodi poddirektorije i braæu
	freeDirectories(current->nextDir);

	free(current);
	return 0;
}