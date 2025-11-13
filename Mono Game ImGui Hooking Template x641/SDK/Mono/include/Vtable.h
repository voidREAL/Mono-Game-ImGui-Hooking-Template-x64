#pragma once
#include "MonoAPI.h"

class Vtable {
public:
	__forceinline static MonoVtable* Get(MonoClass* _class) {
		return (MonoVtable*)Mono::GetInstance().GetVtable(_class);
	}
};