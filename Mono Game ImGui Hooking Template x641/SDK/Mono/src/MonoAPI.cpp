#include "../include/MonoAPI.h"

Mono::Mono()
{
	monoModule = GetModuleHandle(L"mono-2.0-bdwgc.dll");
}

Mono& Mono::GetInstance()
{
	static Mono instance;
	if (instance.initialized == false) {
		instance.Initialize();
	}
	return instance;
}

HMODULE Mono::GetMonoModule()
{
	return GetModuleHandle(L"mono-2.0-bdwgc.dll");
}

MonoString* Mono::MakeString(const char* _string)
{
	return stringNew(GetAppDomain(), _string);
}

const char* Mono::StringToUTF8(MonoString* _string)
{
	return stringToUtf8(_string);
}

MonoImage* Mono::GetCorlib()
{
	return getCorlib();
}

MonoDomain* Mono::GetAppDomain()
{
	return domainGet();
}

void Mono::ObjectInit(MonoObject* obj)
{
	return runtimeObjectInit(obj);
}

void* Mono::ObjectUnbox(MonoObject* obj)
{
	return objectUnbox(obj);
}

MonoType* Mono::InflateGenericType(MonoType* type1, MonoType* type2)
{
	return inflateGenericType(type1, type2);
}

MonoClass* Mono::GetObjectClass()
{
	return getObjectClass();
}

MonoObject* Mono::CreateObject(MonoClass* _class)
{
	return newObject(GetAppDomain(), _class);
}

MonoMethodDesc* Mono::GetMethodDesc(const char* name, bool isIncludeNamespace)
{
	return methodDescNew(name, isIncludeNamespace);
}

MonoType* Mono::GetType(MonoClass* _class)
{
	return getClassTypeFunc(_class);
}

MonoReflectionType* Mono::_GetObject(MonoType* type)
{
	return getObjectType(GetAppDomain(), type);
}

void Mono::AttachThreadRootDomain()
{
	threadAttachFunc(getRootDomainFunc());
}

void Mono::AttachThreadAppDomain()
{
	threadAttachFunc(domainGet());
}

MonoMethod* Mono::GetNativeAddress(const char* className, const char* methodName, int paramCount, const char* _namespace, const char* assemblyName)
{
	MonoMethod* method = GetMethod(className, methodName, paramCount, _namespace, assemblyName);

	MonoMethod* nativeMethodAddress = compileMethodFunc(method);

	return nativeMethodAddress;
}

__int64 Mono::GetOffset(MonoField* field)
{
	return getOffsetFunc(field);
}

void* Mono::GetArrayElement(MonoArray* arr, int elementSize, int index)
{
	if (!arr) return nullptr;
	return *(void**)arrayAddressWithSizeFunc(arr, elementSize, index);
}

void Mono::MethodDescFree(MonoMethodDesc* desc)
{
	return methodDescFree(desc);
}

