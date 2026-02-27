#pragma once
#ifndef __CAMERAMANAGER_H
#define __CAMERAMANAGER_H
#include <iostream>

/*
* @brief Singleton class that handles Camera Movement and Effects
*/
class CameraManager
{
private:
    // Singleton: private constructor
    CameraManager();

    static CameraManager* instance;
    // Camera properties
    float x, y, targetx, targety, smoothspeed, shaketimer, shakeduration, shakestrength;

public:
    ~CameraManager();
    static CameraManager* GetInstance();
    static void Free();
    // Initialize per scene
    void Init();
    void Update(float dt);

    // Camera movement
    void SetPosition(float newX, float newY);
    void Move(float dx, float dy);

    void SetTarget(float tx, float ty);
    void Shake(float duration, float strength);

    // Get camera position
    float GetX() const { return x; }
    float GetY() const { return y; }
};

#endif