#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <iomanip>
#include <sstream>
#include <vector>
#include <algorithm>
#include <string>
#include <random>
#include <ctime>

using json = nlohmann::json;

namespace {
    const std::string reservationFile = "../assets/reservations.json";
    const std::string cardsFile = "../assets/cards.json";

    std::vector<std::string> generateSeats(int rows = 5, int cols = 5) {
        std::vector<std::string> seats;
        for (char row = 'A'; row < 'A' + rows; ++row) {
            for (int col = 1; col <= cols; ++col) {
                seats.push_back(std::string(1, row) + std::to_string(col));
            }
        }
        return seats;
    }

    void displaySeats(const std::vector<std::string>& allSeats, const std::vector<std::string>& taken) {
        for (char row = 'A'; row < 'A' + 5; ++row) {
            for (int col = 1; col <= 5; ++col) {
                std::string seat = std::string(1, row) + std::to_string(col);
                if (std::find(taken.begin(), taken.end(), seat) != taken.end()) {
                    std::cout << "[X] ";
                }
                else {
                    std::cout << "[ ] ";
                }
            }
            std::cout << "\n";
        }
        std::cout << "\n";
    }

    std::string getInput(const std::string& prompt) {
        std::string input;
        std::cout << prompt;
        std::getline(std::cin, input);
        return input;
    }

    json loadJsonFile(const std::string& filename) {
        std::ifstream inFile(filename);
        if (!inFile.is_open()) return json::array();

        std::stringstream buffer;
        buffer << inFile.rdbuf();
        inFile.close();

        if (buffer.str().empty()) return json::array();

        try {
            return json::parse(buffer.str());
        }
        catch (json::parse_error& e) {
            std::cerr << "Error parsing JSON (" << filename << "): " << e.what() << std::endl;
            return json::array();
        }
    }

    void saveJsonFile(const std::string& filename, const json& data) {
        std::ofstream outFile(filename);
        if (!outFile.is_open()) {
            std::cerr << "Error opening file for writing: " << filename << std::endl;
            return;
        }
        outFile << std::setw(4) << data << std::endl;
    }

    bool validateCard(const std::string& number, const std::string& expiry, const std::string& cvv) {
        json cards = loadJsonFile(cardsFile);
        for (const auto& card : cards) {
            if (card.contains("card_number") && card.contains("expiry") && card.contains("cvv")) {
                if (card["card_number"] == number &&
                    card["expiry"] == expiry &&
                    card["cvv"] == cvv) {
                    return true;
                }
            }
        }
        return false;
    }

    void addCardToJson(const std::string& number, const std::string& expiry, const std::string& cvv) {
        json cards = loadJsonFile(cardsFile);

        json newCard = {
            {"card_number", number},
            {"expiry", expiry},
            {"cvv", cvv}
        };

        cards.push_back(newCard);
        saveJsonFile(cardsFile, cards);

        std::cout << "Card successfully registered.\n";
    }

    bool processPayment() {
        std::string number = getInput("\nEnter card number (16 digits, no spaces): ");
        std::string expiry = getInput("Enter expiry date (MM/YY): ");
        std::string cvv = getInput("Enter CVV (3 digits): ");

        if (validateCard(number, expiry, cvv)) {
            std::cout << "\nPayment successful!\n";
            return true;
        }
        else {
            std::string choice = getInput("\nCard not found. Would you like to register this card? (y/n): ");
            if (choice == "y" || choice == "Y") {
                addCardToJson(number, expiry, cvv);
                std::cout << "Payment successful!\n";
                return true;
            }
            else {
                std::cout << "Payment failed. Invalid card details.\n";
                return false;
            }
        }
    }

    std::vector<std::string> parseSeats(const std::string& input) {
        std::vector<std::string> seats;
        std::istringstream iss(input);
        std::string seat;

        while (iss >> seat) {
            if (seat == "done") break;
            seats.push_back(seat);
        }

        return seats;
    }

    bool isValidSeat(const std::string& seat, const std::vector<std::string>& allSeats) {
        return std::find(allSeats.begin(), allSeats.end(), seat) != allSeats.end();
    }

    bool isSeatTaken(const std::string& seat, const std::vector<std::string>& taken) {
        return std::find(taken.begin(), taken.end(), seat) != taken.end();
    }

