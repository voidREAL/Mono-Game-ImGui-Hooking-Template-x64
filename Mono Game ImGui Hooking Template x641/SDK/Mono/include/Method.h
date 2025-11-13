#pragma once
#include "MonoAPI.h"

class Method {
public:
	__forceinline static MonoMethod* GetMethod(const char* className, const char* methodName, int paramCount, const char* _namespace = "SDG.Unturned", const char* assemblyName = "Assembly-CSharp") {
		return (MonoMethod*)Mono::GetInstance().GetMethod(className, methodName, paramCount, _namespace, assemblyName);
	}

	__forceinline static __int64 Invoke(MonoMethod* method, MonoObject* instance, void** params = nullptr, MonoObject** exc = nullptr) {
		return Mono::GetInstance().InvokeMethod(method, instance, params, exc);
	}
};