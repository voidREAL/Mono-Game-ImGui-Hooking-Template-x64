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

public:
	static Mono& GetInstance();
	HMODULE GetMonoModule();

	__int64 GetOffset(MonoField* field);
	
	void* GetField(MonoClass* _class, const char* fieldName);
	void* GetMethod(const char* className, const char* methodName, int paramCount, const char* _namespace, const char* assemblyName = "Assembly-CSharp");

	void* GetClass(const char* _namespace, const char* className, const char* assemblyName = "Assembly-CSharp");

	void GetStaticFieldValue(MonoVtable* vtable, MonoField* staticField, void* out);
	void GetFieldValue(MonoObject* instance, MonoField* field, void* out);

	__int64 InvokeMethod(MonoMethod* method, MonoObject* instance, void** paramsArray, MonoObject** exc = nullptr);

	void* GetVtable(MonoClass* _class);
	void* GetDomain();

	void* GetImage(const char* assemblyName = "Assembly-CSharp");
	void Initialize();
};