#pragma once
#include "MonoAPI.h"

class Object {
public:
	__forceinline static MonoReflectionType* Get(MonoType* type) {
		return Mono::GetInstance()._GetObject(type);
	}

	__forceinline static MonoObject* Create(MonoClass* _class) {
		return Mono::GetInstance().CreateObject(_class);
	}

	__forceinline static MonoClass* GetObjectClass() {
		return Mono::GetInstance().GetObjectClass();
	}

	__forceinline static void Init(MonoObject* obj) {
		return Mono::GetInstance().ObjectInit(obj);
	}

	__forceinline static void* Unbox(MonoObject* obj) {
		return Mono::GetInstance().ObjectUnbox(obj);
	}
};