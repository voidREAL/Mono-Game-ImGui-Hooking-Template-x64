#pragma once

using monoThreadAttach = MonoThread * (*)(MonoDomain*);
using monoGetRootDomain = MonoDomain * (*)();
using monoDomainAssemblyOpen = MonoAssembly * (*)(MonoDomain*, const char*);
using monoAssemblyGetImage = MonoImage * (*)(MonoAssembly*);
using monoClassFromName = MonoClass * (*)(MonoImage*, const char*, const char*);
using monoClassGetMethodFromName = MonoMethod * (*)(MonoClass*, const char*, int);
using monoGetFieldValue = void(*)(MonoObject*, void*, void*);
using monoCompileMethod = void* (*)(MonoMethod*);
using monoFieldStaticGetValue = void(*)(MonoVtable*, MonoClass*, void*);
using monoGetClassVtable = void* (*)(MonoDomain*, MonoClass*);
using monoGetFieldFromName = void* (*)(MonoClass*, const char*);
using monoGetOffset = __int64 (*)(MonoField*);
using monoRuntimeInvoke = __int64 (*)(MonoMethod*, void*, void**, MonoObject**);