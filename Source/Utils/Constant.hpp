/*!
* @file ExplosiveProjectile.cpp
* @author Ryan Lau (r.lau@digipen.edu)
* @date 13 March 2026
* @course CSD1451
* @brief Central header defining game-wide constants, macros, and default values.
*        Includes gameplay settings, bullet properties, enemy defaults, boss
*        configurations, and lane positions for boss 3. Used throughout the game
*        for consistent values and tuning.
*/

#pragma once
#ifndef __CONSTANT_H
#define __CONSTANT_H

#ifdef _DEBUG
#define TOTAL_LEVELS 4
#endif
#ifndef _DEBUG
#define TOTAL_LEVELS 3
#endif

//Gameplay Macros
#define PLAYER_INVULNERABILITY_ON_HIT 0.75f
#define PLAYER_CONTROL_LOCK_AFTER_HIT 0.5f

//Bullets
#define BULLETSCALEY 0.5f
#define BULLETSCALEX 1.f
#define BULLETMINSPEED 10.f
#define BULLETMAXSPEED 20.f
#define BULLETPLAYERSPEED 30.f
#define BULLETLIFETIME 5.f


//Enemy defaults
#define DEFAULTENEMYMAXHEALTH 50.f
#define DEFAULTENEMYDAMAGE 10.f

//Boss defaults
#define DEFAULTBOSSMAXHEALTH 300.f
#define DEFAULTBOSSDAMAGE 10.f
#define DEFAULTBOSSACTIVATED false

//Boss 1
#define BOSS1ATTACKRANGE 20.f
#define BOSS1BASEPROJECTILES 3
#define BOSS1EXTRAPROJECTILES 10
#define BOSS1JUMPVELX 15.f
#define BOSS1JUMPVELY 50.f
#define BOSS1ROOMPOSY 25.f
#define BOSS1SHOOTCOOLDOWN 3.f
#define BOSS1SPIKELIFETIME 8.f

//Boss 2
#define BOSS2ATTACKRANGE 10.f
#define BOSS2BASEPROJECTILES 2
#define BOSS2EXTRAPROJECTILES 3
#define BOSS2JUMPVELX 15.f
#define BOSS2JUMPVELY 50.f
#define BOSS2ROOMMAXHEIGHT 30.f
#define BOSS2FRICTION 100.f
#define BOSS2SHOOTCOOLDOWN 0.5f


//Boss 3
#define BOSS3VELX 5.f
#define BOSS3VELY 5.f
#define BOSS3OFFSETX 15.f
#define BOSS3PLATFORMSPEED 1.f
#define BOSS3PLATFORMLIFETIME 5.f
#define BOSS3PLATFORMSCALEX 7.f
#define BOSS3PLATFORMSCALEY 0.5f
#define BOSS3LANE1 6.f
#define BOSS3LANE2 14.f
#define BOSS3LANE3 22.f
#define BOSS3TOTALLANES 3

#endif