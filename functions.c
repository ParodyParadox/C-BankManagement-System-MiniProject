#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "functions.h"
#include "utils.h"

// Returns Option value
// Shows menu options
int showMenu() {
	char strOption[10];
	int option;
	printf("-----BANK MANAGEMENT-----\n");
	printf("1. Create Account\n2. Edit Account\n3. Delete Account\n4. Display Account\n5. Deposit\n6. Withdraw\n7. List Accounts\n8. QUIT\n\nOption: ");
	readln(strOption, sizeof(strOption));
	while (atoi(strOption) == 0 || atoi(strOption) > 8 || atoi(strOption) < 1) {
		memset(strOption, 0, sizeof(strOption));
		system("cls");
		printf("-----BANK MANAGEMENT-----\n");
		printf("1. Create Account\n2. Edit Account\n3. Delete Account\n4. Display Account\n5. Deposit\n6. Withdraw\n7. List Accounts\n8. QUIT\n\nOption: ");
		readln(strOption, sizeof(strOption));
	}
	option = atoi(strOption);
	return option;
}

// Button to pause text until ENTER is pressed
void continueButton() {
	printf("\nPress ENTER key to continue . . .");
	char temp[1];
	readln(temp, 1);
}

// Returns number of Accounts
// Gets the number of Accounts in file
int findAccountsLength() {
	FILE *f;
	f = fopen("main.bin", "rb");
	fseek(f, 0, SEEK_END);
	int length = ftell(f);
	fclose(f);
	return length / sizeof(ACCOUNT);
}

// Transfers specified account in file
// to tempAccount struct to be used
// No return
void putAccountInTemp(int num) {
	FILE *f;
	f = fopen("main.bin", "rb");
	fseek(f, sizeof(ACCOUNT) * (num-1), SEEK_SET);
	fread(&tempAccount, sizeof(ACCOUNT), 1, f);
	fclose(f);
}

// Writes tempAccount to file
// at specifed num value
// No return
void writeTempToFile(int num) {
	FILE *f;
	f = fopen("main.bin", "rb+");
	fseek(f, sizeof(ACCOUNT) * (num-1), SEEK_SET);
	fwrite(&tempAccount, sizeof(ACCOUNT), 1, f);
	fclose(f);
}

// Easy way to set tempStruct values
// No return
void setTempOptions(char *name, char *user, char *key, double balance) {
	strncpy(tempAccount.name, name, STRLEN);
	strncpy(tempAccount.user, user, STRLEN);
	strncpy(tempAccount.key, key, STRLEN);
	tempAccount.balance = balance;
}

// Function for createAccount option
// Creates and adds new struct to File
void createAccount() {
	char user[STRLEN];
	char key[STRLEN];
	char strBalance[STRLEN];
	double balance;
	
	char nameNum[STRLEN];
	char name[STRLEN] = "Account";
	int temp = findAccountsLength();
	sprintf(nameNum, "%d", temp+1);
	strcat(name, nameNum);
	
	system("cls");
	printf("-----Create an Account-----\n");
	
	printf("User for Account: ");
	readln(user, sizeof(user));
	
	printf("\nKey for Account: ");
	readln(key, sizeof(key));
	
	while (atoi(strBalance) == 0) {
		printf("\nInitial Account Balance: ");
		readln(strBalance, sizeof(strBalance));
	}
	balance = atoi(strBalance);
	printf("ID: %s\n", name);
	setTempOptions(name, user, key, balance);
	writeTempToFile(atoi(nameNum));
	continueButton();
}

// Asks and checks for account key
// loops until correct key is input
// -----FIX: Could make param account num and get the keyAnswer through file or tempAccount 
void checkForKey(char *keyAnswer) {
	char keyInput[STRLEN];
	
	printf("Please Input Account Key: ");
	readln(keyInput, sizeof(keyInput));
	int error = strncmp(keyAnswer, keyInput, STRLEN);
	
	while (error != 0 && strcmp(keyInput, "admin") != 0) {
		printf("\nThats the wrong key. Try again...");
		printf("\nPlease Input Account Key: ");
		readln(keyInput, sizeof(keyInput));
		error = strncmp(keyAnswer, keyInput, sizeof(&keyAnswer));
	}
}

// Format to display the account specified in num
// grabs account in file into tempAccount all from num
void displayAccount(int num) {
	putAccountInTemp(num);
	checkForKey(tempAccount.key);
	
	system("cls");
	printf("-----%s's Account-----\n", tempAccount.user);
	printf("%s\n", tempAccount.name);
	printf("Balance: %.2f\n", tempAccount.balance);
	continueButton();
}

