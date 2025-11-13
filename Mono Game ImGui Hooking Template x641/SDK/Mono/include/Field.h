#pragma once
#include "MonoAPI.h"

class Field {
public:
	__forceinline static MonoField* Get(MonoClass* _class, const char* fieldName) {
		return (MonoField*)Mono::GetInstance().GetField(_class, fieldName);
	}

	__forceinline static void GetValue(MonoObject* instance, MonoField* field, void* out) {
		Mono::GetInstance().GetFieldValue(instance, field, out);
	}

	__forceinline static void GetStaticValue(MonoVtable* vtable, MonoField* staticField, void* out) {
		Mono::GetInstance().GetStaticFieldValue(vtable, staticField, out);
	}

	__forceinline static __int64 GetOffset(const char* className, const char* fieldName, const char* ns = "SDG.Unturned") {
		MonoClass* _class = Mono::GetInstance().GetClass(ns, className);
		MonoField* field = Field::Get(_class, fieldName);

		return Mono::GetInstance().GetOffset(field);
	}
};