#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H
#include <vector>
#include <SFML/System.hpp>
class SettingsManager
{
public:
	SettingsManager();
	void save() const;

	//Getters
	int getWindowMode() const;
	int getVSync() const;

	//Setters
	void setWindowMode(const int);
	void setVSync(const int);
private:

	void updateFile();
	int windowMode_ = 0;
	int vSync_ = 0;

};
#endif