#include "../include/onrender.h"
#include "../../SDK/SDK.h"

#include <Windows.h>
#include <iostream>

OnRender onRender;

void OnRender::onPresent()
{
	Player* localPlayer = Player::GetLocalPlayer();
	PlayerLife* lifeObject = localPlayer->playerLife();

	PlayerLife* playerLife = (PlayerLife*)lifeObject;
	playerLife->stamina() = 100;

	if (GetAsyncKeyState(VK_F4)) {
		localPlayer->TeleportToLocation({ 500,500,500 }, 0.5);
	}
}
