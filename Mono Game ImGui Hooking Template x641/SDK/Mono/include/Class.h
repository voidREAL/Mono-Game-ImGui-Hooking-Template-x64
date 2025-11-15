#pragma once

#include "MonoAPI.h"

class Class {
public:
	__forceinline static MonoClass* GetClass(const char* _namespace, const char* className, const char* assemblyName = "Assembly-CSharp") {
		return (MonoClass*)Mono::GetInstance().GetClass(_namespace, className, assemblyName);
	}

	__forceinline static MonoClass* GetClass(MonoObject* object) {
		return (MonoClass*)Mono::GetInstance().GetClass(object);
	}

	__forceinline static const char* GetNameClass(MonoClass* _class) {
		return Mono::GetInstance().GetNameClass(_class);
	}
};