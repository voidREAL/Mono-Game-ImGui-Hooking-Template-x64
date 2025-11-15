#pragma once
#include <Windows.h>

#include "Typedef.h"
#include "MonoAPIPrototype.h"

class Mono {
private:
	Mono();
	HMODULE monoModule;
	bool initialized = false;

	monoThreadAttach threadAttachFunc;
	monoGetRootDomain getRootDomainFunc;
	monoDomainAssemblyOpen domainAssemblyOpenFunc;
	monoAssemblyGetImage assemblyGetImageFunc;
	monoClassFromName classFromNameFunc;
	monoClassGetMethodFromName classGetMethodFromNameFunc;
	monoCompileMethod compileMethodFunc;
	monoGetFieldValue getFieldValueFunc;
	monoFieldStaticGetValue getFieldStaticValueFunc;
	monoGetClassVtable getClassVtableFunc;
	monoGetFieldFromName getFieldFromNameFunc;
	monoGetOffset getOffsetFunc;
	monoRuntimeInvoke runtimeInvokeFunc;
	monoArrayLength arrayLengthFunc;
	monoArrayAddressWithSize arrayAddressWithSizeFunc;
	monoArrayElementSize arrayElementSizeFunc;
	monoObjectGetClass objectGetClassFunc;
	monoGetClassName getClassNameFunc;

public:
	static Mono& GetInstance();
	HMODULE GetMonoModule();

	__int64 GetOffset(MonoField* field);
	
	MonoField* GetField(MonoClass* _class, const char* fieldName);
	MonoMethod* GetMethod(const char* className, const char* methodName, int paramCount, const char* _namespace, const char* assemblyName);
	MonoMethod* GetMethod(MonoClass* _class, const char* methodName, int paramCount, const char* _namespace, const char* assemblyName);

	MonoClass* GetClass(const char* _namespace, const char* className, const char* assemblyName);
	MonoClass* GetClass(MonoObject* object);

	void GetStaticFieldValue(MonoVtable* vtable, MonoField* staticField, void* out);
	void GetFieldValue(MonoObject* instance, MonoField* field, void* out);

	__int64 InvokeMethod(MonoMethod* method, MonoObject* instance, void** paramsArray, MonoObject** exc = nullptr);

	MonoVtable* GetVtable(MonoClass* _class);
	MonoDomain* GetDomain();

	MonoAssembly* GetImage(const char* assemblyName);
	const char* GetNameClass(MonoClass* _class);

	int GetArrayLength(MonoArray* arr);
	void* GetArrayElement(MonoArray* arr, int elementSize, int index);

	template<typename T>
	inline T GetArrayElement(MonoArray* arr, int index) {
		if (!arr) return (T)nullptr;
		int elementSize = sizeof(MonoArray*);
		void* ptr = GetArrayElement(arr, elementSize, index);
		return *(T*)&ptr;
	}

	void Initialize();
};