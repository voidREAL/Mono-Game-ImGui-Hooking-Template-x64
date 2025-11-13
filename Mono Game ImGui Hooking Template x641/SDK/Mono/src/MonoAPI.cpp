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

__int64 Mono::GetOffset(MonoField* field)
{
	return getOffsetFunc(field);
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

	threadAttachFunc(getRootDomainFunc());

	this->initialized = true;
}

void* Mono::GetMethod(const char* className, const char* methodName, int paramCount, const char* _namespace, const char* assemblyName /*= "Assembly-CSharp"*/)
{
	MonoClass* _class = GetClass(_namespace, className);
	if (_class == nullptr) {
		return nullptr;
	}

	MonoMethod* method = classGetMethodFromNameFunc(_class, methodName, paramCount);
	if(method == nullptr) {
		return nullptr;
	}

	return method;
}

void* Mono::GetClass(const char* _namespace, const char* className, const char* assemblyName)
{
	MonoImage* image = GetImage(assemblyName);
	return classFromNameFunc(image, _namespace, className);
}

void* Mono::GetField(MonoClass* _class, const char* fieldName)
{
	return getFieldFromNameFunc(_class, fieldName);
}

void* Mono::GetVtable(MonoClass* _class)
{
	return getClassVtableFunc(GetDomain(), _class);
}

void Mono::GetStaticFieldValue(MonoVtable* vtable, MonoField* staticField, void* out)
{
	return getFieldStaticValueFunc(vtable, staticField, out);
}

void Mono::GetFieldValue(MonoObject* instance, MonoField* field, void* out)
{
	return getFieldValueFunc(instance, field, out);
}

__int64 Mono::InvokeMethod(MonoMethod* method, MonoObject* instance, void** paramsArray, MonoObject** exc /*= nullptr*/)
{
	return runtimeInvokeFunc(method, instance, paramsArray, exc);
}

void* Mono::GetDomain()
{
	return getRootDomainFunc();
}

void* Mono::GetImage(const char* assemblyName)
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
