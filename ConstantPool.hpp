#pragma once
#include "Types.hpp"

#include <vector>
#include <memory>
#include <string>

enum {
	CONSTANT_Utf8    = 1,
	CONSTANT_Integer = 3,
	CONSTANT_Float,
	CONSTANT_Long,
	CONSTANT_Double,
	CONSTANT_Class,
	CONSTANT_String,
	CONSTANT_Fieldref,
	CONSTANT_Methodref,
	CONSTANT_InterfaceMethodref,
	CONSTANT_NameAndType,
	CONSTANT_MethodHandle = 15,
	CONSTANT_MethodType,
	CONSTANT_Dynamic,
	CONSTANT_InvokeDynamic,
	CONSTANT_Module,
	CONSTANT_Package
};

#pragma pack(push, 2) // Set the alignment of the following classes to 2 bytes

struct NameIndexEntryInfo {
	u2 name_index = NULL;
};
typedef NameIndexEntryInfo ClassEntryInfo, ModuleEntryInfo, PackageEntryInfo;

struct RefEntryInfo {
	u2 class_index         = NULL;
	u2 name_and_type_index = NULL;
};
typedef RefEntryInfo FieldrefEntryInfo, MethodrefEntryInfo, InterfaceMethodrefEntryInfo;

struct StringEntryInfo {
	u2 string_index = NULL;
};

template<class T>
struct ValueEntryInfo {
	T value = NULL;
};

typedef ValueEntryInfo<int> IntegerEntryInfo;
typedef ValueEntryInfo<float> FloatEntryInfo;
typedef ValueEntryInfo<long long> LongEntryInfo;
typedef ValueEntryInfo<double> DoubleEntryInfo;

struct NameAndTypeEntryInfo {
	u2 name_index       = NULL;
	u2 descriptor_index = NULL;
};

#pragma pack(pop) // Restore default class alignment

#pragma pack(push, 1) // Set class alignment to 1 byte
struct Utf8EntryInfo {
	u2 length = NULL;
	char str[1];

	std::string as_string() { return std::string(str, length); }
};

struct MethodHandleEntryInfo {
	u1 reference_kind  = NULL;
	u2 reference_index = NULL;
};
#pragma pack(pop) // Restore class alignment

#pragma pack(push, 2) // Set class alignment to 2 bytes
struct MethodTypeEntryInfo {
	u2 descriptor_index = NULL;
};

struct DynamicEntryInfo {
	u2 bootstrap_method_attr_index = NULL;
	u2 name_and_type_index         = NULL;
};
typedef DynamicEntryInfo InvokeDynamicEntryInfo;
#pragma pack(pop) // Restore

class ConstantPoolEntry {
public:
	u1 tag = NULL;
	std::shared_ptr<u1[]> info;

	bool operator==(const ConstantPoolEntry& other) {
		if (other.tag != tag) return false;
		else if (other.info.get() == info.get()) return true;
		else if (other.info.get() == nullptr || info.get() == nullptr) return false;

		u2 info_size = NULL;

		switch (other.tag) {
			case CONSTANT_Class:
			case CONSTANT_String:
			case CONSTANT_MethodType:
			case CONSTANT_Module:
			case CONSTANT_Package: info_size = 2; break;
			case CONSTANT_MethodHandle: info_size = sizeof(MethodHandleEntryInfo); break;
			case CONSTANT_Fieldref:
			case CONSTANT_Methodref:
			case CONSTANT_InterfaceMethodref:
			case CONSTANT_Integer:
			case CONSTANT_Float:
			case CONSTANT_NameAndType:
			case CONSTANT_Dynamic:
			case CONSTANT_InvokeDynamic: info_size = 4; break;
			case CONSTANT_Long:
			case CONSTANT_Double: info_size = 8; break;
			case CONSTANT_Utf8: info_size = 2 + reinterpret_cast<Utf8EntryInfo*>(other.info.get())->length; break;
			default: return false;
		}
		
		if (std::memcmp(other.info.get(), info.get(), info_size) == 0) return true;
		return false;
	}
};

struct AttributeNameIndexes {
	u2 code              = NULL;
	u2 line_number_table = NULL;
};

class ConstantPoolInfo {
public:
	AttributeNameIndexes attribute_indexes;

	u2 entries_count = NULL;
	std::vector<ConstantPoolEntry> entries;

	inline ConstantPoolEntry get_entry(u2 index) { return entries[index - 1]; }

	u2 find_entry(u1 tag, ...);
	u2 add_entry(u1 tag, ...);

private:
	static ConstantPoolEntry create_entry(u1 tag, va_list& vargs);
};