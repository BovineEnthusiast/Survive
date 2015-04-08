#include "SettingsManager.h"
#include <fstream>
#include <iostream>
SettingsManager::SettingsManager()
{
	int length = 0;

	std::fstream file("settings.txt", std::ios_base::in);
	if (file.is_open())
	{
		//Gets the length of the text
		file.seekg(0, std::ios::end);
		length = (int)file.tellg();
		file.seekg(0, std::ios::beg);

		file.close();
	}
	else
		std::cout << "Failed to open \"Settings.txt\"" << std::endl;


	//If it is not 0 (aka exists), load data. Else create one from default data
	if (length != 0)
	{
		std::cout << "Loading contents..." << std::endl;
		std::fstream file("settings.txt", std::ios_base::in);

		if (file.is_open())
		{
			std::vector<std::string> vContents;
			while (file.good())
			{
				std::string tempString;
				std::getline(file, tempString);
				vContents.push_back(tempString);
				std::cout << "Loaded: " << tempString << std::endl;
			}
			windowMode_ = atoi(vContents.at(0).c_str());
			vSync_ = atoi(vContents.at(1).c_str());
			file.close();
		}
		else
			std::cout << "Failed to open \"Settings.txt\"" << std::endl;

		
	}
	else
	{
		std::cout << "Writing contents..." << std::endl;
		std::fstream file("settings.txt", std::ios_base::out);
		if (file.is_open())
		{
			file << std::to_string(windowMode_) << std::endl;
			std::cout << "Wrote windowMode_: " << windowMode_ << std::endl;
			file << std::to_string(vSync_);
			std::cout << "Wrote vSync_: " << windowMode_ << std::endl;
			file.close();
		}
		else
			std::cout << "Failed to open \"Settings.txt\"" << std::endl;

	}
	std::cout << "Done!" << std::endl;

}
void SettingsManager::save() const 
{
	std::cout << "Writing contents..." << std::endl;
	std::fstream file("settings.txt", std::ios_base::out);
	if (file.is_open())
	{
		file << std::to_string(windowMode_) << std::endl;
		std::cout << "Wrote windowMode_: " << windowMode_ << std::endl;
		file << std::to_string(vSync_);
		std::cout << "Wrote vSync_: " << windowMode_ << std::endl;
		file.close();
	}
	else
		std::cout << "Failed to open \"Settings.txt\"" << std::endl;
}

//Getters
int SettingsManager::getWindowMode() const { return windowMode_; }
int SettingsManager::getVSync() const { return vSync_; }
//Setters
void SettingsManager::setWindowMode(const int num) { windowMode_ = num; }
void SettingsManager::setVSync(const int num) { vSync_ = num; }