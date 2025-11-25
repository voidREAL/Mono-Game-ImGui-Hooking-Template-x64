#pragma once
#include "MonoAPI.h"

class String {
public:
	__forceinline static MonoString* Make(const char* _string) {
		return Mono::GetInstance().MakeString(_string);
	}

	__forceinline static const char* ConvertToUTF8(MonoString* _string) {
		return Mono::GetInstance().StringToUTF8(_string);
	}
};