void Mono::Initialize()
{
	monoModule = GetMonoModule();

	if (monoModule == NULL) {
		return;
	}

	threadAttachFunc = (monoThreadAttach)(GetProcAddress(monoModule, "mono_thread_attach"));
	getRootDomainFunc = (monoGetRootDomain)(GetProcAddress(monoModule, "mono_get_root_domain"));
	domainAssemblyOpenFunc = (monoDomainAssemblyOpen)(GetProcAddress(monoModule, "mono_domain_assembly_open"));
	assemblyGetImageFunc = (monoAssemblyGetImage)(GetProcAddress(monoModule, "mono_assembly_get_image"));
	classFromNameFunc = (monoClassFromName)(GetProcAddress(monoModule, "mono_class_from_name"));
	classGetMethodFromNameFunc = (monoClassGetMethodFromName)(GetProcAddress(monoModule, "mono_class_get_method_from_name"));
	compileMethodFunc = (monoCompileMethod)(GetProcAddress(monoModule, "mono_compile_method"));
	getFieldValueFunc = (monoGetFieldValue)(GetProcAddress(monoModule, "mono_field_get_value"));
	getFieldStaticValueFunc = (monoFieldStaticGetValue)(GetProcAddress(monoModule, "mono_field_static_get_value"));
	getClassVtableFunc = (monoGetClassVtable)(GetProcAddress(monoModule, "mono_class_vtable"));
	getFieldFromNameFunc = (monoGetFieldFromName)(GetProcAddress(monoModule, "mono_class_get_field_from_name"));
	getOffsetFunc = (monoGetOffset)(GetProcAddress(monoModule, "mono_field_get_offset"));
	runtimeInvokeFunc = (monoRuntimeInvoke)(GetProcAddress(monoModule, "mono_runtime_invoke"));
	arrayLengthFunc = (monoArrayLength)(GetProcAddress(monoModule, "mono_array_length"));
	arrayAddressWithSizeFunc = (monoArrayAddressWithSize)(GetProcAddress(monoModule, "mono_array_addr_with_size"));
	arrayElementSizeFunc = (monoArrayElementSize)(GetProcAddress(monoModule, "mono_array_element_size"));
	objectGetClassFunc = (monoObjectGetClass)(GetProcAddress(monoModule, "mono_object_get_class"));
	getClassNameFunc = (monoGetClassName)(GetProcAddress(monoModule, "mono_class_get_name"));
	getClassTypeFunc = (monoClassGetType)(GetProcAddress(monoModule, "mono_class_get_type"));
	getObjectType = (monoTypeGetObject)(GetProcAddress(monoModule, "mono_type_get_object"));
	newObject = (monoNewObject)(GetProcAddress(monoModule, "mono_object_new"));
	methodDescNew = (monoMethodDescNew)(GetProcAddress(monoModule, "mono_method_desc_new"));
	methodDescSearchImage = (monoMethodDescSearchInImage)(GetProcAddress(monoModule, "mono_method_desc_search_in_image"));
	methodDescFree = (monoMethodDescFree)(GetProcAddress(monoModule, "mono_method_desc_free"));
	inflateGenericType = (monoClassInflateGenericType)(GetProcAddress(monoModule, "mono_class_inflate_generic_type"));
	classFromType = (monoClassFromMonoType)(GetProcAddress(monoModule, "mono_class_from_mono_type"));
	arrayNew = (monoArrayNew)(GetProcAddress(monoModule, "mono_array_new"));
	getObjectClass = (monoGetObjectClass)(GetProcAddress(monoModule, "mono_get_object_class"));
	domainGet = (monoDomainGet)(GetProcAddress(monoModule, "mono_domain_get"));
	runtimeObjectInit = (monoRuntimeObjectInit)(GetProcAddress(monoModule, "mono_runtime_object_init"));
	getCorlib = (monoGetCorlib)(GetProcAddress(monoModule, "mono_get_corlib"));
	typeGetName = (monoTypeGetName)(GetProcAddress(monoModule, "mono_type_get_name"));
	stringNew = (monoStringNew)(GetProcAddress(monoModule, "mono_string_new"));
	stringToUtf8 = (monoStringToUtf8)(GetProcAddress(monoModule, "mono_string_to_utf8"));
	objectUnbox = (monoObjectUnbox)(GetProcAddress(monoModule, "mono_object_unbox"));

	threadAttachFunc(getRootDomainFunc());

	this->initialized = true;
}

MonoMethod* Mono::GetMethod(const char* className, const char* methodName, int paramCount, const char* _namespace, const char* assemblyName /*= "Assembly-CSharp"*/)
{
	MonoClass* _class = GetClass(_namespace, className, assemblyName);
	if (_class == nullptr) {
		return nullptr;
	}

	MonoMethod* method = classGetMethodFromNameFunc(_class, methodName, paramCount);
	if(method == nullptr) {
		return nullptr;
	}

	return method;
}

