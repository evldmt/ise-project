#include <stdio.h>
#include <string.h>

#define MAX_NAME 50
#define MAX_PASSWORD 20
#define MAX_USERS 100
#define MAX_HISTORY 20

void login_user();
void register_user();
void deposit_money();
void withdraw_money();
void transfer_money();
void view_balance();
void view_transaction_history();
void save_user();
void load_users();
void save_transaction_history();
void load_transaction_history();
int find_user_by_username(const char* username);

typedef struct {
    char username[MAX_NAME];  // Логин пользователя
    char password[MAX_PASSWORD]; // Пароль
    float balance;  // Баланс пользователя
    char transaction_history[MAX_HISTORY][100];  // История транзакций
    int history_count;  // Количество транзакций
} User;

User users[MAX_USERS]; // Массив всех пользователей
int user_count = 0; // Количество пользователей
int logged_in_user = -1; // Индикатор текущего вошедшего пользователя

int main()
{
    load_users(); // Загрузка пользователей из файла
    load_transaction_history(); // Загрузка истории транзакций из файла
    int choice;

    while (1)
    {
        printf("1. Register\n2. Login\n3. Exit\nEnter your choice: ");
        scanf("%d", &choice);
        printf("\n");

        if (choice == 1)
        {
            register_user();
        }
        else if (choice == 2)
        {
            login_user(); // Попытка входа в систему

            if (logged_in_user != -1) // Только если логин успешен
            {
                // Основное меню после успешного входа
                while (1)
                {
                    printf("1. Deposit Money\n2. Withdraw Money\n3. Transfer Money\n4. View Balance\n5. View Transaction History\n6. Logout\nEnter your choice: ");
                    scanf("%d", &choice);

                    if (choice == 1)
                    {
                        deposit_money();
                    }
                    else if (choice == 2)
                    {
                        withdraw_money();
                    }
                    else if (choice == 3)
                    {
                        transfer_money();
                    }
                    else if (choice == 4)
                    {
                        view_balance();
                    }
                    else if (choice == 5)
                    {
                        view_transaction_history();
                    }
                    else if (choice == 6)
                    {
                        logged_in_user = -1; // Выход из системы
                        break;
                    }
                    else
                    {
                        printf("Invalid choice!\n");
                    }
                }
            }
            else
            {
                printf("Login failed! Try again.\n"); // Если логин не удался, показываем ошибку
            }
        }
        else if(choice == 3)
        {
            break; // Завершаем программу
        }
        else
        {
            printf("Invalid choice! Please try again.\n"); // Некорректный выбор
        }
    }
    return 0;
}

void register_user()
{
    char username[MAX_NAME], password[MAX_PASSWORD];
    printf("Enter your username (login): ");
    getchar(); // Чтобы очистить символ новой строки
    fgets(username, MAX_NAME, stdin);
    username[strcspn(username, "\n")] = '\0';  // Удаляем символ новой строки

    // Проверяем, существует ли уже пользователь с таким юзернеймом
    if (find_user_by_username(username) != -1) {
        printf("User with this username already exists!\n");
        return; // Выход, если юзернейм уже занят
    }

    printf("Enter the password: ");
    fgets(password, MAX_PASSWORD, stdin);
    password[strcspn(password, "\n")] = '\0';  // Удаляем символ новой строки

    strcpy(users[user_count].username, username);
    strcpy(users[user_count].password, password);
    users[user_count].balance = 0;
    users[user_count].history_count = 0;  // Инициализируем историю транзакций
    user_count++;

    save_user(); // Сохраняем данные в файл
    save_transaction_history(); // Сохраняем историю транзакций
    printf("User registered successfully!\n");
}

int find_user_by_username(const char* username)
{
    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].username, username) == 0) {
            return i; // Возвращаем индекс пользователя
        }
    }
    return -1; // Если пользователь не найден
}

void save_user() {
    FILE *file = fopen("users.txt", "w");  // Открываем файл с режимом записи (перезапись)
    if (file == NULL) {
        printf("Error opening file for writing!\n");
        return;
    }

    // Записываем данные всех пользователей
    for (int i = 0; i < user_count; i++) {
        fprintf(file, "%s %s %f\n", users[i].username, users[i].password, users[i].balance);
    }

    fclose(file);
    printf("User data saved successfully!\n");
}

void load_users() {
    FILE *file = fopen("users.txt", "r");
    if (file == NULL) return; // Если файл пуст или не найден, выходим

    while (fscanf(file, "%s %s %f", users[user_count].username, users[user_count].password, &users[user_count].balance) != EOF) {
        users[user_count].history_count = 0; // Инициализируем историю транзакций
        user_count++;
    }

    fclose(file);
}

void save_transaction_history() {
    FILE *file = fopen("transaction.txt", "w");  // Открываем файл с режимом записи (перезапись)
    if (file == NULL) {
        printf("Error opening file for writing transactions!\n");
        return;
    }

    // Записываем историю транзакций каждого пользователя
    for (int i = 0; i < user_count; i++) {
        // Сначала записываем имя пользователя
        fprintf(file, "%s\n", users[i].username);

        // Ограничиваем количество транзакций до 15
        int history_limit = (users[i].history_count > 15) ? 15 : users[i].history_count;

        // Записываем транзакции, начиная с последних
        for (int j = users[i].history_count - history_limit; j < users[i].history_count; j++) {
            fprintf(file, "\t%s\n", users[i].transaction_history[j]);
        }

        // Разделяем пользователей пустой строкой для удобства чтения
        fprintf(file, "\n");
    }

    fclose(file);
    printf("Transaction history saved successfully!\n");
}

