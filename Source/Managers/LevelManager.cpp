/*!
* @file LevelManager.cpp
* @author Rance Andres (andresrancerowell.g@digipen.edu)
* @date 2 March 2026
* @course CSD1451
* @brief Definition file for a level manager that handles levels
*/

#include "LevelManager.hpp"
#include "../UI/ImageUI.hpp"
#include <fstream>
#include <sstream>
#include <vector>

namespace Tutorial {
	/*!
	* @brief Add a tutorial text to the game scene
	* @param pos - The position of the text in world space
	* @param text - The text to display
	* @return The instance of the BaseUI
	*/
	static BaseUI* AddTutorialText(GameScene* scene, AEVec2 const& pos, std::string text) {
		ImageUI* tut = new ImageUI{ ASSET_HUD_IMAGE, pos };
		tut->layer = BaseEntity::RenderLayer::WORLD;
		tut->scale = { 15.f, 2.5f };
		tut->text_size = 8.f;
		tut->text = text;
		scene->AddEntityToScene(tut);
		return tut;
	}

	/*!
	* @brief Load the tutorial texts from file
	* @param level - The level to load
	* 
	*/
	static std::vector<BaseUI*> LoadTutorialTexts(int level) {
		std::vector<BaseUI*> ui;
		std::ostringstream oss;
		oss << "Assets/tut_" << level << ".dat";
		std::ifstream is{ oss.str() };
		if (is.fail())
			return ui;



		return ui;
	}
}

namespace LevelManager {
	static std::map<int, float> unlocked_levels;
	static int global_level; // the currently played level

	/*!
	* @brief Set a level's fastest time
	* @param level - The level to unlock
	* @param time - The time (in seconds) to complete the level (or -1 to lock it, 0 to unlock it)
	*/
	void SetLevelTime(int level, float time) {
		if (unlocked_levels[level] <= 0 || time < unlocked_levels[level]) {
			unlocked_levels[level] = time;
		}
	}

	/*!
	* @brief Get the current level
	* @return The current level
	*/
	int GetLevel() {
		return global_level;
	}

	/*!
	* @brief Set the current level
	* @brief Call this before setting the next scene
	* @param l - The next level
	*/
	void SetLevel(int l) {
		global_level = l;
	}

	/*!
	* @brief Save the player data (Unlocked levels & score)
	*/
	void SavePlayerData() {
		std::ofstream os{ "user.data" };
		if (os.fail())
			return;

		for (std::pair<int, float> pair : unlocked_levels) {
			os << pair.first << '=' << pair.second << '\n';
		}
		os.close();
	}

	/*!
	* @brief Load the player data (Unlocked levels & score)
	*/
	void LoadPlayerData() {
		std::ifstream is{ "user.data" };
		if (is.fail())
			return;

		int i; float f; char c;
		while (is >> i >> c >> f >> std::noskipws >> c) {
			SetLevelTime(i, f);
		}
		is.close();
	}

	/*!
	* @brief Load the tutorial UI objects based on level
	* @param scene - The game scene pointer
	*/
	void LoadTutorial(GameScene* scene) {
		if (global_level > 2) {
			return;
		}

		std::vector<BaseUI*> texts = Tutorial::LoadTutorialTexts(global_level);
		if (texts.size() != 0) {
			for (BaseUI* ui : texts) {
				scene->AddEntityToScene(ui);
			}
			return;
		}

		// Hard coded tutorial text
		if (global_level == 0) {
			Tutorial::AddTutorialText(scene, { -80.f, 16.f }, "Hold left mouse button to charge your fist!\nRelease to use your attack!");
			Tutorial::AddTutorialText(scene, { -80.f, 13.f }, "This is your turbo fist!");
			Tutorial::AddTutorialText(scene, { 5.f, 21.f }, "Charge your first to get past!");
			Tutorial::AddTutorialText(scene, { 47.f, 8.f }, "This is a breakable crate with loot!");
			Tutorial::AddTutorialText(scene, { 106.f, 20.f }, "Kill the slimes to get more coins!");
			Tutorial::AddTutorialText(scene, { 160.f, 22.f }, "Next is the boss room!\nKill the boss to advance!");
		}

		if (global_level == 1) {
			Tutorial::AddTutorialText(scene, { 16.f, 16.f }, "Cycle between '1', '2' to switch your fists!");
			Tutorial::AddTutorialText(scene, { 16.f, 13.f }, "The grapple fist pulls enemies to you!\nTry combo-ing your grapple and turbo fist!");
			Tutorial::AddTutorialText(scene, { 112.f, 10.f }, "Don't fall in the void!")->scale = { 10.f, 1.5f };
			Tutorial::AddTutorialText(scene, { 194.f, 13.f }, "Spiders spawn babies when killed!");
		}

		if (global_level == 2) {
			Tutorial::AddTutorialText(scene, { 8.f, 7.f }, "Hotkey '3' is your finger gun!");
			Tutorial::AddTutorialText(scene, { 51.f, 18.f }, "This is an archer! Ranged enemy!");
		}
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
		if (unlocked_levels[level] < 0)
			unlocked_levels[level] = 0;
	}
}