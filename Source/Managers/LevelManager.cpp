/*!
* @file LevelManager.cpp
* @author Rance Andres (andresrancerowell.g@digipen.edu)
* @date 2 March 2026
* @course CSD1451
* @brief Definition file for a level manager that handles levels
*/

#include "LevelManager.hpp"
#include "../UI/ImageUI.hpp"
#include "../Utils/AEOverload.hpp"
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
	static BaseUI* AddTutorialText(GameScene* scene, std::ostream& os, AEVec2 const& pos, std::string text) {
		ImageUI* tut = new ImageUI{ ASSET_HUD_IMAGE, pos };
		tut->layer = BaseEntity::RenderLayer::WORLD;
		tut->scale = { 15.f, 2.5f };
		tut->text_size = 8.f;
		tut->text = text;
		os << pos << ',' << text << '\n';
		scene->AddEntityToScene(tut);
		return tut;
	}

	/*!
	* @brief Load the tutorial texts from file
	* @param level - The level to load
	* 
	*/
	static std::vector<ImageUI*> LoadTutorialTexts(int level) {
		std::vector<ImageUI*> ui;
		std::ostringstream oss;
	
		// Open file
		oss << "Assets/tut_" << level << ".dat";
		std::ifstream is{ oss.str() };
		if (is.fail())
			return ui;

		// Each line is formatted as "{pos.x,pos.y},text to display|2nd line"
		std::string line;
		while (std::getline(is, line)) {
			std::istringstream iss{ line };
			AEVec2 pos; char c; // ','
			std::string word;
			std::string text;
			iss >> pos >> c;
			while (iss >> word) {
				text += word;
				text += " ";
			}

			// Instantiate UI element
			ImageUI* tut = new ImageUI{ ASSET_HUD_IMAGE, pos };
			tut->layer = BaseEntity::RenderLayer::WORLD;
			tut->scale = { 15.f, 2.5f };
			tut->text_size = 8.f;
			tut->text = text;

			ui.push_back(tut);
		}

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

		// Load serialized tutorial texts
		std::vector<ImageUI*> texts = Tutorial::LoadTutorialTexts(global_level);
		if (texts.size() != 0) { // File exists, so load that one
			for (ImageUI* ui : texts) {
				scene->AddEntityToScene(ui);
			}
			return;
		}

		// Load & save hard coded tutorial text
		std::ostringstream oss;
		oss << "Assets/tut_" << global_level << ".dat";
		std::ofstream ofs{ oss.str() };
		if (ofs.fail()) {
			return;
		}

		if (global_level == 0) {
			Tutorial::AddTutorialText(scene, ofs, { -80.f, 16.f }, "Hold left mouse button to charge your fist!|Release to use your attack!");
			Tutorial::AddTutorialText(scene, ofs, { -80.f, 13.f }, "This is your turbo fist!");
			Tutorial::AddTutorialText(scene, ofs, { 5.f, 21.f }, "Charge your first to get past!");
			Tutorial::AddTutorialText(scene, ofs, { 47.f, 8.f }, "This is a breakable crate with loot!");
			Tutorial::AddTutorialText(scene, ofs, { 106.f, 20.f }, "Kill the slimes to get more coins!");
			Tutorial::AddTutorialText(scene, ofs, { 160.f, 22.f }, "Next is the boss room!|Kill the boss to advance!");
		}
		else if (global_level == 1) {
			Tutorial::AddTutorialText(scene, ofs, { 16.f, 16.f }, "Cycle between '1', '2' to switch your fists!");
			Tutorial::AddTutorialText(scene, ofs, { 16.f, 13.f }, "The grapple fist pulls enemies to you!|Try combo-ing your grapple and turbo fist!");
			Tutorial::AddTutorialText(scene, ofs, { 112.f, 10.f }, "Don't fall in the void!");
			Tutorial::AddTutorialText(scene, ofs, { 194.f, 13.f }, "Spiders spawn babies when killed!");
		}
		else if (global_level == 2) {
			Tutorial::AddTutorialText(scene, ofs, { 8.f, 7.f }, "Hotkey '3' is your finger gun!");
			Tutorial::AddTutorialText(scene, ofs, { 51.f, 18.f }, "This is an archer! Ranged enemy!");
		}

		// Save serialized file
		ofs.close();
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