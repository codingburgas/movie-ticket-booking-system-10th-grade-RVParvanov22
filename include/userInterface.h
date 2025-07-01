#pragma once
#include "pch.h"
#include "adminSystem.h"

class UI
{
public:
    UI();
    ~UI();

    void mainMenu();

private:
    void startScreen();
    void registerUI();
    void registerAsAdmin();
    void logInUi();
    void displayCategoriesMenu();
    void displayHorrorMenu();
    void displayFantasyMenu();
    void displayThrillerMenu();

    void adminMenu(); 
    User* user;
    AdminSystem adminSystem;
};