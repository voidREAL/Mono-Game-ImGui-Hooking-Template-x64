#pragma once
#include "../../Mono/include/Field.h"
#include "../../Mono/include/Method.h"
#include "../../Utils/Include.h"
#include <cstdint>

class PlayerLife {
public:
	MEMBER(uint8_t, health, Field::GetOffset("PlayerLife", "_health"));
	MEMBER(uint8_t, food, Field::GetOffset("PlayerLife", "_food"));
	MEMBER(uint8_t, stamina, Field::GetOffset("PlayerLife", "_stamina"));

	void sendSuicide();
};