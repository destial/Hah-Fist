/*!
* @file CameraManager.hpp
* @author Ryan Lau (r.lau@digipen.edu)
* @date 28 February 2026
* @course CSD1451
* @brief Declaration of the CameraManager singleton class responsible for
*        handling camera positioning, smooth movement, and visual effects
*        such as screen shake. This class provides functionality to set,
*        move, and interpolate the camera position, ensuring centralized
*        control of camera behavior across the application.
*/

#pragma once
#ifndef __CAMERAMANAGER_H
#define __CAMERAMANAGER_H
#include <iostream>
#include "../Utils/Singleton.hpp"

/*
* @brief Singleton class that handles Camera Movement and Effects
*/
class CameraManager : public Singleton<CameraManager> {
private: 
    float x, y, targetx, targety, smoothspeed, shaketimer, shakeduration, shakestrength;
public:
    /*!
    * @brief Construct a CameraManager object
    *        Initializes camera variables and default state
    */
    CameraManager();

    /*!
    * @brief Destroy the CameraManager object
    *        Cleans up any resources used by the camera manager
    */
    ~CameraManager();

    /*!
    * @brief Initialize the camera manager and its default values
    */
    void Init();

    /*!
    * @brief Update the camera position and apply effects each frame
    * @param dt - Delta time between frames
    */
    void Update(float dt);

    /*!
    * @brief Set the camera position instantly
    * @param newX - New X position of the camera
    * @param newY - New Y position of the camera
    */
    void SetPosition(float newX, float newY);

    /*!
    * @brief Move the camera relative to its current position
    * @param dx - Offset in the X direction
    * @param dy - Offset in the Y direction
    */
    void Move(float dx, float dy);

    /*!
    * @brief Smoothly move the camera towards a target position using interpolation
    * @param tx - Target X position
    * @param ty - Target Y position
    */
    void SetTarget(float tx, float ty);

    /*!
    * @brief Apply a screen shake effect to the camera
    * @param duration - Duration of the shake effect in seconds
    * @param strength - Intensity of the shake
    */
    void Shake(float duration, float strength);

    /*!
    * @brief Get the current X position of the camera
    * @return Current X coordinate
    */
    float GetX() const { return x; };

    /*!
    * @brief Get the current Y position of the camera
    * @return Current Y coordinate
    */
    float GetY() const { return y; };
};

#endif