#pragma once
#include "MonoAPI.h"

class Method {
public:
	__forceinline static MonoMethod* GetMethod(const char* className, const char* methodName, int paramCount, const char* _namespace = "SDG.Unturned", const char* assemblyName = "Assembly-CSharp") {
		return (MonoMethod*)Mono::GetInstance().GetMethod(className, methodName, paramCount, _namespace, assemblyName);
	}

	__forceinline static MonoMethod* GetMethod(MonoClass* _class, const char* methodName, int paramCount, const char* _namespace = "SDG.Unturned", const char* assemblyName = "Assembly-CSharp") {
		return (MonoMethod*)Mono::GetInstance().GetMethod(_class, methodName, paramCount, _namespace, assemblyName);
	}

	__forceinline static __int64 Invoke(MonoMethod* method, MonoObject* instance, void** params = nullptr, MonoObject** exc = nullptr) {
		return Mono::GetInstance().InvokeMethod(method, instance, params, exc);
	}

	__forceinline static MonoMethod* GetNativeMethod(const char* className, const char* methodName, int paramCount, const char* _namespace = "SDG.Unturned", const char* assemblyName = "Assembly-CSharp") {
		return (MonoMethod*)Mono::GetInstance().GetNativeAddress(className, methodName, paramCount, _namespace, assemblyName);
	}

	__forceinline static MonoMethodDesc* NewDesc(const char* name, bool isIncludeNamespace) {
		return Mono::GetInstance().GetMethodDesc(name, isIncludeNamespace);
	}

	__forceinline static MonoMethod* GetMethod(MonoMethodDesc* desc, const char* assemblyName = "Assembly-CSharp") {
		return Mono::GetInstance().GetMethod(desc, assemblyName);
	}

	__forceinline static void FreeDesc(MonoMethodDesc* desc) {
		return Mono::GetInstance().MethodDescFree(desc);
	}
};