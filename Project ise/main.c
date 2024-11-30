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
    char username[MAX_NAME];      // Username
    char password[MAX_PASSWORD];  // Password
    char email[MAX_EMAIL];        // Email for password recovery
    float balance;                // User's balance
    char transaction_history[MAX_HISTORY][100];  // Transaction history
    int history_count;            // Number of transactions
    float daily_limit;            // Daily transfer limit
    float daily_transaction_total; // Total transactions for the day
    char last_transaction_date[11]; // Date for daily reset
} User;

User users[MAX_USERS]; // Array of all users
int user_count = 0;    // Number of registered users
int logged_in_user = -1; // Index of logged-in user

// Function declarations
void register_user();
void login_user();
void reset_password();
void deposit_money();
void withdraw_money();
void transfer_money();
void view_balance();
void view_transaction_history();
void set_daily_limit();
void check_and_reset_daily_total();
void save_users();
void load_users();
void save_transaction_history();
void load_transaction_history();
int find_user_by_username(const char* username);

// Main function
int main() {
    load_users();
    load_transaction_history();

    int choice;
    while (1) {
        printf("1. Register\n2. Login\n3. Exit\nEnter your choice: ");
        scanf("%d", &choice);
        printf("\n");

        if (choice == 1) {
            register_user();
        } else if (choice == 2) {
            login_user();
            if (logged_in_user != -1) { // Logged in successfully
                while (1) {
                    printf("1. Deposit Money\n2. Withdraw Money\n3. Transfer Money\n4. View Balance\n5. View Transaction History\n6. Set Daily Limit\n7. Logout\nEnter your choice: ");
                    scanf("%d", &choice);

                    if (choice == 1) deposit_money();
                    else if (choice == 2) withdraw_money();
                    else if (choice == 3) transfer_money();
                    else if (choice == 4) view_balance();
                    else if (choice == 5) view_transaction_history();
                    else if (choice == 6) set_daily_limit();
                    else if (choice == 7) {
                        logged_in_user = -1;
                        break;
                    } else {
                        printf("Invalid choice!\n");
                    }
                }
            } else {
                char reset_option[4];
                printf("Login failed. Forgot password? (yes/no): ");
                scanf("%s", reset_option);
                for (int i = 0; reset_option[i]; i++) reset_option[i] = tolower(reset_option[i]);

                if (strcmp(reset_option, "yes") == 0) {
                    reset_password();
                } else {
                    printf("Returning to main menu.\n");
                }
            }
        } else if (choice == 3) {
            break;
        } else {
            printf("Invalid choice! Try again.\n");
        }
    }
    return 0;
}

// Function to register a new user
void register_user() {
    char username[MAX_NAME], password[MAX_PASSWORD], email[MAX_EMAIL];
    printf("Enter your username: ");
    getchar(); // Clear newline character
    fgets(username, MAX_NAME, stdin);
    username[strcspn(username, "\n")] = '\0';

    if (find_user_by_username(username) != -1) {
        printf("User with this username already exists!\n");
        return;
    }

    printf("Enter your email: ");
    fgets(email, MAX_EMAIL, stdin);
    email[strcspn(email, "\n")] = '\0';

    printf("Enter your password: ");
    fgets(password, MAX_PASSWORD, stdin);
    password[strcspn(password, "\n")] = '\0';

    strcpy(users[user_count].username, username);
    strcpy(users[user_count].email, email);
    strcpy(users[user_count].password, password);
    users[user_count].balance = 0.0;
    users[user_count].history_count = 0;
    users[user_count].daily_limit = 300000.0;
    users[user_count].daily_transaction_total = 0.0;
    strcpy(users[user_count].last_transaction_date, ""); // Empty last transaction date

    user_count++;
    save_users();
    printf("User registered successfully!\n");
}

