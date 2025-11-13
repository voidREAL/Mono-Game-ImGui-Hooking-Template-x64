#pragma once

#include "MonoAPI.h"

class Class {
public:
	__forceinline static MonoClass* GetClass(const char* _namespace, const char* className, const char* assemblyName = "Assembly-CSharp") {
		return (MonoClass*)Mono::GetInstance().GetClass(_namespace, className, assemblyName);
	}
};