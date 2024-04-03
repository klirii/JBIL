#pragma once
#include "ClassFileStream.hpp"
#include "ConstantPool.hpp"
#include "FieldInfo.hpp"
#include "MethodInfo.hpp"

class ClassFile {
public:
	ClassFileStream stream;

	ClassFile(const char* path);
	ClassFile(u1* buffer, u4 size);

	u4 magic_dword   = NULL;
	u2 minor_version = NULL;
	u2 major_version = NULL;
	
	ConstantPoolInfo cp_info;

	u2 access_flags = NULL;
	u2 this_class   = NULL;
	u2 super_class  = NULL;

	u2 interfaces_count = NULL;
	std::shared_ptr<u2[]> interfaces;

	u2 fields_count = NULL;
	std::shared_ptr<FieldInfo[]> fields;

	u2 methods_count = NULL;
	std::shared_ptr<MethodInfo[]> methods;

	u2 attributes_count = NULL;
	std::shared_ptr<AttributeInfo[]> attributes;

	MethodInfo* find_method(u2 name_index, u2 descriptor_index);

	std::vector<u1> dump();
	void dump(const char* path);

private:
	std::shared_ptr<u1[]> temp_buffer;
};