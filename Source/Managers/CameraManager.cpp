/*!
* @file CameraManager.cpp
* @author Ryan Lau (r.lau@digipen.edu)
* @date 28 February 2026
* @course CSD1451
* @brief Declaration of the CameraManager singleton class responsible for
*        handling camera positioning, smooth movement, and visual effects
*        such as screen shake. This class provides functionality to set,
*        move, and interpolate the camera position, ensuring centralized
*        control of camera behavior across the application.
*/
#include "CameraManager.hpp"
#include "AEEngine.h"
#include "../Utils/Utils.hpp"

/*!
* @brief Construct a CameraManager object
*        Initializes camera variables and default state
*/
CameraManager::CameraManager()
: x(0.0f), y(0.0f), targetx(0.0f), targety(0.0f),
  smoothspeed(5.f), shaketimer(0.f), shakeduration(0.f), shakestrength(0.f) {}

/*!
* @brief Destroy the CameraManager object
*        Cleans up any resources used by the camera manager
*/
CameraManager::~CameraManager(){}

/*!
* @brief Initialize the camera manager and its default values
*/
void CameraManager::Init()
{
    x = targetx = 0.f;
    y = targety = 0.f;

    shaketimer = 0.f;
    shakeduration = 0.f;
    shakestrength = 0.f;
}

/*!
* @brief Set the camera position instantly
* @param newX - New X position of the camera
* @param newY - New Y position of the camera
*/
void CameraManager::SetPosition(float newX, float newY)
{
    x = newX;
    y = newY;
    targetx = newX;
    targety = newY;
}

/*!
* @brief Move the camera relative to its current position
* @param dx - Offset in the X direction
* @param dy - Offset in the Y direction
*/
void CameraManager::Move(float dx, float dy)
{
    x += dx;
    y += dy;
    targetx += dx;
    targety += dy;
}

/*!
* @brief Smoothly move the camera towards a target position using interpolation
* @param tx - Target X position
* @param ty - Target Y position
*/
void CameraManager::SetTarget(float tx, float ty)
{
    targetx = tx;
    targety = ty;
}

/*!
* @brief Apply a screen shake effect to the camera
* @param duration - Duration of the shake effect in seconds
* @param strength - Intensity of the shake
*/
void CameraManager::Shake(float duration, float strength)
{
    shakeduration = duration;
    shaketimer = duration;
    shakestrength = strength;
}

/*!
* @brief Update the camera position and apply effects each frame
* @param dt - Delta time between frames
*/
void CameraManager::Update(float dt)
{
    x += (targetx - x) * smoothspeed * dt;
    y += (targety - y) * smoothspeed * dt;

    float finalX = x;
    float finalY = y;

    if (shaketimer > 0.f)
    {
        shaketimer -= dt;
        float progress = shaketimer / shakeduration;
        float strength = shakestrength * progress;

        float offsetX = ((rand() / (float)RAND_MAX) * 2.f - 1.f) * strength;
        float offsetY = ((rand() / (float)RAND_MAX) * 2.f - 1.f) * strength;

        finalX += offsetX;
        finalY += offsetY;
    }

    auto screenRes = Utils::GetScreenResolution(); 
    finalX -= screenRes.first / 2.f;
    //finalY -= screenRes.second / 2.f;

    AEGfxSetCamPosition(finalX, finalY);
}