/*!
* @file LevelManager.hpp
* @author Rance Andres (andresrancerowell.g@digipen.edu)
* @date 2 March 2026
* @course CSD1451
* @brief Declaration file for a level manager that handles levels
*/

#pragma once
#ifndef __LEVEL_MANAGER_H
#define __LEVEL_MANAGER_H
#include <map>

namespace LevelManager { // start namespace

	/*!
	* @brief Get all unlocked levels
	* @return Return a vector of their fastest completion time, in level order
	*/
	std::map<int, float> GetUnlockedLvls();

	/*!
	* @brief Unlock a level
	* @param level - The level to unlock
	*/
	void UnlockLevel(int level);

	/*!
	* @brief Set a level's fastest time
	* @param level - The level to unlock
	* @param time - The time (in seconds) to complete the level
	*/
	void SetLevelTime(int level, float time);

	/*!
	* @brief Get the current level
	* @return The current level
	*/
	int GetLevel();

	/*!
	* @brief Set the current level
	* @brief Call this before setting the next scene
	* @param l - The next level
	*/
	void SetLevel(int l);

	void SavePlayerData();

	void LoadPlayerData();
} // end namespace

#endif