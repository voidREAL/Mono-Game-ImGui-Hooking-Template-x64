#pragma once
#define MEMBER(type, name, offset) \
		inline type& name() { return *reinterpret_cast<type*>((uintptr_t)this + (uintptr_t)offset); }

#define MEMBER_PTR(type, name, offset) \
		inline type name() { return *reinterpret_cast<type*>((uintptr_t)this + (uintptr_t)offset); }

#define MEMBER_ARR(type, size, name, offset) \
		inline type* name() { return *reinterpret_cast<type**>((uintptr_t)this + (uintptr_t)offset); }