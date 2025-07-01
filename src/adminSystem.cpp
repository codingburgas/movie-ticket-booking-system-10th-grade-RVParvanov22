#include "AdminSystem.h"
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <algorithm>
#include <ctime>

AdminSystem::AdminSystem() {
}

void AdminSystem::adminMenu() {
    char choice;

    do {
        displayAdminOptions();

        std::cout << "Enter your choice: ";
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        handleOption(choice);

    } while (choice != '0');
}

void AdminSystem::displayAdminOptions() {
    std::cout << "\n=== Admin Menu ===\n";
    std::cout << "1. Add Movie\n";
    std::cout << "2. Delete Movie\n";
    std::cout << "3. Edit Movie\n";
    std::cout << "4. Add Projection\n";
    std::cout << "5. Delete Projection\n";
    std::cout << "6. Edit Projection\n";
    std::cout << "0. Exit Admin Menu\n";
}

void AdminSystem::handleOption(char option) {
    switch (option) {
    case '1':
        addMovie();
        break;
    case '2':
        deleteMovie();
        break;
    case '3':
        editMovie();
        break;
    case '4':
        addProjection();
        break;
    case '5':
        deleteProjection();
        break;
    case '6':
        editProjection();
        break;
    case '0':
        std::cout << "Exiting Admin Menu...\n";
        break;
    default:
        std::cout << "Invalid option! Try again.\n";
        break;
    }
}

void AdminSystem::addMovie() {
    std::string movieName, category, description;
    std::cout << "Enter movie name to add: ";
    std::getline(std::cin, movieName);

    std::cout << "Enter category (Horror, Fantasy, Thriller): ";
    std::getline(std::cin, category);

    std::cout << "Enter a short description: ";
    std::getline(std::cin, description);

    nlohmann::json moviesJson;
    std::ifstream inFile("../assets/movies.json");
    if (inFile) {
        inFile >> moviesJson;
    }
    inFile.close();

    for (const auto& movie : moviesJson) {
        if (movie["name"] == movieName) {
            std::cout << "Movie already exists.\n";
            return;
        }
    }

    moviesJson.push_back({
        {"name", movieName},
        {"category", category},
        {"description", description}
    });

    std::ofstream outFile("../assets/movies.json");
    outFile << moviesJson.dump(4);
    outFile.close();

    std::cout << "Movie '" << movieName << "' added successfully.\n";

    nlohmann::json notifications;
    std::ifstream notifIn("../assets/notifications.json");
    if (notifIn) notifIn >> notifications;
    notifIn.close();

    std::time_t now = std::time(nullptr);
    char buf[32];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&now));

    nlohmann::json notif = {
        {"type", "new_movie"},
        {"message", "New movie '" + movieName + "' has been released!"},
        {"timestamp", buf}
    };
    notifications.push_back(notif);

    std::ofstream notifOut("../assets/notifications.json");
    notifOut << notifications.dump(4);
    notifOut.close();
}

void AdminSystem::deleteMovie() {
    std::string movieName;
    std::cout << "Enter movie name to delete: ";
    std::getline(std::cin, movieName);

    nlohmann::json moviesJson;
    std::ifstream inFile("../assets/movies.json");
    if (inFile) {
        inFile >> moviesJson;
    }
    inFile.close();

    bool found = false;
    nlohmann::json newMoviesJson = nlohmann::json::array();
    for (const auto& movie : moviesJson) {
        if (movie["name"] != movieName) {
            newMoviesJson.push_back(movie);
        }
        else {
            found = true;
        }
    }

    if (!found) {
        std::cout << "Movie not found.\n";
        return;
    }

    std::ofstream outFile("../assets/movies.json");
    outFile << newMoviesJson.dump(4);
    outFile.close();

    std::cout << "Movie '" << movieName << "' deleted successfully.\n";
}

