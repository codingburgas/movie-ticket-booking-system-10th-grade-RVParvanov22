#include "pch.h"


User::User() {
	this->isAdmin = false;
}

bool User::checkEmail(const std::string& email, const std::string& fileName)
{
	nlohmann::json data;
	if (!Utiles::isFileEmpty(fileName)) {
		data = Utiles::loadFile(fileName);
	}

	for (const auto& item : data) {
		if (item["email"] == email) {
			std::cout << "     ERROR: Email already exists.\n     Enter Email: ";
			return false;
		}
	}

	std::regex email_regex(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");
	if (!std::regex_match(email, email_regex)) {
		std::cout << "     ERROR: Invalid email.\n     Enter Email: ";
		return false;
	}

	std::string domain = email.substr(email.find('@') + 1);
	std::string command = "nslookup -type=MX " + domain + " > nul 2>&1";

	if (std::system(command.c_str()) == 0) {
		this->email = email;
		return true;
	}
	else {
		std::cout << "     ERROR: Invalid email domain.\n     Enter Email: ";
		return false;
	}
}

bool User::checkPassword(const std::string& password)
{
	const char specialCharacters[10] = { '!', '@', '#', '$', '%', '^', '&', '*', '(', ')' };

	if (password.size() < 6)
	{
		std::cout << "     ERROR: Password must be at least 6 characters long.\n     Enter Password: ";
		return false;
	}

	for (size_t i = 0; i < 10; i++)
	{
		if (password.find(specialCharacters[i]) != std::string::npos) {
			this->password = password;
			return true;
		}
	}
	std::cout << "     ERROR: Password must contain at least one special character.\n     Enter Password: ";
	return false;
}


nlohmann::json User::saveAsJson() {
	nlohmann::json data;
	data["email"] = this->email;
	data["password"] = this->password;
	data["userName"] = this->userName;
	data["isAdmin"] = this->isAdmin;
	return data;
}

bool User::loadFromFile(const std::string& fileName, const std::string& emailToFind)
{
	nlohmann::json data;
	if (!Utiles::isFileEmpty(fileName)) {
		data = Utiles::loadFile(fileName);
	}

	if (!data.is_array()) {
		std::cerr << "Invalid user data format" << std::endl;
		return false;
	}

	for (const auto& item : data) {
		if (!item.contains("email") || !item["email"].is_string()) {
			continue;
		}

		std::string storedEmail = item.value("email", "");
		if (storedEmail == emailToFind) 
		{

			this->id = item.value("id", 0);
			this->email = storedEmail;
			this->userName = item.value("userName", "");
			this->password = item.value("password", "");
			this->isAdmin = item.value("isAdmin", false);
			return true;
		}
	}

}

void User::displayUser()
{
	std::cout << "id: " << this->id << "\n";
	std::cout << "Email: " << this->email << "\n";
	std::cout << "Username: " << this->userName << "\n";
	std::cout << "Password: " << this->password << "\n";
	std::cout << "isAdmin: " << this->isAdmin << "\n";
}

void User::eraseUser() {
	this->email = "";
	this->userName = "";
	this->password = "";
	this->isAdmin = false;
}

bool User::isLoggedIn() const {
    return !this->getUserName().empty();
}

