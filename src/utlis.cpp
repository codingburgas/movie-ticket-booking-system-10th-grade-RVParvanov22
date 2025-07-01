#include "pch.h"

namespace Utiles
{
	void SetColor(int textColor)
	{
		std::cout << "\033[" << textColor << "m";
	}

	void resetColor()
	{
		std::cout << "\033[0m";
	}

	nlohmann::json loadFile(const std::string& fileName)
	{
		nlohmann::json dataToSave;
		std::ifstream file(fileName);

		if (file.is_open()) {
			file >> dataToSave;
			file.close();
		}
		else {
			std::cerr << "Could not open file for reading!" << std::endl;
		}

		return dataToSave;
	}

	void displayFile(const std::string& fileName) {
		std::ifstream file(fileName);

		if (file.is_open() || !isFileEmpty(fileName)) {
			std::cout << file.rdbuf() << std::endl;
			file.close();
		}
		else {
			std::cerr << "Could not open file for reading!" << std::endl;
		}
	}

	void saveToFile(const std::string& fileName, const nlohmann::json& data) {
		nlohmann::json existingData;
		nlohmann::json inData = data;

		if (!isFileEmpty(fileName)) {
			existingData = loadFile(fileName);
		}

		if (!existingData.is_array()) {
			existingData = nlohmann::json::array();
		}

		inData["id"] = existingData.size() + 1;
		existingData.push_back(inData);

		std::ofstream outFile(fileName);
		if (outFile.is_open()) {
			outFile << existingData.dump(4);
			outFile.close();
		}
		else {
			std::cerr << "Could not open file for writing!" << std::endl;
		}
	}

	bool isFileEmpty(const std::string& fileName) {
		std::ifstream file(fileName);

		if (!file.is_open()) {
			std::cerr << "Could not open the file!" << std::endl;
			return false;
		}

		file.seekg(0, std::ios::end);
		std::streampos fileSize = file.tellg();

		file.close();
		return fileSize == 0;
	}


}