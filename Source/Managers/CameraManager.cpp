#include "CameraManager.hpp"
#include "AEEngine.h"
#include "../Utils/Utils.hpp"

CameraManager* CameraManager::instance = nullptr;

CameraManager::CameraManager(): x(0.0f), y(0.0f),targetx(0.0f), targety(0.0f), smoothspeed(5.f), shaketimer(0.f), shakeduration(0.f), shakestrength(0.f)
{
}

CameraManager::~CameraManager(){}

CameraManager* CameraManager::GetInstance()
{
    if (instance == nullptr)
    {
        instance = new CameraManager();
    }
    return instance;
}
void CameraManager::Init()
{
    x = targetx = 0.f;
    y = targety = 0.f;

    shaketimer = 0.f;
    shakeduration = 0.f;
    shakestrength = 0.f;
}

void CameraManager::Free()
{
    if (instance != nullptr)
    {
        delete instance;
        instance = nullptr;
    }
}
void CameraManager::SetPosition(float newX, float newY)
{
    x = newX;
    y = newY;
    targetx = newX;
    targety = newY;
}

void CameraManager::Move(float dx, float dy)
{
    x += dx;
    y += dy;
    targetx += dx;
    targety += dy;
}
void CameraManager::SetTarget(float tx, float ty)
{
    targetx = tx;
    targety = ty;
}
void CameraManager::Shake(float duration, float strength)
{
    shakeduration = duration;
    shaketimer = duration;
    shakestrength = strength;
}
void CameraManager::Update(float dt)
{
    // Smooth lerp follow
    x += (targetx - x) * smoothspeed * dt;
    y += (targety - y) * smoothspeed * dt;

    float finalX = x;
    float finalY = y;

    // Shake effect
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

    // Center camera on screen
    auto screenRes = Utils::GetScreenResolution(); 
    finalX -= screenRes.first / 2.f;
    //finalY -= screenRes.second / 2.f;

    // Apply to engine
    AEGfxSetCamPosition(finalX, finalY);
}