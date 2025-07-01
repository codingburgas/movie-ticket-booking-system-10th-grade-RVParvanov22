#pragma once
#include <string>
#include <nlohmann/json.hpp>


class AdminSystem {
public:
    AdminSystem();

    void adminMenu();

    void addMovie();
    void deleteMovie();
    void editMovie();

    void addProjection();
    void deleteProjection();
    void editProjection();

private:

    void displayAdminOptions();
    void handleOption(char option);
};