void AdminSystem::editMovie() {
    std::string movieName;
    std::cout << "Enter movie name to edit: ";
    std::getline(std::cin, movieName);

    nlohmann::json moviesJson;
    std::ifstream inFile("../assets/movies.json");
    if (inFile) {
        inFile >> moviesJson;
    }
    inFile.close();

    bool found = false;
    for (auto& movie : moviesJson) {
        if (movie["name"] == movieName) {
            std::string newName;
            std::cout << "Enter new movie name: ";
            std::getline(std::cin, newName);
            movie["name"] = newName;
            found = true;
            break;
        }
    }

    if (!found) {
        std::cout << "Movie not found.\n";
        return;
    }

    std::ofstream outFile("../assets/movies.json");
    outFile << moviesJson.dump(4);
    outFile.close();

    std::cout << "Movie '" << movieName << "' edited successfully.\n";
}

void AdminSystem::addProjection() {
    std::string movie, time;
    std::cout << "Enter movie name for projection: ";
    std::getline(std::cin, movie);
    std::cout << "You have entered the movie: " << movie << std::endl;

    std::cout << "Enter projection time (e.g., 20:00): ";
    std::getline(std::cin, time);
    std::cout << "You have set the projection time to: " << time << std::endl;

    nlohmann::json projectionsJson;
    std::ifstream inFile("../assets/projections.json");
    if (inFile) {
        inFile >> projectionsJson;
    }
    inFile.close();

    projectionsJson.push_back({ {"movie", movie}, {"time", time} });

    std::ofstream outFile("../assets/projections.json");
    outFile << projectionsJson.dump(4);
    outFile.close();

    std::cout << "Projection for '" << movie << "' at " << time << " has been successfully added!\n";
}

void AdminSystem::deleteProjection() {
    std::string movie, time;
    std::cout << "Enter movie name for projection to delete: ";
    std::getline(std::cin, movie);
    std::cout << "You have chosen to delete the movie: " << movie << std::endl;

    std::cout << "Enter projection time: ";
    std::getline(std::cin, time);
    std::cout << "You have chosen the time: " << time << std::endl;

    nlohmann::json projectionsJson;
    std::ifstream inFile("../assets/projections.json");
    if (inFile) {
        inFile >> projectionsJson;
    }
    inFile.close();

    bool found = false;
    nlohmann::json newProjectionsJson = nlohmann::json::array();
    for (const auto& proj : projectionsJson) {
        if (proj["movie"] == movie && proj["time"] == time) {
            found = true;
            continue;
        }
        newProjectionsJson.push_back(proj);
    }

    if (!found) {
        std::cout << "No projection found for '" << movie << "' at " << time << ".\n";
        return;
    }

    std::ofstream outFile("../assets/projections.json");
    outFile << newProjectionsJson.dump(4);
    outFile.close();

    std::cout << "Projection for '" << movie << "' at " << time << " has been deleted.\n";
}

void AdminSystem::editProjection() {
    std::string movie, time;
    std::cout << "Enter movie name for projection to edit: ";
    std::getline(std::cin, movie);
    std::cout << "You have chosen to edit the movie: " << movie << std::endl;

    std::cout << "Enter projection time: ";
    std::getline(std::cin, time);
    std::cout << "You have chosen the time: " << time << std::endl;

    nlohmann::json projectionsJson;
    std::ifstream inFile("../assets/projections.json");
    if (inFile) {
        inFile >> projectionsJson;
    }
    inFile.close();

    bool found = false;
    for (auto& proj : projectionsJson) {
        if (proj["movie"] == movie && proj["time"] == time) {
            std::string newMovie, newTime;
            std::cout << "Enter new movie name: ";
            std::getline(std::cin, newMovie);
            std::cout << "You have entered the new movie: " << newMovie << std::endl;

            std::cout << "Enter new projection time: ";
            std::getline(std::cin, newTime);
            std::cout << "You have set the new time to: " << newTime << std::endl;

            proj["movie"] = newMovie;
            proj["time"] = newTime;
            found = true;
            break;
        }
    }

    if (!found) {
        std::cout << "No projection found for '" << movie << "' at " << time << ".\n";
        return;
    }

    std::ofstream outFile("../assets/projections.json");
    outFile << projectionsJson.dump(4);
    outFile.close();

    std::cout << "Projection has been updated successfully.\n";
}