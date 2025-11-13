#pragma once

#include "../../Mono/include/Class.h"
#include "../../Mono/include/Field.h"
#include "../../Mono/include/Vtable.h"
#include "../../Unity/include/Vector3.h"
#include "../../Utils/Macro.h"

#include "PlayerLife.h"

class Player {
public:
	static Player* GetLocalPlayer();

	MEMBER_PTR(PlayerLife*, playerLife, Field::GetOffset("Player", "_life"));

	bool TeleportToLocation(Vector3 position, float yaw);
};