void load_transaction_history() {
    FILE *file = fopen("transaction.txt", "r");
    if (file == NULL) return; // Если файл пуст или не найден, выходим

    char line[200];
    int user_index = -1;

    while (fgets(line, sizeof(line), file) != NULL) {
        line[strcspn(line, "\n")] = '\0'; // Убираем символ новой строки

        // Если строка не пустая, то это имя пользователя
        if (strlen(line) > 0 && line[0] != '\t') {
            user_index = find_user_by_username(line);
            if (user_index == -1) {
                continue;  // Пропускаем, если пользователя не нашли
            }
        }
        // Если строка начинается с табуляции, это транзакция
        else if (line[0] == '\t' && user_index != -1) {
            // Записываем транзакцию в историю пользователя
            if (users[user_index].history_count < MAX_HISTORY) {
                strcpy(users[user_index].transaction_history[users[user_index].history_count], line + 1); // Убираем табуляцию
                users[user_index].history_count++;
            }
        }
    }

    fclose(file);
}

void login_user()
{
    char username[MAX_NAME], password[MAX_PASSWORD];
    printf("Enter username (login): ");
    getchar(); // Чтобы очистить символ новой строки
    fgets(username, MAX_NAME, stdin);
    username[strcspn(username, "\n")] = '\0';  // Удаляем символ новой строки

    printf("Enter password: ");
    fgets(password, MAX_PASSWORD, stdin);
    password[strcspn(password, "\n")] = '\0';  // Удаляем символ новой строки

    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0) {
            logged_in_user = i; // Запоминаем индекс успешного входа
            printf("Login successful!\n");
            return; // Прерываем функцию, если вход успешен
        }
    }

    logged_in_user = -1; // Устанавливаем -1, если логин не был успешен
}

void deposit_money()
{
    float amount;
    printf("Enter the amount to deposit: ");
    scanf("%f", &amount);

    if (amount <= 0)
    {
        printf("Invalid amount!\n");
        return;
    }

    users[logged_in_user].balance += amount;

    if (users[logged_in_user].history_count < MAX_HISTORY)
    {
        snprintf(users[logged_in_user].transaction_history[users[logged_in_user].history_count],
                 sizeof(users[logged_in_user].transaction_history[users[logged_in_user].history_count]),
                 "Deposited: %.2f", amount);
        users[logged_in_user].history_count++;
    }

    save_transaction_history(); // Сохраняем транзакции в файл
    save_user(); // Сохраняем данные пользователя
    printf("Deposit successful! New balance: %.2f\n", users[logged_in_user].balance);
}

void withdraw_money()
{
    float amount;
    printf("Enter the amount to withdraw: ");
    scanf("%f", &amount);

    if (amount <= 0 || amount > users[logged_in_user].balance)
    {
        printf("Invalid amount!\n");
        return;
    }

    users[logged_in_user].balance -= amount;

    if (users[logged_in_user].history_count < MAX_HISTORY)
    {
        snprintf(users[logged_in_user].transaction_history[users[logged_in_user].history_count],
                 sizeof(users[logged_in_user].transaction_history[users[logged_in_user].history_count]),
                 "Withdrew: %.2f", amount);
        users[logged_in_user].history_count++;
    }

    save_transaction_history();
    save_user();
    printf("Withdrawal successful! New balance: %.2f\n", users[logged_in_user].balance);
}

void transfer_money()
{
    char recipient_username[MAX_NAME];
    float amount;
    printf("Enter recipient username: ");
    getchar(); // Чтобы очистить символ новой строки
    fgets(recipient_username, MAX_NAME, stdin);
    recipient_username[strcspn(recipient_username, "\n")] = '\0';

    int recipient_index = find_user_by_username(recipient_username);

    if (recipient_index == -1)
    {
        printf("Recipient not found!\n");
        return;
    }

    printf("Enter the amount to transfer: ");
    scanf("%f", &amount);

    if (amount <= 0 || amount > users[logged_in_user].balance)
    {
        printf("Invalid amount!\n");
        return;
    }

    users[logged_in_user].balance -= amount;
    users[recipient_index].balance += amount;

    if (users[logged_in_user].history_count < MAX_HISTORY)
    {
        snprintf(users[logged_in_user].transaction_history[users[logged_in_user].history_count],
                 sizeof(users[logged_in_user].transaction_history[users[logged_in_user].history_count]),
                 "Transferred: %.2f to %s", amount, recipient_username);
        users[logged_in_user].history_count++;
    }

    if (users[recipient_index].history_count < MAX_HISTORY)
    {
        snprintf(users[recipient_index].transaction_history[users[recipient_index].history_count],
                 sizeof(users[recipient_index].transaction_history[users[recipient_index].history_count]),
                 "Received: %.2f from %s", amount, users[logged_in_user].username);
        users[recipient_index].history_count++;
    }

    save_transaction_history();
    save_user();
    printf("Transfer successful! Your new balance: %.2f\n", users[logged_in_user].balance);
}

void view_balance()
{
    printf("Your balance: %.2f\n", users[logged_in_user].balance);
}

void view_transaction_history()
{
    printf("Transaction history:\n");
    for (int i = 0; i < users[logged_in_user].history_count; i++) {
        printf("%s\n", users[logged_in_user].transaction_history[i]);
    }
}
