#pragma once
#include "pch.h"
#include "user.h"

namespace Utiles
{
	void SetColor(int textColor);
	void resetColor();

	nlohmann::json loadFile(const std::string& fileName);
	void displayFile(const std::string& fileName);

	void saveToFile(const std::string& fileName, const nlohmann::json& data);

	bool isFileEmpty(const std::string& fileName);
};