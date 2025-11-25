#pragma once
#include "MonoAPI.h"

class Type {
public:
	__forceinline static MonoType* Get(MonoClass* _class) {
		return Mono::GetInstance().GetType(_class);
	}

	__forceinline static MonoType* Inflate(MonoType* type1, MonoType* type2) {
		return Mono::GetInstance().InflateGenericType(type1, type2);
	}

	__forceinline static const char* GetName(MonoType* type) {
		return Mono::GetInstance().GetNameType(type);
	}
};