#include "pch.h"
#include "user.h"
#include "utils.h"
#include "adminSystem.h"
#include "ReservationSystem.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

UI::UI()
{
    this->user = new User;
    startScreen();
}

UI::~UI() {
    delete user;
}

void UI::startScreen()
{
    Utiles::SetColor(33);
    Utiles::displayFile("../assets/graphic/startScreen.txt");

    while (true) {
        if (std::cin.get() == '\n') {
            mainMenu();
            break;
        }
    }
    Utiles::resetColor();
}

void UI::mainMenu() {
    Utiles::displayFile("../assets/graphic/header.txt");

    char choice;

    while (true) {
        std::cout << "Choice: ";
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice) {
        case '1':
            registerUI();
            mainMenu();
            break;
        case '2':
            logInUi();
            mainMenu();
            break;
        case '3':
            if (!user->isLoggedIn()) {
                std::cout << "You have to login first to find movies.\n";
                break;
            }
            system("cls");
            displayCategoriesMenu();
            break;
        case '4':
            user->eraseUser();
            return;
        default:
            std::cout << "You've entered an invalid option. Please try again." << std::endl;
            break;
        }
    }
}

void UI::registerUI() {
    const char fileName[] = "../assets/graphic/register.txt";
    const char fileToSave[] = "../assets/users.json";

    std::string line;
    std::string password;
    std::string confurmPassword;

    std::ifstream file(fileName);

    if (!file.is_open() || Utiles::isFileEmpty(fileName)) {
        std::cerr << "Could not open file for reading!" << std::endl;
        return;
    }

    while (std::getline(file, line)) {

        if (line.find("Email") != std::string::npos)
        {
            std::cout << line;

            std::string email;
            std::cin >> email;
            while (!user->checkEmail(email, fileToSave)) {
                std::cin >> email;
            }
        }
        else if (line.find("Username") != std::string::npos)
        {
            std::cout << line;

            std::string userName;
            std::cin >> userName;
            user->setUserName(userName);
        }
        else if (line.find("Password") != std::string::npos)
        {
            std::cout << line;

            std::cin >> password;
            while (!user->checkPassword(password)) {
                std::cin >> password;
            }
        }
        else if (line.find("Confirm Password") != std::string::npos)
        {
            std::cout << line;
            std::cin >> confurmPassword;
            while (password != confurmPassword) {
                std::cout << "Wrong password try again";
                std::cin >> confurmPassword;
            }
        }
        else if (line.find("Registering as Admin? ") != std::string::npos)
        {
            std::cout << line;

            char choise;
            std::cin >> choise;
            if (choise == 'y' || choise == 'Y')
            {
                registerAsAdmin();
            }
        }
        else
        {
            std::cout << line << std::endl;
        }

    }
    Utiles::saveToFile(fileToSave, user->saveAsJson());

    file.close();
}

void UI::registerAsAdmin()
{
    const char adminKey[] = "Cinema22";

    std::cout << "     Enter key: ";

    std::string keyToEnter;
    std::cin >> keyToEnter;

    while (keyToEnter != adminKey) {
        std::cout << "     Wrong key: ";
        std::cin >> keyToEnter;
    }

    user->setAdmin();
}

void UI::logInUi() {

    const char fileName[] = "../assets/graphic/login.txt";
    const char fileToSave[] = "../assets/users.json";

    std::string line;
    std::string email;
    std::string password;

    std::ifstream file(fileName);

    if (!file.is_open() || Utiles::isFileEmpty(fileName)) {
        std::cerr << "Could not open file for reading!" << std::endl;
        return;
    }

    while (std::getline(file, line))
    {
        if (line.find("Email") != std::string::npos)
        {
            std::cout << line;

            std::cin >> email;
            while (!user->loadFromFile(fileToSave, email)) {
                std::cin >> email;
            }
        }
        else if (line.find("Password") != std::string::npos)
        {
            std::cout << line;

            std::cin >> password;
            while (user->getPassword() != password) {
                std::cout << "Wrong password\n";
                std::cin >> password;
            }
        }
        else
        {
            std::cout << line << std::endl;
        }
    }
    user->displayUser();

    nlohmann::json notifications;
    std::ifstream notifIn("../assets/notifications.json");
    if (notifIn) notifIn >> notifications;
    notifIn.close();

    std::cout << "\n--- Notifications ---\n";
    for (const auto& notif : notifications) {
        std::cout << notif["message"];
        if (notif.contains("timestamp")) std::cout << " (" << notif["timestamp"] << ")";
        std::cout << "\n";
    }

    std::cout << "\nPress Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    // -------------------------------------------------

    if (user->getIsAdmin()) {
        std::cout << "\nWelcome Admin! Opening admin menu...\n";
        adminMenu();   // Call admin menu here
    }

    file.close();
}

