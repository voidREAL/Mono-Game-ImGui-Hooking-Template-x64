#include "../include/Player.h"

Player* Player::GetLocalPlayer()
{
	MonoClass* playerClass = Class::GetClass("SDG.Unturned", "Player");
	MonoField* localPlayerField = Field::Get(playerClass, "_localPlayer");

	MonoVtable* vtable = Vtable::Get(playerClass);
	Player* playerObject = nullptr;
	Field::GetStaticValue(vtable, localPlayerField, &playerObject);
	return playerObject;
}

bool Player::TeleportToLocation(Vector3 position, float yaw)
{
	MonoMethod* method = Method::GetMethod("Player", "teleportToLocation", 2);

	if (!method) {
		return false;
	}

	void* args[2] = { &position, &yaw };

	return Method::Invoke(method, this, args);
}

