/*!
* @file LevelManager.cpp
* @author Rance Andres (andresrancerowell.g@digipen.edu)
* @date 2 March 2026
* @course CSD1451
* @brief Definition file for a level manager that handles levels
*/
#include "LevelManager.hpp"

namespace LevelManager {
	static int level; // the currently played level

	int GetLevel() {
		return level;
	}

	void SetLevel(int l) {
		level = l;
	}
}