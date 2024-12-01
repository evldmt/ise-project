#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_NAME 50
#define MAX_PASSWORD 20
#define MAX_USERS 100
#define MAX_EMAIL 50

// Структура для хранения данных пользователя
typedef struct {
    char username[MAX_NAME];
    char password[MAX_PASSWORD];
    char email[MAX_EMAIL];
    float balance;
    float daily_transaction_total;
} User;

// Глобальные переменные
User users[MAX_USERS];
int user_count = 0;
int logged_in_user = -1;

// Объявления функций
void register_user();
void login_user();
void reset_password(int user_index);
void manage_user_session();
void deposit_money();
void withdraw_money();
void transfer_money();
void view_balance();
void view_recent_transactions();
void record_transaction(const char* transaction);
void save_users();
void load_users();
int find_user_by_username(const char* username);
void clear_input_buffer();

// Основная функция
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

// Функция для регистрации нового пользователя
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

    users[user_count++] = new_user;
    save_users();
    printf("Registration successful!\n");
}

// Функция для поиска пользователя по имени
int find_user_by_username(const char* username) {
    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].username, username) == 0) {
            return i;
        }
    }
    return -1;
}

// Функция для авторизации пользователя
void login_user() {
    char username[MAX_NAME], password[MAX_PASSWORD];
    int attempts = 0;  // Счётчик попыток ввода пароля

    printf("Enter username: ");
    clear_input_buffer();
    fgets(username, MAX_NAME, stdin);
    username[strcspn(username, "\n")] = '\0';

    int user_index = find_user_by_username(username);
    if (user_index == -1) {
        printf("User not found!\n");
        return;
    }

    while (attempts < 2) {
        printf("Enter password: ");
        fgets(password, MAX_PASSWORD, stdin);
        password[strcspn(password, "\n")] = '\0';

        if (strcmp(users[user_index].password, password) == 0) {
            logged_in_user = user_index;
            printf("Login successful! Welcome, %s (%s).\n", users[user_index].username, users[user_index].email);
            return;  // Успешный вход, выходим из функции
        } else {
            attempts++;
            printf("Incorrect password! Attempts left: %d\n", 2 - attempts);
        }
    }

    // Если неправильный пароль введён дважды, предложить сбросить пароль через email
    char choice;
    printf("You have entered the wrong password 2 times. Do you want to reset your password? (y/n): ");
    clear_input_buffer();
    scanf("%c", &choice);

    if (choice == 'y' || choice == 'Y') {
        reset_password(user_index);  // Вызов функции сброса пароля
    }
}

// Функция для сброса пароля
void reset_password(int user_index) {
    char new_password[MAX_PASSWORD];
    printf("Enter a new password: ");
    clear_input_buffer();
    fgets(new_password, MAX_PASSWORD, stdin);
    new_password[strcspn(new_password, "\n")] = '\0';

    // Обновление пароля
    strcpy(users[user_index].password, new_password);
    save_users();
    printf("Password reset successful!\n");
}

// Функция для управления сессией пользователя
void manage_user_session() {
    while (1) {
        int choice;
        printf("\n1. Deposit Money\n2. Withdraw Money\n3. Transfer Money\n4. View Balance\n5. View Transactionse\n6. Logout\nEnter your choice: ");
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
                view_recent_transactions();
                break;
            case 6:
                logged_in_user = -1;
                printf("Logged out successfully.\n");
                return;
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }
}

// Функция для записи транзакции в файл
void record_transaction(const char* transaction) {
    FILE *file = fopen("transactions.txt", "a");
    if (!file) {
        printf("Error recording transaction!\n");
        return;
    }
    fprintf(file, "User: %s | %s\n", users[logged_in_user].username, transaction);
    fclose(file);
}

// Функция для пополнения баланса
void deposit_money() {
    float amount;
    printf("Enter amount to deposit: ");
    scanf("%f", &amount);

    users[logged_in_user].balance += amount;
    printf("Deposit successful! New balance: %.2f\n", users[logged_in_user].balance);

    char transaction[100];
    snprintf(transaction, sizeof(transaction), "Deposited: %.2f, New Balance: %.2f", amount, users[logged_in_user].balance);
    record_transaction(transaction);
    save_users();
}

// Функция для снятия денег
void withdraw_money() {
    float amount;
    printf("Enter amount to withdraw: ");
    scanf("%f", &amount);

    if (users[logged_in_user].balance >= amount) {
        users[logged_in_user].balance -= amount;
        printf("Withdrawal successful! New balance: %.2f\n", users[logged_in_user].balance);

        char transaction[100];
        snprintf(transaction, sizeof(transaction), "Withdrew: %.2f, New Balance: %.2f", amount, users[logged_in_user].balance);
        record_transaction(transaction);
        save_users();
    } else {
        printf("Insufficient funds!\n");
    }
}

// Функция для перевода денег
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
        printf("Transfer successful! Your new balance: %.2f\n", users[logged_in_user].balance);
        printf("Recipient %s's new balance: %.2f\n", users[recipient_index].username, users[recipient_index].balance);

        // Запись транзакции для отправителя
        char transaction[200];
        snprintf(transaction, sizeof(transaction), "Transferred: %.2f to %s, Your New Balance: %.2f", amount, recipient_username, users[logged_in_user].balance);
        record_transaction(transaction);

        // Запись транзакции для получателя
        // Запись транзакции для получателя (изменено)
        snprintf(transaction, sizeof(transaction), "User: %s | Received: %.2f from %s, New Balance: %.2f", users[recipient_index].username, amount, users[logged_in_user].username, users[recipient_index].balance);
        record_transaction(transaction);

        save_users();
    } else {
        printf("Insufficient funds!\n");
    }
}

// Функция для просмотра баланса
void view_balance() {
    printf("Your balance: %.2f\n", users[logged_in_user].balance);
}

// Функция для сохранения пользователей в файл
void save_users() {
    FILE *file = fopen("users.txt", "w");
    if (!file) {
        printf("Error saving users!\n");
        return;
    }

    for (int i = 0; i < user_count; i++) {
        fprintf(file, "%s %s %s %.2f %.2f\n", users[i].username, users[i].password, users[i].email,
                users[i].balance, users[i].daily_transaction_total);
    }

    fclose(file);
}

// Функция для загрузки пользователей из файла
void load_users() {
    FILE *file = fopen("users.txt", "r");
    if (!file) {
        printf("No users found, starting fresh.\n");
        return;
    }

    while (fscanf(file, "%s %s %s %f %f\n", users[user_count].username, users[user_count].password,
                   users[user_count].email, &users[user_count].balance, &users[user_count].daily_transaction_total) == 5) {
        user_count++;
    }

    fclose(file);
}
void view_recent_transactions() {
    FILE *file = fopen("transactions.txt", "r");
    if (!file) {
        printf("No transaction records found.\n");
        return;
    }

    char line[200];
    int found = 0; // Флаг для проверки наличия транзакций

    printf("\nTransactions for user %s:\n", users[logged_in_user].username);

    // Считываем файл построчно
    while (fgets(line, sizeof(line), file)) {
        // Проверяем, относится ли строка к текущему пользователю
        if (strstr(line, users[logged_in_user].username)) {
            printf("%s", line); // Печатаем транзакцию
            found = 1; // Устанавливаем флаг, если найдена транзакция
        }
    }

    fclose(file);

    if (!found) {
        printf("No transactions found for user %s.\n", users[logged_in_user].username);
    }
}

// Функция для очистки буфера ввода
void clear_input_buffer() {
    while (getchar() != '\n');
}
