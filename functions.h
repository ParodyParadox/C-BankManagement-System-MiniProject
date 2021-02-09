#include <stdio.h>

#define STRLEN 100

typedef struct account {
	char name[STRLEN];
	char key[STRLEN];
	char user[STRLEN];
	double balance;
} ACCOUNT;

ACCOUNT tempAccount;

int showMenu();
void continueButton();
int findAccountsLength();
void putAccountInTemp(int);
void writeTempToFile(int);
void setTempOptions(char*, char*, char*, double);
void createAccount();
void deleteAccount(int);
void displayAccount(int);
void displayAllAccounts();
int editAccount(int);
void deposit(int, double);
void withdraw(int, double);
void checkForKey(char*);