    std::vector<std::string> generateRandomBookedSeats(const std::vector<std::string>& allSeats, int numSeats = 5) {
        std::vector<std::string> bookedSeats;
        std::vector<std::string> availableSeats = allSeats;

        std::random_device rd;
        std::mt19937 gen(rd());

        numSeats = std::min(numSeats, (int)availableSeats.size());

        for (int i = 0; i < numSeats; ++i) {
            if (availableSeats.empty()) break;

            std::uniform_int_distribution<> dis(0, availableSeats.size() - 1);
            int randomIndex = dis(gen);

            bookedSeats.push_back(availableSeats[randomIndex]);
            availableSeats.erase(availableSeats.begin() + randomIndex);
        }

        return bookedSeats;
    }

    void initializeReservations(const std::string& movieTitle, const std::string& timeSlot) {
        json data = loadJsonFile(reservationFile);

        // Check if this movie/time slot already has reservations
        if (data.contains(movieTitle) && data[movieTitle].contains(timeSlot)) {
            return;
        }

        std::vector<std::string> allSeats = generateSeats();
        std::vector<std::string> randomBookedSeats = generateRandomBookedSeats(allSeats, 3);

        if (!data.contains(movieTitle)) {
            data[movieTitle] = json::object();
        }
        data[movieTitle][timeSlot] = randomBookedSeats;

        saveJsonFile(reservationFile, data);
    }
}

namespace ReservationSystem {
    void makeReservation(const std::string& movieTitle, const std::string& timeSlot) {
        initializeReservations(movieTitle, timeSlot);

        json data = loadJsonFile(reservationFile);

        std::vector<std::string> allSeats = generateSeats();
        std::vector<std::string> taken;

        if (data.contains(movieTitle) && data[movieTitle].contains(timeSlot)) {
            taken = data[movieTitle][timeSlot].get<std::vector<std::string>>();
        }

        std::cout << "\nAvailable seats for " << movieTitle << " at " << timeSlot << ":" << std::endl;
        displaySeats(allSeats, taken);

        std::string input = getInput("Enter seat codes to reserve (e.g., A1 B2 C3), or type 'done' when finished: ");

        std::vector<std::string> requestedSeats = parseSeats(input);
        std::vector<std::string> validSeats;

        for (const std::string& seat : requestedSeats) {
            if (!isValidSeat(seat, allSeats)) {
                std::cout << "Invalid seat: " << seat << " (ignored)\n";
            }
            else if (isSeatTaken(seat, taken)) {
                std::cout << "Seat already taken: " << seat << " (ignored)\n";
            }
            else {
                if (std::find(validSeats.begin(), validSeats.end(), seat) == validSeats.end()) {
                    validSeats.push_back(seat);
                }
            }
        }

        if (validSeats.empty()) {
            std::cout << "\nNo valid seats were selected. Reservation cancelled.\n";
            return;
        }

        std::cout << "\nSelected seats: ";
        for (size_t i = 0; i < validSeats.size(); ++i) {
            std::cout << validSeats[i];
            if (i < validSeats.size() - 1) std::cout << ", ";
        }
        std::cout << "\n";

        std::string confirm = getInput("Confirm reservation? (y/n): ");
        if (confirm != "y" && confirm != "Y") {
            std::cout << "Reservation cancelled.\n";
            return;
        }

        std::cout << "\nProceeding to payment...\n";
        if (!processPayment()) {
            std::cout << "Reservation cancelled due to payment failure.\n";
            return;
        }

        taken.insert(taken.end(), validSeats.begin(), validSeats.end());

        if (!data.contains(movieTitle)) {
            data[movieTitle] = json::object();
        }
        data[movieTitle][timeSlot] = taken;

        saveJsonFile(reservationFile, data);

        std::cout << "\nReservation successful! Your seats: ";
        for (size_t i = 0; i < validSeats.size(); ++i) {
            std::cout << validSeats[i];
            if (i < validSeats.size() - 1) std::cout << ", ";
        }
        std::cout << "\n";

        nlohmann::json notifications;
        std::ifstream notifIn("../assets/notifications.json");
        if (notifIn) notifIn >> notifications;
        notifIn.close();

        std::time_t now = std::time(nullptr);
        char buf[32];
        std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&now));

        nlohmann::json notif = {
            {"type", "booking_made"},
            {"message", "Booking made for '" + movieTitle + "' at " + timeSlot},
            {"timestamp", buf}
        };
        notifications.push_back(notif);

        std::ofstream notifOut("../assets/notifications.json");
        notifOut << notifications.dump(4);
        notifOut.close();
        // -----------------------------------------
    }
}