// Function to find user by username
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
    printf("Enter your username: ");
    getchar();
    fgets(username, MAX_NAME, stdin);
    username[strcspn(username, "\n")] = '\0';

    int user_index = find_user_by_username(username);
    if (user_index == -1) {
        printf("User not found!\n");
        return;
    }

    printf("Enter your email: ");
    fgets(email, MAX_EMAIL, stdin);
    email[strcspn(email, "\n")] = '\0';

    if (strcmp(users[user_index].email, email) != 0) {
        printf("Email does not match our records!\n");
        return;
    }

    printf("Enter your new password: ");
    fgets(new_password, MAX_PASSWORD, stdin);
    new_password[strcspn(new_password, "\n")] = '\0';

    strcpy(users[user_index].password, new_password);
    save_users();
    printf("Password reset successfully!\n");
}

// Function to log in user
void login_user() {
    char username[MAX_NAME], password[MAX_PASSWORD];
    printf("Enter your username: ");
    getchar();
    fgets(username, MAX_NAME, stdin);
    username[strcspn(username, "\n")] = '\0';

    int user_index = find_user_by_username(username);
    if (user_index == -1) {
        printf("User not found!\n");
        return;
    }

    printf("Enter your password: ");
    fgets(password, MAX_PASSWORD, stdin);
    password[strcspn(password, "\n")] = '\0';

    if (strcmp(users[user_index].password, password) == 0) {
        logged_in_user = user_index;
        printf("Login successful! Welcome, %s (%s)\n", users[user_index].username, users[user_index].email);
    } else {
        logged_in_user = -1;
        printf("Incorrect password!\n");
    }
}

// Function to deposit money
void deposit_money() {
    float amount;
    printf("Enter amount to deposit: ");
    scanf("%f", &amount);
    users[logged_in_user].balance += amount;
    printf("Deposit successful. New balance: %.2f\n", users[logged_in_user].balance);
    save_users();
}

// Function to withdraw money
void withdraw_money() {
    float amount;
    printf("Enter amount to withdraw: ");
    scanf("%f", &amount);
    if (users[logged_in_user].balance >= amount) {
        users[logged_in_user].balance -= amount;
        printf("Withdrawal successful. New balance: %.2f\n", users[logged_in_user].balance);
        save_users();
    } else {
        printf("Insufficient funds!\n");
    }
}

// Function to transfer money to another user
void transfer_money() {
    char recipient_username[MAX_NAME];
    float amount;
    printf("Enter recipient username: ");
    getchar();
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
        printf("Transfer successful! Your new balance: %.2f\n", users[logged_in_user].balance);
        printf("Recipient's new balance: %.2f\n", users[recipient_index].balance);
        save_users();
    } else {
        printf("Insufficient funds!\n");
    }
}

// Function to view the user's balance
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

// Function to set daily transaction limit
void set_daily_limit() {
    float new_limit;
    printf("Enter new daily limit: ");
    scanf("%f", &new_limit);
    users[logged_in_user].daily_limit = new_limit;
    printf("New daily limit set: %.2f\n", new_limit);
    save_users();
}

// Function to check and reset daily transaction total
void check_and_reset_daily_total() {
    time_t now = time(NULL);
    struct tm *local_time = localtime(&now);
    char current_date[11];
    snprintf(current_date, sizeof(current_date), "%02d-%02d-%04d", local_time->tm_mday, local_time->tm_mon + 1, local_time->tm_year + 1900);

    if (strcmp(users[logged_in_user].last_transaction_date, current_date) != 0) {
        users[logged_in_user].daily_transaction_total = 0;
        strcpy(users[logged_in_user].last_transaction_date, current_date);
        save_users();
    }
}

// Function to save user data to file
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

// Function to load user data from file
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
        users[user_count].history_count = 0;
        user_count++;
    }
    fclose(file);
}

// Function to load transaction history (optional)
void load_transaction_history() {
    // Implement loading transaction history if needed
}

// Function to save transaction history (optional)
void save_transaction_history() {
    // Implement saving transaction history if needed
}
