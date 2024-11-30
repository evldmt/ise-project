#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_NAME 50
#define MAX_PASSWORD 20
#define MAX_USERS 100
#define MAX_HISTORY 20
#define MAX_EMAIL 50

// Struct for storing user data
typedef struct {
    char username[MAX_NAME];
    char password[MAX_PASSWORD];
    char email[MAX_EMAIL];
    float balance;
    char transaction_history[MAX_HISTORY][100];
    int history_count;
    float daily_limit;
    float daily_transaction_total;
    char last_transaction_date[11];
} User;

// Global variables
User users[MAX_USERS];
int user_count = 0;
int logged_in_user = -1;

// Function declarations
void register_user();
void login_user();
void reset_password();
void manage_user_session();
void deposit_money();
void withdraw_money();
void transfer_money();
void view_balance();
void view_transaction_history();
void set_daily_limit();
void check_and_reset_daily_total();
void save_users();
void load_users();
int find_user_by_username(const char* username);
void clear_input_buffer();

// Main function
int main() {
    load_users();

    while (1) {
        int choice;
        printf("\n1. Register\n2. Login\n3. Exit\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                register_user();
                break;
            case 2:
                login_user();
                if (logged_in_user != -1) {
                    manage_user_session();
                }
                break;
            case 3:
                printf("Exiting program. Goodbye!\n");
                return 0;
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }
}

// Function to register a new user
void register_user() {
    char username[MAX_NAME], password[MAX_PASSWORD], email[MAX_EMAIL];

    printf("Enter username: ");
    clear_input_buffer();
    fgets(username, MAX_NAME, stdin);
    username[strcspn(username, "\n")] = '\0';

    if (find_user_by_username(username) != -1) {
        printf("Username already exists!\n");
        return;
    }

    printf("Enter email: ");
    fgets(email, MAX_EMAIL, stdin);
    email[strcspn(email, "\n")] = '\0';

    printf("Enter password: ");
    fgets(password, MAX_PASSWORD, stdin);
    password[strcspn(password, "\n")] = '\0';

    User new_user = {0};
    strcpy(new_user.username, username);
    strcpy(new_user.email, email);
    strcpy(new_user.password, password);
    new_user.daily_limit = 300000.0;

    users[user_count++] = new_user;
    save_users();
    printf("Registration successful!\n");
}

// Function to find a user by username
int find_user_by_username(const char* username) {
    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].username, username) == 0) {
            return i;
        }
    }
    return -1;
}

// Function to reset password
void reset_password() {
    char username[MAX_NAME], email[MAX_EMAIL], new_password[MAX_PASSWORD];

    printf("Enter username: ");
    clear_input_buffer();
    fgets(username, MAX_NAME, stdin);
    username[strcspn(username, "\n")] = '\0';

    int user_index = find_user_by_username(username);
    if (user_index == -1) {
        printf("User not found!\n");
        return;
    }

    printf("Enter email: ");
    fgets(email, MAX_EMAIL, stdin);
    email[strcspn(email, "\n")] = '\0';

    if (strcmp(users[user_index].email, email) != 0) {
        printf("Email does not match our records!\n");
        return;
    }

    printf("Enter new password: ");
    fgets(new_password, MAX_PASSWORD, stdin);
    new_password[strcspn(new_password, "\n")] = '\0';

    strcpy(users[user_index].password, new_password);
    save_users();
    printf("Password reset successful!\n");
}

// Function to log in a user
void login_user() {
    char username[MAX_NAME], password[MAX_PASSWORD];

    printf("Enter username: ");
    clear_input_buffer();
    fgets(username, MAX_NAME, stdin);
    username[strcspn(username, "\n")] = '\0';

    int user_index = find_user_by_username(username);
    if (user_index == -1) {
        printf("User not found!\n");
        return;
    }

    printf("Enter password: ");
    fgets(password, MAX_PASSWORD, stdin);
    password[strcspn(password, "\n")] = '\0';

    if (strcmp(users[user_index].password, password) == 0) {
        logged_in_user = user_index;
        printf("Login successful! Welcome, %s.\n", users[user_index].username);
    } else {
        printf("Incorrect password!\n");
    }
}

