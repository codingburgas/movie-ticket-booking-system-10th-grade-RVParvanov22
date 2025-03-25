#pragma once
#include <iostream>
#include <string>
using namespace std;

void signUp()
{
    string username, password;
    cout << "Please enter a username: \n";
    cin >> username;
    cout << "Please enter a password: \n";
    cin >> password;
    cout << "Register successful!\n";
}

void loginAccount() {
    string username, password;
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;
    cout << "Login successful!\n";
}

void adminLogin() {
    string username, password;
    cout << "Enter admin username: ";
    cin >> username;
    cout << "Enter admin password: ";
    cin >> password;
    cout << "Admin login successful!\n";
}
