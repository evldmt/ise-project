#include <stdio.h>
#include <string.h>

#define MAX_NAME 50
#define MAX_PASSWORD 20
#define MAX_USERS 100

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
} User;

User users[MAX_USERS]; // это массив где будут хранится юзеры
int user_count = 0; // это счетчик юзеров

int main()
{
    load_users(); // Эта функция нужна чтобы добавить c файла "users.txt" всех юзеров в массив "users"
    int choice;

    while (1)
    {
        // Сначала юзер должен либо создать аккаунт или войти если у него уже есть
        // Либо закончить программу выбрав exit
        printf("1. Register\n2. Login\n3. Exit\nEnter your choice : ");
        scanf("%d", &choice);
        printf("\n");

        if (choice == 1)
        {
            register_user();
        }
        else if (choice == 2)
        {
            login_user();

            // Тут еще должна быть проверка типа проверки юзернейма и пароля

            while (1)
            {
                // ну и здесь функционал после логина, юзер выберет что делать
                printf("1. Deposit Money\n2. Withdraw Money\n3. Transfer Money\n4. View Balance\n5. View Transaction History\n6.Logout\nEnter your choice : ");
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
                    break;
                }
                else
                {
                    printf("Invalid choice!\n");
                }
            }
            
        }
        else if(choice == 3)
        {
            break;
        }
        else
        {
            printf("Invalid choice!");
        }
        
        
    }
    

    return 0;
}

void register_user()
{
    char username[MAX_NAME], password[MAX_PASSWORD];
    // Берем инпуты у юзера
    printf("Enter your login name : ");
    scanf("%s", username);
    printf("Enter the password : ");
    scanf("%s", password);

    // Потом добавляем данные в массив который создали в начале
    strcpy(users[user_count].username, username);
    strcpy(users[user_count].password, password);
    users[user_count].balance = 0;
    user_count++;
    save_user(); // Сохраняем чела

    printf("User registered successfully!\n");
    
}

void save_user() {
    FILE *file = fopen("users.txt", "w");
    for (int i = 0; i < user_count; i++) {
        fprintf(file, "%s %s %f\n", users[i].username, users[i].password, users[i].balance);
    }
    fclose(file);
}

void load_users() {
    FILE *file = fopen("users.txt", "r");
    if (file == NULL) return;

    while (fscanf(file, "%s %s %f %d", users[user_count].username, users[user_count].password, &users[user_count].balance) != EOF) {
        user_count++;
    }

    fclose(file);
}

void login_user()
{
    printf("Login\n");
}

void deposit_money()
{
    printf("Deposit Money\n");
}

void withdraw_money()
{
    printf("Withdraw Money\n");
}

void transfer_money()
{
    printf("Transfer Money\n");
}

void view_balance()
{
    printf("View balance\n");
}

void view_transaction_history()
{
    printf("View Transaction History\n");
}