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

	/*!
	* @brief Get the current level
	* @return The current level
	*/
	int GetLevel() {
		return level;
	}

	/*!
	* @brief Set the current level
	* @brief Call this before setting the next scene
	* @param l - The next level
	*/
	void SetLevel(int l) {
		level = l;
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
		if (level > 2) {
			return;
		}

		if (level == 0) {
			ImageUI* hold = new ImageUI{ ASSET_HUD_IMAGE, {-80.f, 16.f} };
			hold->layer = BaseEntity::RenderLayer::WORLD;
			hold->scale = { 15.f, 2.5f };
			hold->text_size = 8.f;
			hold->text = "Hold left mouse button to charge your fist!\nRelease to use your attack!";
			scene->AddEntityToScene(hold);

			ImageUI* turbo = new ImageUI{ ASSET_HUD_IMAGE, {-80.f, 13.f} };
			turbo->layer = BaseEntity::RenderLayer::WORLD;
			turbo->scale = { 15.f, 2.5f };
			turbo->text_size = 8.f;
			turbo->text = "This is your turbo fist!";
			scene->AddEntityToScene(turbo);

			ImageUI* jump = new ImageUI{ ASSET_HUD_IMAGE, {5.f, 21.f} };
			jump->layer = BaseEntity::RenderLayer::WORLD;
			jump->scale = { 15.f, 2.5f };
			jump->text_size = 8.f;
			jump->text = "Charge your first to get past!";
			scene->AddEntityToScene(jump);

			ImageUI* crate = new ImageUI{ ASSET_HUD_IMAGE, {47.f, 8.f} };
			crate->layer = BaseEntity::RenderLayer::WORLD;
			crate->scale = { 15.f, 2.5f };
			crate->text_size = 8.f;
			crate->text = "This is a breakable crate with loot!";
			scene->AddEntityToScene(crate);

			ImageUI* slime = new ImageUI{ ASSET_HUD_IMAGE, {106.f, 20.f} };
			slime->layer = BaseEntity::RenderLayer::WORLD;
			slime->scale = { 15.f, 2.5f };
			slime->text_size = 8.f;
			slime->text = "Kill the slimes to get more coins!";
			scene->AddEntityToScene(slime);

			ImageUI* boss = new ImageUI{ ASSET_HUD_IMAGE, {160.f, 22.f} };
			boss->layer = BaseEntity::RenderLayer::WORLD;
			boss->scale = { 15.f, 2.f };
			boss->text_size = 8.f;
			boss->text = "Next is the boss room!\nKill the boss to advance!";
			scene->AddEntityToScene(boss);
		}

		if (level == 1) {
			ImageUI* cycle = new ImageUI{ ASSET_HUD_IMAGE, {16.f, 16.f} };
			cycle->layer = BaseEntity::RenderLayer::WORLD;
			cycle->scale = { 15.f, 2.5f };
			cycle->text_size = 8.f;
			cycle->text = "Cycle between '1', '2' to switch your fists!";
			scene->AddEntityToScene(cycle);

			ImageUI* grapple = new ImageUI{ ASSET_HUD_IMAGE, {16.f, 13.f} };
			grapple->layer = BaseEntity::RenderLayer::WORLD;
			grapple->scale = { 15.f, 2.5f };
			grapple->text_size = 8.f;
			grapple->text = "The grapple fist pulls enemies to you!\nTry combo-ing your grapple and turbo fist!";
			scene->AddEntityToScene(grapple);

			ImageUI* fall = new ImageUI{ ASSET_HUD_IMAGE, {112.f, 10.f} };
			fall->layer = BaseEntity::RenderLayer::WORLD;
			fall->scale = { 10.f, 1.5f };
			fall->text_size = 8.f;
			fall->text = "Don't fall in the void!";
			scene->AddEntityToScene(fall);

			ImageUI* spider = new ImageUI{ ASSET_HUD_IMAGE, {194.f, 13.f} };
			spider->layer = BaseEntity::RenderLayer::WORLD;
			spider->scale = { 15.f, 2.5f };
			spider->text_size = 8.f;
			spider->text = "Spiders spawn babies when killed!";
			scene->AddEntityToScene(spider);
		}

		if (level == 2) {
			ImageUI* finger = new ImageUI{ ASSET_HUD_IMAGE, {8.f, 7.f} };
			finger->layer = BaseEntity::RenderLayer::WORLD;
			finger->scale = { 15.f, 2.5f };
			finger->text_size = 8.f;
			finger->text = "Hotkey '3' is your finger gun!";
			scene->AddEntityToScene(finger);

			ImageUI* archer = new ImageUI{ ASSET_HUD_IMAGE, {51.f, 18.f} };
			archer->layer = BaseEntity::RenderLayer::WORLD;
			archer->scale = { 15.f, 2.5f };
			archer->text_size = 8.f;
			archer->text = "This is an archer! Ranged enemy!";
			scene->AddEntityToScene(archer);
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
		unlocked_levels[level] = 0;
	}
}