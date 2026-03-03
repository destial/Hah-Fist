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
    CameraManager();
    ~CameraManager();
    void Init();
    void Update(float dt);

    // Camera movement
    
    //Set Position of the camera to new positon instantly
    //E.g CameraManager::GetInstance()->SetPosition(Utils::WorldToScreen(position.x, position.y).x,0);
    void SetPosition(float newX, float newY);

    //Offset Camera position to the new position instantly but based off its original position
    //E.g CameraManager::GetInstance()->Move(100,0);
    void Move(float dx, float dy);

    //Uses Lerp to slowly move the Camera position to the new location
    //E.g CameraManager::GetInstance()->SetTarget(CameraManager::GetInstance()->GetX() + 500, 0);
    void SetTarget(float tx, float ty);

    //Does a screenshake
    //E.g CameraManager::GetInstance()->Shake(0.1f, 5.f);
    void Shake(float duration, float strength);

    // Get camera position
    float GetX() const { return x; }
    float GetY() const { return y; }
};

#endif