// Function to manage a logged-in user's session
void manage_user_session() {
    while (1) {
        int choice;
        printf("\n1. Deposit Money\n2. Withdraw Money\n3. Transfer Money\n4. View Balance\n5. View Transaction History\n6. Set Daily Limit\n7. Logout\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                deposit_money();
                break;
            case 2:
                withdraw_money();
                break;
            case 3:
                transfer_money();
                break;
            case 4:
                view_balance();
                break;
            case 5:
                view_transaction_history();
                break;
            case 6:
                set_daily_limit();
                break;
            case 7:
                logged_in_user = -1;
                printf("Logged out successfully.\n");
                return;
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }
}

// Function to deposit money
void deposit_money() {
    float amount;
    printf("Enter amount to deposit: ");
    scanf("%f", &amount);

    users[logged_in_user].balance += amount;
    printf("Deposit successful! New balance: %.2f\n", users[logged_in_user].balance);
    save_users();
}

// Function to withdraw money
void withdraw_money() {
    float amount;
    printf("Enter amount to withdraw: ");
    scanf("%f", &amount);

    if (users[logged_in_user].balance >= amount) {
        users[logged_in_user].balance -= amount;
        printf("Withdrawal successful! New balance: %.2f\n", users[logged_in_user].balance);
        save_users();
    } else {
        printf("Insufficient funds!\n");
    }
}

// Function to transfer money
void transfer_money() {
    char recipient_username[MAX_NAME];
    float amount;

    printf("Enter recipient username: ");
    clear_input_buffer();
    fgets(recipient_username, MAX_NAME, stdin);
    recipient_username[strcspn(recipient_username, "\n")] = '\0';

    int recipient_index = find_user_by_username(recipient_username);
    if (recipient_index == -1) {
        printf("Recipient not found!\n");
        return;
    }

    printf("Enter amount to transfer: ");
    scanf("%f", &amount);

    if (users[logged_in_user].balance >= amount) {
        users[logged_in_user].balance -= amount;
        users[recipient_index].balance += amount;
        printf("Transfer successful! New balance: %.2f\n", users[logged_in_user].balance);
        save_users();
    } else {
        printf("Insufficient funds!\n");
    }
}

// Function to view balance
void view_balance() {
    printf("Your balance: %.2f\n", users[logged_in_user].balance);
}

// Function to view transaction history
void view_transaction_history() {
    printf("Transaction History:\n");
    for (int i = 0; i < users[logged_in_user].history_count; i++) {
        printf("%s\n", users[logged_in_user].transaction_history[i]);
    }
}

// Function to set daily limit
void set_daily_limit() {
    float new_limit;
    printf("Enter new daily limit: ");
    scanf("%f", &new_limit);

    users[logged_in_user].daily_limit = new_limit;
    printf("New daily limit set: %.2f\n", new_limit);
    save_users();
}

// Function to clear input buffer
void clear_input_buffer() {
    while (getchar() != '\n');
}

// Function to save user data
void save_users() {
    FILE *file = fopen("users.txt", "w");
    if (!file) {
        printf("Error saving users!\n");
        return;
    }

    for (int i = 0; i < user_count; i++) {
        fprintf(file, "%s %s %s %.2f %.2f %.2f %s\n",
                users[i].username, users[i].email, users[i].password,
                users[i].balance, users[i].daily_limit,
                users[i].daily_transaction_total, users[i].last_transaction_date);
    }
    fclose(file);
}

// Function to load user data
void load_users() {
    FILE *file = fopen("users.txt", "r");
    if (!file) return;

    while (fscanf(file, "%s %s %s %f %f %f %s",
                  users[user_count].username,
                  users[user_count].email,
                  users[user_count].password,
                  &users[user_count].balance,
                  &users[user_count].daily_limit,
                  &users[user_count].daily_transaction_total,
                  users[user_count].last_transaction_date) != EOF) {
        user_count++;
    }
    fclose(file);
}