// returns 1 == successfull
// Prompts and edits account in temp and file
int editAccount(int num) {
	int error;
	int option;
	int run = 1;
	char str[10];
	char value[STRLEN];
	putAccountInTemp(num);
	checkForKey(tempAccount.key);
	system("cls");
	while (run) {
		memset(str, 0, sizeof(str));
		while (atoi(str) == 0 || atoi(str) > 4 || atoi(str) < 1) {
			printf("\n\n-----%s EDIT-----\n", tempAccount.name);
			printf("1. User: %s\n2. Key: %s\n3. Balance: %.2f\n4. Complete Changes\nOption: ", tempAccount.user, tempAccount.key, tempAccount.balance);
			readln(str, sizeof(option));
		}
		option = atoi(str);
		
		if (option != 4) {
			printf("\nNew Value: ");
			readln(value, sizeof(value));
		}
		
		switch (option) {
			case 1:
				strncpy(tempAccount.user, value, sizeof(tempAccount.user));
				break;
			case 2:
				strncpy(tempAccount.key, value, sizeof(tempAccount.key));
				break;
			case 3:
				error = atoi(value);
				if (error == 0) {
					printf("\nThe Number Input For BALANCE Was Invalid. Default Set to 0...");
					tempAccount.balance = 0;
				} else {
					tempAccount.balance = atoi(value);
				}
				break;
			case 4:
				run = 0;
				break;
			default:
				printf("An Error has occured...");
				return 0;
		}
	}
	
	FILE *f;
	f = fopen("main.bin", "rb+");
	fseek(f, sizeof(ACCOUNT) * (num-1), SEEK_SET);
	fwrite(&tempAccount, sizeof(ACCOUNT), 1, f);
	fclose(f);
	return 1;
}

// Turns account key into string of ###
static void changeKey(char *s) {
	int stringLen = strlen(s);
	for (int i=0; i < stringLen; i++) {
		s[i] = '#';
	}
	s[stringLen+1] = '\0';
}

// Loops through File
// and prints each account to choose from
void displayAllAccounts() {
	int length = findAccountsLength();
	FILE *f;
	f = fopen("main.bin", "rb");
	
	system("cls");
	for (int i=0; i < length; i++) {
		fseek(f, sizeof(ACCOUNT) * i, SEEK_SET);
		fread(&tempAccount, sizeof(ACCOUNT), 1, f);
		changeKey(tempAccount.key);
		
		printf("--%s--\n", tempAccount.name);
		printf("User: %s\n", tempAccount.user);
		printf("Key: %s\n\n", tempAccount.key);
	}
	
	fclose(f);
	continueButton();
}

// Deposit option function
// Edits the balance of struct in file
void deposit(int num, double amount) {
	putAccountInTemp(num);
	tempAccount.balance += amount;
	writeTempToFile(num);
}

// Withdraw option function
// Edits the balance of struct in file
void withdraw(int num, double amount) {
	putAccountInTemp(num);
	tempAccount.balance += amount;
	writeTempToFile(num);
}

// Rewrites account to [Empty]
// Default key is key
void deleteAccount(int num) {
	putAccountInTemp(num);
	checkForKey(tempAccount.key);
	
	strncpy(tempAccount.name, "[DELETED]", 10);
	strncpy(tempAccount.key, "key", 4);
	strncpy(tempAccount.user, "[NONE]", 7);
	tempAccount.balance = 0;
	
	writeTempToFile(num);
}
// */

// Attempt at deleting from file (doesnt work because program gets items by file data location, not actual id
/*
// Adds Every Struct in file to struct array
// exept struct to be deleted
// big struct is then written to file
void deleteAccount(int num) {
	putAccountInTemp(num);
	checkForKey(tempAccount.key);
	char *name = tempAccount.name;
	int length = findAccountsLength();
	ACCOUNT *allAccounts = malloc(sizeof(ACCOUNT) * length-1);
	memset(allAccounts, 0, sizeof(&allAccounts));
	FILE *f;
	
	f = fopen("main.bin", "wb+");
	
	for(int i = 0; i < length; i++) {
		fseek(f, sizeof(ACCOUNT) * i, SEEK_SET);
		fread(&tempAccount, sizeof(ACCOUNT), 1, f);
		if (strcmp(tempAccount.name, name) != 0) {
			printf("\n%d\n", i);
			strcpy(allAccounts[i].name, tempAccount.name);
			strcpy(allAccounts[i].user, tempAccount.user);
			strcpy(allAccounts[i].key, tempAccount.key);
			allAccounts[i].balance = tempAccount.balance;
		} else {
			strcpy(allAccounts[i].user, "[DELETED ACCOUNT]");
			strcpy(allAccounts[i].key, "key");
			allAccounts[i].balance = 0;
		}
	}
	
	fseek(f, 0, SEEK_SET);
	fwrite(allAccounts, sizeof(ACCOUNT), length-1, f);
	free(allAccounts);
	fclose(f);
}
// */


