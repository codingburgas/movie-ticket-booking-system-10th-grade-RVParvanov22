#pragma once
#include <nlohmann/json.hpp>
#include <string>

class User
{
public:
	User();

	std::string getEmail() { return this->email; }
	std::string getPassword() { return this->password; }
	std::string getUserName() const { return this->userName; }
	std::size_t getId() { return this->id; }

	bool getIsAdmin() { return this->isAdmin; }
	void setAdmin() { this->isAdmin = true; }
	void setUserName(std::string name) { this->userName = name; }

	void setEmail(const std::string& email) { this->email = email; }
	void setPassword(const std::string& password) { this->password = password; }

	bool checkEmail(const std::string& email, const std::string& fileName);
	bool checkPassword(const std::string& password);
	bool isUserEmpty() { return this->email.size() == 0; }
	nlohmann::json saveAsJson();
	bool loadFromFile(const std::string& fileName, const std::string& emailToFind);
	void displayUser();
	void eraseUser();
	bool isLoggedIn() const;

private:
	size_t id;
	bool isAdmin;
	std::string email;
	std::string password;
	std::string userName;
};
