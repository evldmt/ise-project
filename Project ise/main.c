#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure to store account information
struct Account {
    int accountNumber;
    char name[50];
    float balance;
};

// Function prototypes
void createAccount();
void displayAccount();
void deposit();
void withdraw();
void deleteAccount();
int authenticateUser(int accountNumber);

FILE *fp;

int main() {
    int choice;

    while (1) {
        printf("\n=== Bank Management System ===\n");
        printf("1. Create Account\n");
        printf("2. Display Account\n");
        printf("3. Deposit\n");
        printf("4. Withdraw\n");
        printf("5. Delete Account\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                createAccount();
                break;
            case 2:
                displayAccount();
                break;
            case 3:
                deposit();
                break;
            case 4:
                withdraw();
                break;
            case 5:
                deleteAccount();
                break;
            case 6:
                printf("\nThank you for using the Bank Management System!\n");
                exit(0);
            default:
                printf("\nInvalid choice! Please try again.\n");
        }
    }

    return 0;
}

// Function to create a new account
void createAccount() {
    struct Account account;

    printf("\nEnter Account Number: ");
    scanf("%d", &account.accountNumber);
    printf("Enter Name: ");
    getchar();  // Consume leftover newline
    fgets(account.name, 50, stdin);
    account.name[strcspn(account.name, "\n")] = 0;  // Remove newline character
    account.balance = 0;

    fp = fopen("accounts.dat", "ab");
    if (fp == NULL) {
        printf("\nError: Unable to open file.\n");
        return;
    }

    fwrite(&account, sizeof(struct Account), 1, fp);
    fclose(fp);

    printf("\nAccount created successfully!\n");
}

// Function to display account details
void displayAccount() {
    struct Account account;
    int accountNumber, found = 0;

    printf("\nEnter Account Number: ");
    scanf("%d", &accountNumber);

    fp = fopen("accounts.dat", "rb");
    if (fp == NULL) {
        printf("\nError: Unable to open file.\n");
        return;
    }

    while (fread(&account, sizeof(struct Account), 1, fp)) {
        if (account.accountNumber == accountNumber) {
            printf("\nAccount Number: %d\n", account.accountNumber);
            printf("Name: %s\n", account.name);
            printf("Balance: %.2f\n", account.balance);
            found = 1;
            break;
        }
    }

    fclose(fp);

    if (!found) {
        printf("\nAccount not found.\n");
    }
}

// Function to deposit money
void deposit() {
    struct Account account;
    int accountNumber, found = 0;
    float amount;

    printf("\nEnter Account Number: ");
    scanf("%d", &accountNumber);

    fp = fopen("accounts.dat", "rb+");
    if (fp == NULL) {
        printf("\nError: Unable to open file.\n");
        return;
    }

    while (fread(&account, sizeof(struct Account), 1, fp)) {
        if (account.accountNumber == accountNumber) {
            printf("Enter amount to deposit: ");
            scanf("%f", &amount);
            if (amount <= 0) {
                printf("\nInvalid amount.\n");
                fclose(fp);
                return;
            }

            account.balance += amount;
            fseek(fp, -sizeof(struct Account), SEEK_CUR);
            fwrite(&account, sizeof(struct Account), 1, fp);
            found = 1;
            printf("\nDeposit successful! New balance: %.2f\n", account.balance);
            break;
        }
    }

    fclose(fp);

    if (!found) {
        printf("\nAccount not found.\n");
    }
}

// Function to withdraw money
void withdraw() {
    struct Account account;
    int accountNumber, found = 0;
    float amount;

    printf("\nEnter Account Number: ");
    scanf("%d", &accountNumber);

    fp = fopen("accounts.dat", "rb+");
    if (fp == NULL) {
        printf("\nError: Unable to open file.\n");
        return;
    }

    while (fread(&account, sizeof(struct Account), 1, fp)) {
        if (account.accountNumber == accountNumber) {
            printf("Enter amount to withdraw: ");
            scanf("%f", &amount);
            if (amount <= 0 || amount > account.balance) {
                printf("\nInvalid amount or insufficient funds.\n");
                fclose(fp);
                return;
            }

            account.balance -= amount;
            fseek(fp, -sizeof(struct Account), SEEK_CUR);
            fwrite(&account, sizeof(struct Account), 1, fp);
            found = 1;
            printf("\nWithdrawal successful! New balance: %.2f\n", account.balance);
            break;
        }
    }

    fclose(fp);

    if (!found) {
        printf("\nAccount not found.\n");
    }
}

// Function to delete an account
void deleteAccount() {
    struct Account account;
    int accountNumber, found = 0;

    printf("\nEnter Account Number: ");
    scanf("%d", &accountNumber);

    FILE *temp = fopen("temp.dat", "wb");
    fp = fopen("accounts.dat", "rb");

    if (fp == NULL || temp == NULL) {
        printf("\nError: Unable to open file.\n");
        return;
    }

    while (fread(&account, sizeof(struct Account), 1, fp)) {
        if (account.accountNumber == accountNumber) {
            found = 1;
            continue;
        }
        fwrite(&account, sizeof(struct Account), 1, temp);
    }

    fclose(fp);
    fclose(temp);

    remove("accounts.dat");
    rename("temp.dat", "accounts.dat");

    if (found) {
        printf("\nAccount deleted successfully.\n");
    } else {
        printf("\nAccount not found.\n");
    }
}
