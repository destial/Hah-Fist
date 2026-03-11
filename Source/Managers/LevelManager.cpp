/*!
* @file LevelManager.cpp
* @author Rance Andres (andresrancerowell.g@digipen.edu)
* @date 2 March 2026
* @course CSD1451
* @brief Definition file for a level manager that handles levels
*/
#include "LevelManager.hpp"
#include <fstream>
#include <cstdio>

namespace LevelManager {
	static std::map<int, float> unlocked_levels;
	static int level; // the currently played level

	/*!
	* @brief Set a level's fastest time
	* @param level - The level to unlock
	* @param time - The time (in seconds) to complete the level
	*/
	void SetLevelTime(int level, float time) {
		unlocked_levels[level] = time;
	}

	int GetLevel() {
		return level;
	}

	void SetLevel(int l) {
		level = l;
	}

	void SavePlayerData() {
		std::ofstream os{ "user.data" };
		if (os.fail())
			return;

		for (std::pair<int, float> pair : unlocked_levels) {
			os << pair.first << '=' << pair.second << '\n';
			std::printf("saved level %d : %0.2f\n", pair.first, pair.second);
		}
		os.close();
	}

	void LoadPlayerData() {
		std::ifstream is{ "user.data" };
		if (is.fail())
			return;

		int i; float f; char c;
		while (is >> i >> c >> f >> std::noskipws >> c) {
			std::printf("loaded level %d : %0.2f\n", i, f);
			SetLevelTime(i, f);
		}
		is.close();
	}

	/*!
	* @brief Get all unlocked levels
	* @return Return a vector of their fastest completion time, in level order
	*/
	std::map<int, float> GetUnlockedLvls() {
		return unlocked_levels;
	}

	/*!
	* @brief Unlock a level
	* @param level - The level to unlock
	*/
	void UnlockLevel(int level) {
		unlocked_levels[level] = 0;
	}
}