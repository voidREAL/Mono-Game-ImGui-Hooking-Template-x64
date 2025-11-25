#pragma once
#include <Windows.h>

#include "Typedef.h"
#include "Prototype.h"

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
	monoClassGetType getClassTypeFunc;
	monoTypeGetObject getObjectType;
	monoNewObject newObject;
	monoMethodDescNew methodDescNew;
	monoMethodDescSearchInImage methodDescSearchImage;
	monoMethodDescFree methodDescFree;
	monoClassInflateGenericType inflateGenericType;
	monoClassFromMonoType classFromType;
	monoArrayNew arrayNew;
	monoGetObjectClass getObjectClass;
	monoDomainGet domainGet;
	monoRuntimeObjectInit runtimeObjectInit;
	monoGetCorlib getCorlib;
	monoTypeGetName typeGetName;
	monoStringNew stringNew;
	monoStringToUtf8 stringToUtf8;
	monoObjectUnbox objectUnbox;

public:
	static Mono& GetInstance();
	MonoImage* GetCorlib();

	HMODULE GetMonoModule();

	MonoString* MakeString(const char* _string);
	const char* StringToUTF8(MonoString* _string);

	MonoDomain* GetAppDomain();
	void ObjectInit(MonoObject* obj);
	void* ObjectUnbox(MonoObject* obj);

	MonoType* InflateGenericType(MonoType* type1, MonoType* type2);
	MonoClass* GetObjectClass();

	MonoObject* CreateObject(MonoClass* _class);
	MonoMethodDesc* GetMethodDesc(const char* name, bool isIncludeNamespace);

	MonoType* GetType(MonoClass* _class);
	MonoReflectionType* _GetObject(MonoType* type);

	void AttachThreadRootDomain();
	void AttachThreadAppDomain();

	MonoMethod* GetNativeAddress(const char* className, const char* methodName, int paramCount, const char* _namespace, const char* assemblyName);

	__int64 GetOffset(MonoField* field);
	MonoField* GetField(MonoClass* _class, const char* fieldName);

	MonoMethod* GetMethod(MonoMethodDesc* desc, const char* assemblyName);
	MonoMethod* GetMethod(const char* className, const char* methodName, int paramCount, const char* _namespace, const char* assemblyName);
	MonoMethod* GetMethod(MonoClass* _class, const char* methodName, int paramCount, const char* _namespace, const char* assemblyName);

	MonoClass* GetClass(MonoImage* image, const char* _namespace, const char* className);
	MonoClass* GetClass(MonoType* type, bool isType);
	MonoClass* GetClass(const char* _namespace, const char* className, const char* assemblyName);
	MonoClass* GetClass(MonoObject* object);

	void GetStaticFieldValue(MonoVtable* vtable, MonoField* staticField, void* out);
	void GetFieldValue(MonoObject* instance, MonoField* field, void* out);

	__int64 InvokeMethod(MonoMethod* method, MonoObject* instance, void** paramsArray, MonoObject** exc = nullptr);

	MonoVtable* GetVtable(MonoClass* _class);
	MonoDomain* GetDomain();

	MonoImage* GetImage(const char* assemblyName);
	const char* GetNameClass(MonoClass* _class);
	const char* GetNameType(MonoType* type);

	MonoArray* CreateNewArray(MonoClass* elementType, uintptr_t size);
	int GetArrayLength(MonoArray* arr);
	void* GetArrayElement(MonoArray* arr, int elementSize, int index);

	template<typename T>
	inline T GetArrayElement(MonoArray* arr, int index) {
		if (!arr) return (T)nullptr;
		int elementSize = sizeof(MonoArray*);
		void* ptr = GetArrayElement(arr, elementSize, index);
		return *(T*)&ptr;
	}
	void MethodDescFree(MonoMethodDesc* desc);

	void Initialize();
};