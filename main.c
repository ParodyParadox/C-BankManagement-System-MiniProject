#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "functions.h"
#include "utils.h"

void askForOptions(char[], int);
int checkForAccount(int);

int main(int argc, char **argv)
{
	// add total accounts option
	int number;
	int option;
	int error;
	char temp[STRLEN];
	while (1) {
		system("cls");
		option = showMenu();
		switch (option) {
			case 1: // Create Account
				createAccount();
				printf("\nSucessfully Created Account for \"%s\"\n", tempAccount.user);
				break;
			case 2: // Edit Account
				memset(temp, 0, sizeof(temp));
				while(atoi(temp) == 0 || checkForAccount(atoi(temp)) == 0) {
					printf("\nEdit Which Account? ");
					readln(temp, sizeof(temp));
				}
				error = editAccount(atoi(temp));
				if (error == 1) {
					printf("\nSucessfully Saved Edit To %s", tempAccount.name);
					continueButton();
				} else {
					printf("\nError Occurred Editing File...");
					continueButton();
				}
				break;
			case 3: // Delete Account
				memset(temp, 0, sizeof(temp));
				while(atoi(temp) == 0 || checkForAccount(atoi(temp)) == 0) {
					printf("\nDelete Which Account? ");
					readln(temp, sizeof(temp));
				} 
				number = atoi(temp);
				memset(temp, 0, sizeof(temp));
				while (strncmp(temp, "y", 1) != 0 && strncmp(temp, "n", 1) != 0) {
					printf("\nAre you sure (y/n)? ");
					readln(temp, 2);
				}
				if (strncmp(temp, "n", 1) == 0) {
					continueButton();
				} else {
					deleteAccount(number);
					printf("Successfully Deleted Account...");
					continueButton();
				}
				
				break;
			case 4: // Display options
				memset(temp, 0, sizeof(temp));
				while(atoi(temp) == 0 || checkForAccount(atoi(temp)) == 0) {
					printf("\nWhich Account Will be Displayed? ");
					readln(temp, sizeof(temp));
				}
				displayAccount(atoi(temp));
				break;
			case 5: // Deposit
				memset(temp, 0, sizeof(temp));
				while(atoi(temp) == 0 || checkForAccount(atoi(temp)) == 0) {
					printf("\nDeposit to Account? ");
					readln(temp, sizeof(temp));
				}
				// Capital letter for display
				askForOptions("Deposit", atoi(temp));
				break;
			case 6: // Withdraw
				memset(temp, 0, sizeof(temp));
				while(atoi(temp) == 0 || checkForAccount(atoi(temp)) == 0) {
					printf("\nWithdraw from Account? ");
					readln(temp, sizeof(temp));
				}
				// Capital letter for display
				askForOptions("Withdraw", atoi(temp));
				break;
			case 7: // List Accounts
				displayAllAccounts();
				break;
			case 8: // Quit
				exit(0);
			default:
				printf("oops, an error occured...\n");
				return 1;
		};
	}
	return 0;
}

// For deposit and withdraw function
// prompts user and make everything look nice
void askForOptions(char option[], int num) {
	char str[STRLEN];
	double amount;
	putAccountInTemp(num);
	checkForKey(tempAccount.key);
	
	system("cls");
	printf("%s Amount: ", option);
	readln(str, sizeof(str));
	while (atoi(str) == 0 || (strncmp(option, "Withdraw", 7) == 0 && tempAccount.balance - atoi(str) < 0)) {
		printf("Invalid Input...\n");
		printf("\n%s Amount: ", option);
		readln(str, sizeof(str));
	}
	amount = atoi(str);
	if (strncmp(option, "Withdraw", 7) == 0) {
		tempAccount.balance -= amount;
	} else {
		tempAccount.balance += amount;
	}
	writeTempToFile(num);
	printf("New Balance: %.2f\n", tempAccount.balance);
	printf("Successfully Completed Transaction!");
	continueButton();
}

// Checks if account number exsists
// Returns 1 if account is real
int checkForAccount(int num) {
	int length = findAccountsLength();
	if (num > length || num < 1) {
		return 0;
	} else {
		return 1;
	}
}