MonoMethod* Mono::GetMethod(MonoClass* _class, const char* methodName, int paramCount, const char* _namespace, const char* assemblyName)
{
	MonoMethod* method = classGetMethodFromNameFunc(_class, methodName, paramCount);
	if (method == nullptr) {
		return nullptr;
	}

	return method;
}

MonoMethod* Mono::GetMethod(MonoMethodDesc* desc, const char* assemblyName)
{
	return methodDescSearchImage(desc, GetImage(assemblyName));
}

MonoClass* Mono::GetClass(const char* _namespace, const char* className, const char* assemblyName)
{
	MonoImage* image = GetImage(assemblyName);
	return classFromNameFunc(image, _namespace, className);
}

MonoClass* Mono::GetClass(MonoObject* object)
{
	return objectGetClassFunc(object);
}

MonoClass* Mono::GetClass(MonoType* type, bool isType)
{
	return classFromType(type);
}

MonoClass* Mono::GetClass(MonoImage* image, const char* _namespace, const char* className)
{
	return classFromNameFunc(image, _namespace, className);
}

MonoField* Mono::GetField(MonoClass* _class, const char* fieldName)
{
	return getFieldFromNameFunc(_class, fieldName);
}

MonoVtable* Mono::GetVtable(MonoClass* _class)
{
	return getClassVtableFunc(GetDomain(), _class);
}

void Mono::GetStaticFieldValue(MonoVtable* vtable, MonoField* staticField, void* out)
{
	return getFieldStaticValueFunc(vtable, staticField, out);
}

void Mono::GetFieldValue(MonoObject* instance, MonoField* field, void* out)
{
	if (!instance) {
		return;
	}

	return getFieldValueFunc(instance, field, out);
}

__int64 Mono::InvokeMethod(MonoMethod* method, MonoObject* instance, void** paramsArray, MonoObject** exc /*= nullptr*/)
{
	return runtimeInvokeFunc(method, instance, paramsArray, exc);
}

MonoDomain* Mono::GetDomain()
{
	return getRootDomainFunc();
}

MonoImage* Mono::GetImage(const char* assemblyName)
{
	using _assemblyForEach = void* (*)(void(*)(void*, void*), void*);
	_assemblyForEach assemblyForEachFunc = (_assemblyForEach)(GetProcAddress(monoModule, "mono_assembly_foreach"));

	if (!assemblyForEachFunc) {
		return nullptr;
	}
	
	struct Context {
		const char* searchName;
		void* result = nullptr;
	};

	Context ctx;
	ctx.searchName = assemblyName;

	auto callback = [](void* assembly, void* userData) {
		Context* ctx = (Context*)userData;

		HMODULE monoModule = (HMODULE)GetModuleHandle(L"mono-2.0-bdwgc.dll");

		if(!monoModule) {
			return;
		}

		using _assemblyGetImageTemp = void* (*)(void*);
		using _assemblyGetNameTemp = const char* (*)(void*);

		_assemblyGetImageTemp assemblyGetImageFunc = (_assemblyGetImageTemp)(GetProcAddress(monoModule, "mono_assembly_get_image"));
		_assemblyGetNameTemp assemblyGetNameFunc = (_assemblyGetNameTemp)(GetProcAddress(monoModule, "mono_image_get_name"));

		void* image = assemblyGetImageFunc(assembly);
		if (!image) {
			return;
		}

		const char* name = assemblyGetNameFunc(image);

		if(name && strcmp(name, ctx->searchName) == 0) {
			ctx->result = image;
		}
	};

	assemblyForEachFunc(callback, &ctx);	

	return ctx.result;
}

const char* Mono::GetNameClass(MonoClass* _class)
{
	return getClassNameFunc(_class);
}

const char* Mono::GetNameType(MonoType* type)
{
	return typeGetName(type);
}

MonoArray* Mono::CreateNewArray(MonoClass* elementType, uintptr_t size)
{
	return arrayNew(GetDomain(), elementType, size);
}

int Mono::GetArrayLength(MonoArray* arr)
{
	if (!arr) return 0;
	return arrayLengthFunc(arr);
}
