#include <iostream>
#include <string>
#include "movie.h"
using namespace std;

int main()
{
    while (true) {
        cout << "How can I help you?\n";
        cout << "1.Regiter\n";
        cout << "2.Login\n";
        cout << "3.Admin Login\n";
        cout << "4.Leave\n";
        cout << "Enter your choice: ";

        int choice;
        cin >> choice;

        switch (choice)
        {
        case 1:
            signUp();
            break;
        case 2:
            loginAccount();
            break;
        case 3:
            adminLogin();
            break;
        case 4:
            cout << "Leaving the system.\n";
            return 0;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    }
    return 0;
}

