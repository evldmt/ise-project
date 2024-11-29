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

typedef struct {
    char username[MAX_NAME];
    char password[MAX_PASSWORD];
    float balance;
    char transaction_history[MAX_HISTORY][100];
    int history_count;
} User;

User users[MAX_USERS]; // Массив всех пользователей
int user_count = 0; // Количество пользователей
int logged_in_user = -1; // Индикатор текущего вошедшего пользователя

int main()
{
    load_users(); // Загрузка пользователей из файла
    int choice;

    while (1)
    {
        printf("1. Register\n2. Login\n3. Exit\nEnter your choice : ");
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
                    printf("1. Deposit Money\n2. Withdraw Money\n3. Transfer Money\n4. View Balance\n5. View Transaction History\n6. Logout\nEnter your choice : ");
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
    printf("Enter your login name : ");
    getchar(); // Чтобы очистить символ новой строки
    fgets(username, MAX_NAME, stdin);
    username[strcspn(username, "\n")] = '\0';  // Удаляем символ новой строки

    printf("Enter the password : ");
    fgets(password, MAX_PASSWORD, stdin);
    password[strcspn(password, "\n")] = '\0';  // Удаляем символ новой строки

    strcpy(users[user_count].username, username);
    strcpy(users[user_count].password, password);
    users[user_count].balance = 0;
    users[user_count].history_count = 0;  // Инициализируем историю транзакций
    user_count++;

    save_user(); // Сохраняем данные в файл

    printf("User registered successfully!\n");
}

void save_user() {
    // Используем режим "w" для перезаписи файла
    FILE *file = fopen("users.txt", "w");  // Открываем файл с режимом записи (перезапись)
    if (file == NULL) {
        printf("Error opening file for writing!\n");
        return;
    }

    // Записываем данные всех пользователей
    for (int i = 0; i < user_count; i++) {
        fprintf(file, "%s %s %f\n", users[i].username, users[i].password, users[i].balance);
        // Записываем историю транзакций
        for (int j = 0; j < users[i].history_count; j++) {
            fprintf(file, "%s\n", users[i].transaction_history[j]);
        }
    }

    // Закрываем файл
    fclose(file);
    printf("User data saved successfully!\n");
}

void load_users() {
    FILE *file = fopen("users.txt", "r");
    if (file == NULL) return; // Если файл пуст или не найден, выходим

    while (fscanf(file, "%s %s %f", users[user_count].username, users[user_count].password, &users[user_count].balance) != EOF) {
        // Загружаем пользователей
        users[user_count].history_count = 0; // Инициализируем историю транзакций
        // Загружаем историю транзакций
        while (fgets(users[user_count].transaction_history[users[user_count].history_count], 100, file) != NULL) {
            // Убираем символ новой строки в конце каждой транзакции
            users[user_count].transaction_history[users[user_count].history_count][strcspn(users[user_count].transaction_history[users[user_count].history_count], "\n")] = '\0';
            users[user_count].history_count++;
            if (users[user_count].history_count >= MAX_HISTORY) break;
        }
        user_count++;
    }

    fclose(file);
}

void login_user()
{
    char username[MAX_NAME], password[MAX_PASSWORD];
    printf("Enter username: ");
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

    printf("Amount deposited successfully!\n");
    save_user();
}

void withdraw_money()
{
    float amount;
    printf("Enter the amount to withdraw: ");
    scanf("%f", &amount);

    if (amount <= 0)
    {
        printf("Invalid amount!\n");
        return;
    }

    if (amount > users[logged_in_user].balance)
    {
        printf("Insufficient balance!\n");
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

    printf("Amount withdrawn successfully!\n");
    save_user();
}

void transfer_money()
{
    char recipient[MAX_NAME];
    float amount;
    int recipient_found = 0;

    printf("Enter the username of the recipient: ");
    getchar(); // Чтобы очистить символ новой строки
    fgets(recipient, MAX_NAME, stdin);
    recipient[strcspn(recipient, "\n")] = '\0';  // Удаляем символ новой строки

    for (int i = 0; i < user_count; i++)
    {
        if (strcmp(users[i].username, recipient) == 0)
        {
            recipient_found = 1;
            break;
        }
    }

    if (!recipient_found)
    {
        printf("Recipient not found!\n");
        return;
    }

    printf("Enter the amount to transfer: ");
    scanf("%f", &amount);

    if (amount <= 0)
    {
        printf("Invalid amount!\n");
        return;
    }

    if (amount > users[logged_in_user].balance)
    {
        printf("Insufficient balance!\n");
        return;
    }

    users[logged_in_user].balance -= amount;

    for (int i = 0; i < user_count; i++)
    {
        if (strcmp(users[i].username, recipient) == 0)
        {
            users[i].balance += amount;
            break;
        }
    }

    if (users[logged_in_user].history_count < MAX_HISTORY)
    {
        snprintf(users[logged_in_user].transaction_history[users[logged_in_user].history_count],
                 sizeof(users[logged_in_user].transaction_history[users[logged_in_user].history_count]),
                 "Transferred: %.2f to %s", amount, recipient);
        users[logged_in_user].history_count++;
    }

    printf("Amount transferred successfully!\n");
    save_user();
}

void view_balance()
{
    printf("Your current balance is: %.2f\n", users[logged_in_user].balance);
}

void view_transaction_history()
{
    printf("Transaction history:\n");
    for (int i = 0; i < users[logged_in_user].history_count; i++)
    {
        printf("%s\n", users[logged_in_user].transaction_history[i]);
    }
}