void UI::displayCategoriesMenu() {
    const char* fileName = "../assets/graphic/categories.txt";
    Utiles::displayFile(fileName);

    char choice;
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    system("cls");

    switch (choice) {
    case '1':
        displayHorrorMenu();
        break;
    case '2':
        displayFantasyMenu();
        break;
    case '3':
        displayThrillerMenu();
        break;
    default:
        std::cout << "Invalid choice. Returning to category menu...\n";
        displayCategoriesMenu();
        break;
    }
}

void UI::displayHorrorMenu() {
    nlohmann::json moviesJson;
    std::ifstream inFile("../assets/movies.json");
    if (inFile) {
        inFile >> moviesJson;
    }
    inFile.close();

    std::vector<std::string> horrorMovies;
    std::vector<std::string> descriptions;

    int idx = 1;
    std::cout << "\n=== Horror Movies ===\n";
    for (const auto& movie : moviesJson) {
        if (movie["category"] == "Horror") {
            std::cout << idx << ". " << movie["name"];
            if (movie.contains("description")) {
                std::cout << " - " << movie["description"];
            }
            std::cout << "\n";
            horrorMovies.push_back(movie["name"]);
            descriptions.push_back(movie.value("description", ""));
            ++idx;
        }
    }

    if (horrorMovies.empty()) {
        std::cout << "No horror movies available.\n";
        return;
    }

    int choice;
    std::cout << "\nYour choice (1-" << horrorMovies.size() << "): ";
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (choice < 1 || choice > (int)horrorMovies.size()) {
        std::cout << "Invalid choice. Returning to category menu.\n";
        return;
    }

    std::string movieTitle = horrorMovies[choice - 1];

    nlohmann::json projectionsJson;
    std::ifstream projFile("../assets/projections.json");
    if (projFile) {
        projFile >> projectionsJson;
    }
    projFile.close();

    std::vector<std::string> times;
    for (const auto& proj : projectionsJson) {
        if (proj.contains("movie") && proj["movie"] == movieTitle && proj.contains("time")) {
            std::string timeStr = proj["time"];
            std::istringstream iss(timeStr);
            std::string t;
            while (iss >> t) {
                times.push_back(t);
            }
        }
    }

    if (times.empty()) {
        times = { "16:00", "18:30", "21:00" };
    }

    std::cout << "\nAvailable hours:\n";
    for (size_t i = 0; i < times.size(); ++i) {
        std::cout << i + 1 << ". " << times[i] << '\n';
    }

    int timeChoice;
    std::cout << "Choose hour (1-" << times.size() << "): ";
    std::cin >> timeChoice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (timeChoice < 1 || timeChoice > (int)times.size()) {
        std::cout << "Invalid time choice.\n";
        return;
    }

    std::string selectedTime = times[timeChoice - 1];

    char reserve;
    std::cout << "Do you want to make a reservation? (y/n): ";
    std::cin >> reserve;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (reserve == 'y' || reserve == 'Y') {
        ReservationSystem::makeReservation(movieTitle, selectedTime);
        std::cout << "\nThank you for your reservation. The program will now exit.\n";
        exit(0);
    }
}

void UI::displayFantasyMenu() {
    nlohmann::json moviesJson;
    std::ifstream inFile("../assets/movies.json");
    if (inFile) {
        inFile >> moviesJson;
    }
    inFile.close();

    std::vector<std::string> fantasyMovies;
    std::vector<std::string> descriptions;

    int idx = 1;
    std::cout << "\n=== Fantasy Movies ===\n";
    for (const auto& movie : moviesJson) {
        if (movie["category"] == "Fantasy") {
            std::cout << idx << ". " << movie["name"];
            if (movie.contains("description")) {
                std::cout << " - " << movie["description"];
            }
            std::cout << "\n";
            fantasyMovies.push_back(movie["name"]);
            descriptions.push_back(movie.value("description", ""));
            ++idx;
        }
    }

    if (fantasyMovies.empty()) {
        std::cout << "No fantasy movies available.\n";
        return;
    }

    int choice;
    std::cout << "\nYour choice (1-" << fantasyMovies.size() << "): ";
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (choice < 1 || choice > (int)fantasyMovies.size()) {
        std::cout << "Invalid choice. Returning to category menu.\n";
        return;
    }

    std::string movieTitle = fantasyMovies[choice - 1];

    nlohmann::json projectionsJson;
    std::ifstream projFile("../assets/projections.json");
    if (projFile) {
        projFile >> projectionsJson;
    }
    projFile.close();

    std::vector<std::string> times;
    for (const auto& proj : projectionsJson) {
        if (proj.contains("movie") && proj["movie"] == movieTitle && proj.contains("time")) {
            std::string timeStr = proj["time"];
            std::istringstream iss(timeStr);
            std::string t;
            while (iss >> t) {
                times.push_back(t);
            }
        }
    }

    if (times.empty()) {
        times = { "12:00", "15:30", "18:00", "20:45" };
    }

    std::cout << "\nAvailable hours:\n";
    for (size_t i = 0; i < times.size(); ++i) {
        std::cout << i + 1 << ". " << times[i] << '\n';
    }

    int timeChoice;
    std::cout << "Choose hour (1-" << times.size() << "): ";
    std::cin >> timeChoice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (timeChoice < 1 || timeChoice > (int)times.size()) {
        std::cout << "Invalid time choice.\n";
        return;
    }

    std::string selectedTime = times[timeChoice - 1];

    char reserve;
    std::cout << "Do you want to make a reservation? (y/n): ";
    std::cin >> reserve;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (reserve == 'y' || reserve == 'Y') {
        ReservationSystem::makeReservation(movieTitle, selectedTime);
        std::cout << "\nThank you for your reservation. The program will now exit.\n";
        exit(0);
    }
}

