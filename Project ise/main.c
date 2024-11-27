#include <stdio.h>

void login_user();
void register_user();
void deposit_money();
void withdraw_money();
void transfer_money();
void view_balance();
void view_transaction_history();

int main()
{
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
    printf("Register\n");
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