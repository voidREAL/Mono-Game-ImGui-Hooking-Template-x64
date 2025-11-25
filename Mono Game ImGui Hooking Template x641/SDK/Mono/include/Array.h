#pragma once
#include "MonoAPI.h"

class Array {
public:
	__forceinline static int GetArrayLength(MonoArray* arr) {
		return Mono::GetInstance().GetArrayLength(arr);
	}

	__forceinline static void* GetArrayElement(MonoArray* arr, int elementSize, int index) {
		return Mono::GetInstance().GetArrayElement(arr, elementSize, index);
	}

	template<typename T>
	__forceinline static T GetArrayElement(T* arr, int index) {
		return Mono::GetInstance().GetArrayElement<T>((MonoArray*)arr, index);
	}

	__forceinline static MonoArray* CreateArray(MonoClass* elementType, uintptr_t size) {
		return Mono::GetInstance().CreateNewArray(elementType, size);
	}
};