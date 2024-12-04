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
void view_transactions_by_date();
void record_transaction_for_user(const char* transaction);
void save_users();
void load_users();
int find_user_by_username(const char* username);
void clear_input_buffer();
int get_valid_choice(int min, int max);

// Основная функция
int main() {
    load_users();

    while (1) {
        printf("\n--- Main Menu ---\n");
        printf("1. Register\n");
        printf("2. Login\n");
        printf("3. Exit\n");
        int choice = get_valid_choice(1, 3);

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
        }
    }
}

// Функция для регистрации нового пользователя
void register_user() {
    char username[MAX_NAME], password[MAX_PASSWORD], email[MAX_EMAIL];

    printf("\n--- Register ---\n");
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
    new_user.balance = 0.0f;

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
void get_time(char *datetime, size_t size) {
    time_t current_time;
    struct tm *local_time;

    // Получаем текущее время
    current_time = time(NULL);
    if (current_time == -1) {
        printf("Error getting current time\n");
        return;
    }

    // Преобразуем текущее время в структуру tm
    local_time = localtime(&current_time);
    if (local_time == NULL) {
        printf("Error converting time to local time\n");
        return;
    }

    // Форматируем строку с датой и временем
    snprintf(datetime, size, "%02d-%02d-%04d,%02d:%02d:%02d",
             local_time->tm_mday,           // День
             local_time->tm_mon + 1,        // Месяц (с 0, поэтому +1)
             local_time->tm_year + 1900,    // Год (с 1900, поэтому +1900)
             local_time->tm_hour,           // Часы
             local_time->tm_min,
             local_time->tm_sec);          
}

void login_user() {
    char username[MAX_NAME], password[MAX_PASSWORD];
    int attempts = 0;
    
    printf("\n--- Login ---\n");
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
            return;
        } else {
            attempts++;
            printf("Incorrect password! Attempts left: %d\n", 2 - attempts);
        }
    }
    
    // After 2 failed attempts, prompt for password reset
    char choice;
    while (1) {
        printf("You have entered the wrong password 2 times. Do you want to reset your password? (y/n): ");
        scanf("%c", &choice);
        
        if (choice == 'y' || choice == 'Y') {
            reset_password(user_index);  // Call reset password function
            return; // Exit the login function
        } else if (choice == 'n' || choice == 'N') {
            printf("Password reset canceled.\n");
            return; // Exit the login function
        } else {
            printf("Invalid input. Please enter 'y' or 'n'.\n");
        }
    }
}

// Функция для сброса пароля
void reset_password(int user_index) {
    char email[MAX_EMAIL];
    char new_password[MAX_PASSWORD];
    int retry_count = 0;

    while (retry_count < 3) {  // Даем 3 попытки для ввода email
        printf("Enter your email: ");
        fgets(email, MAX_EMAIL, stdin);
        email[strcspn(email, "\n")] = '\0';

        // Проверяем, совпадает ли введённый email с записанным
        if (strcmp(users[user_index].email, email) != 0) {
            retry_count++;
            printf("Email does not match our records! Attempts left: %d\n", 3 - retry_count);
            if (retry_count == 3) {
                printf("You have exceeded the maximum attempts for resetting the password.\n");
                return;  // Прекращаем попытки сброса пароля после 3 неверных вводов
            }
        } else {
            // Если email совпал, спрашиваем новый пароль
            printf("Enter your new password: ");
            fgets(new_password, MAX_PASSWORD, stdin);
            new_password[strcspn(new_password, "\n")] = '\0';

            // Обновляем пароль пользователя
            strcpy(users[user_index].password, new_password);
            save_users();
            printf("Password reset successfully!\n");
            return;  // Успешный сброс пароля
        }
    }
}

// Функция для управления сессией пользователя
void manage_user_session() {
    while (1) {
        printf("\n--- User Menu ---\n");
        printf("1. Deposit Money\n");
        printf("2. Withdraw Money\n");
        printf("3. Transfer Money\n");
        printf("4. View Balance\n");
        printf("5. View Recent Transactions\n");
        printf("6. Logout\n");
        int choice = get_valid_choice(1, 6);

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
        }
    }
}

// Функция для записи транзакции
void record_transaction_for_user(const char* transaction) {
    char filename[MAX_NAME + 15];
    snprintf(filename, sizeof(filename), "%s_transactions.txt", users[logged_in_user].username);
    FILE *file = fopen(filename, "a");
    if (!file) {
        printf("Error recording transaction!\n");
        return;
    }
    fprintf(file, "%s\n", transaction);
    fclose(file);
}

// Функции пополнения, снятия, перевода и просмотра баланса
void deposit_money() {
    char datetime[50];
    float amount;
    get_time(datetime, sizeof(datetime));
    printf("Enter amount to deposit: ");
    scanf("%f", &amount);

    users[logged_in_user].balance += amount;
    printf("Deposit successful! New balance: %.2f\n", users[logged_in_user].balance);

    char transaction[100];
    snprintf(transaction, sizeof(transaction), "Deposited: %.2f, New Balance: %.2f. Date And Time: %s", amount, users[logged_in_user].balance,datetime);
    record_transaction_for_user(transaction);
    save_users();
}