void UI::displayThrillerMenu() {
    nlohmann::json moviesJson;
    std::ifstream inFile("../assets/movies.json");
    if (inFile) {
        inFile >> moviesJson;
    }
    inFile.close();

    std::vector<std::string> thrillerMovies;
    std::vector<std::string> descriptions;

    int idx = 1;
    std::cout << "\n=== Thriller Movies ===\n";
    for (const auto& movie : moviesJson) {
        if (!movie.is_object()) continue;
        if (!movie.contains("category") || !movie.contains("name")) continue;
        if (movie["category"] == "Thriller") {
            std::cout << idx << ". " << movie["name"];
            if (movie.contains("description")) {
                std::cout << " - " << movie["description"];
            }
            std::cout << "\n";
            thrillerMovies.push_back(movie["name"]);
            descriptions.push_back(movie.value("description", ""));
            ++idx;
        }
    }

    if (thrillerMovies.empty()) {
        std::cout << "No thriller movies available.\n";
        return;
    }

    int choice;
    std::cout << "\nYour choice (1-" << thrillerMovies.size() << "): ";
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (choice < 1 || choice > (int)thrillerMovies.size()) {
        std::cout << "Invalid choice. Returning to category menu.\n";
        return;
    }

    std::string movieTitle = thrillerMovies[choice - 1];

    nlohmann::json projectionsJson;
    std::ifstream projFile("../assets/projections.json");
    if (projFile) {
        projFile >> projectionsJson;
    }
    projFile.close();

    std::vector<std::string> times;
    for (const auto& proj : projectionsJson) {
        if (proj.contains("movie") && proj["movie"] == movieTitle && proj.contains("time")) {
            std::string timeStr = proj["time"];
            std::istringstream iss(timeStr);
            std::string t;
            while (iss >> t) {
                times.push_back(t);
            }
        }
    }

    if (times.empty()) {
        times = { "13:30", "16:30", "19:00", "21:45" };
    }

    std::cout << "\nAvailable hours:\n";
    for (size_t i = 0; i < times.size(); ++i) {
        std::cout << i + 1 << ". " << times[i] << '\n';
    }

    int timeChoice;
    std::cout << "Choose hour (1-" << times.size() << "): ";
    std::cin >> timeChoice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (timeChoice < 1 || timeChoice > (int)times.size()) {
        std::cout << "Invalid time choice.\n";
        return;
    }

    std::string selectedTime = times[timeChoice - 1];

    char reserve;
    std::cout << "Do you want to make a reservation? (y/n): ";
    std::cin >> reserve;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (reserve == 'y' || reserve == 'Y') {
        ReservationSystem::makeReservation(movieTitle, selectedTime);
        std::cout << "\nThank you for your reservation. The program will now exit.\n";
        exit(0);
    }
}

void UI::adminMenu()
{
	system("cls");
    std::cout << "Admin Menu: \n";
    std::cout << "1. Add projection\n";
    std::cout << "2. Delete projection\n";
    std::cout << "3. Edit projection\n";
    std::cout << "4. Add film\n";
    std::cout << "5. Delete film\n";
    std::cout << "6. Edit film\n";
    std::cout << "7. Logout\n";

    char choice;
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    AdminSystem admin;

    switch (choice)
    {
    case '1':
        admin.addProjection();
        break;
    case '2':
        admin.deleteProjection();
        break;
    case '3':
        admin.editProjection();
        break;
    case '4':
        admin.addMovie();
        break;
    case '5':
        admin.deleteMovie();
        break;
    case '6':
        admin.editMovie();
        break;
    case '7':
        std::cout << "Logging out...\n";
        return;
    default:
        std::cout << "Invalid choice\n";
        break;
    }

    std::cout << "Press Enter to return to the menu...";
    std::cin.get();

    adminMenu();
}