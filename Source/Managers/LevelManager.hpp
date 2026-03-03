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

namespace LevelManager { // start namespace
	/*!
	* @brief Get the current level
	* @return The current level
	*/
	int GetLevel();

	/*!
	* @brief Set the current level
	*		 Call this before setting the next scene
	* @param l - The next level
	*/
	void SetLevel(int l);
} // end namespace

#endif