void withdraw_money() {
    float amount;
    char datetime[50];
    get_time(datetime, sizeof(datetime));
    printf("Enter amount to withdraw: ");
    scanf("%f", &amount);

    if (users[logged_in_user].balance >= amount) {
        users[logged_in_user].balance -= amount;
        printf("Withdrawal successful! New balance: %.2f\n", users[logged_in_user].balance);

        char transaction[100];
        snprintf(transaction, sizeof(transaction), "Withdrew: %.2f, New Balance: %.2f. Date And Time: %s ", amount, users[logged_in_user].balance,datetime);
        record_transaction_for_user(transaction);
        save_users();
    } else {
        printf("Insufficient funds!\n");
    }
}

void transfer_money() {
    char recipient_username[MAX_NAME];
    float amount;
    char datetime[50];
    get_time(datetime,sizeof(datetime));

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

        // Запись транзакции для отправителя
        char sender_transaction[200];
        snprintf(sender_transaction, sizeof(sender_transaction), "Transferred: %.2f to %s, New Balance: %.2f. Date And Time: %s ",
                 amount, recipient_username, users[logged_in_user].balance, datetime);
        record_transaction_for_user(sender_transaction);

        // Запись транзакции для получателя
        char recipient_transaction[200];
        snprintf(recipient_transaction, sizeof(recipient_transaction), "Received: %.2f from %s, New Balance: %.2f. Date And Time: %s ",
                 amount, users[logged_in_user].username, users[recipient_index].balance, datetime);
        
        // Создаем файл транзакций получателя
        char filename[MAX_NAME + 15];
        snprintf(filename, sizeof(filename), "%s_transactions.txt", users[recipient_index].username);
        FILE *recipient_file = fopen(filename, "a");
        if (recipient_file) {
            fprintf(recipient_file, "%s\n", recipient_transaction);
            fclose(recipient_file);
        } else {
            printf("Error recording transaction for recipient!\n");
        }

        save_users();
    } else {
        printf("Insufficient funds!\n");
    }
}
void view_balance() {
    printf("Your balance: %.2f\n", users[logged_in_user].balance);
}

void view_recent_transactions() {
    char filename[MAX_NAME + 15];
    snprintf(filename, sizeof(filename), "%s_transactions.txt", users[logged_in_user].username);
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("No transaction records found.\n");
        return;
    }

    // Считаем количество транзакций
    char line[200];
    int total_transactions = 0;
    while (fgets(line, sizeof(line), file)) {
        total_transactions++;
    }
    
    // Переходим в начало файла
    fseek(file, 0, SEEK_SET);

    // Показываем только последние 10 транзакций
    int start_line = total_transactions - 10;
    int current_line = 0;

    printf("\nLatest 10 transactions for user %s:\n", users[logged_in_user].username);
    while (fgets(line, sizeof(line), file)) {
        if (current_line >= start_line) {
            printf("%s", line);
        }
        current_line++;
    }

    fclose(file);

    // Запрашиваем у пользователя выбор дальнейших действий
    char choice;
    while (1) {
        printf("\nDo you want to:\n");
        printf("1. Search transactions by date\n");
        printf("2. Return to the main menu\n");
        printf("Enter your choice (1 or 2): ");
        clear_input_buffer();
        scanf("%c", &choice);

        if (choice == '1') {
            view_transactions_by_date();  // Переходим к поиску транзакций по дате
            break;  // Завершаем текущую функцию
        } else if (choice == '2') {
            return;  // Возвращаемся в главное меню
        } else {
            printf("Invalid choice. Please enter 1 or 2.\n");
        }
    }
}

void view_transactions_by_date() {
    char date[11];
    printf("Enter the date to search (DD-MM-YYYY): ");
    clear_input_buffer();
    fgets(date, sizeof(date), stdin);
    date[strcspn(date, "\n")] = '\0';  // Убираем символ новой строки

    char filename[MAX_NAME + 15];
    snprintf(filename, sizeof(filename), "%s_transactions.txt", users[logged_in_user].username);
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("No transaction records found.\n");
        return;
    }

    char line[200];
    int found = 0;

    printf("\nTransactions on %s for user %s:\n", date, users[logged_in_user].username);
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, date)) {  // Проверяем, содержит ли строка дату
            printf("%s", line);
            found = 1;
        }
    }

    if (!found) {
        printf("No transactions found for the specified date.\n");
    }

    fclose(file);
}

// Валидация ввода
int get_valid_choice(int min, int max) {
    int choice;
    while (1) {
        printf("Enter your choice (%d-%d): ", min, max);
        if (scanf("%d", &choice) == 1 && choice >= min && choice <= max) {
            return choice;
        }
        printf("Invalid input. Please try again.\n");
        clear_input_buffer();
    }
}

// Чтение и запись пользователей в файл
void save_users() {
    FILE *file = fopen("users.txt", "w");
    if (!file) {
        printf("Error saving users!\n");
        return;
    }

    for (int i = 0; i < user_count; i++) {
        fprintf(file, "%s %s %s %.2f\n", users[i].username, users[i].password, users[i].email, users[i].balance);
    }
    fclose(file);
}

void load_users() {
    FILE *file = fopen("users.txt", "r");
    if (!file) return;

    while (fscanf(file, "%s %s %s %f",
           users[user_count].username,
           users[user_count].password,
           users[user_count].email,
           &users[user_count].balance) == 4) {
        user_count++;
    }
    fclose(file);
}

// Очистка буфера ввода
void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}





