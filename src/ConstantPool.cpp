#include "../include/ConstantPool.hpp"
#include <stdarg.h>

u2 ConstantPoolInfo::find_entry(u1 tag, ...) {
	if (entries.size() < 1) return NULL;

	va_list vargs;
	va_start(vargs, tag);

	ConstantPoolEntry required_entry = create_entry(tag, vargs);
	va_end(vargs);

	for (u2 index = 1; index < entries_count; index++) {
		ConstantPoolEntry entry = get_entry(index);
		if (entry == required_entry) return index;
		if (entry.tag == CONSTANT_Long || entry.tag == CONSTANT_Double) index++;
	}

	return NULL;
}

u2 ConstantPoolInfo::add_entry(u1 tag, ...) {
	if (!tag) return NULL;

	va_list vargs;
	va_start(vargs, tag);

	ConstantPoolEntry entry = create_entry(tag, vargs);
	va_end(vargs);

	entries.push_back(entry);
	entries_count++;
	return static_cast<u2>(entries.size());
}

ConstantPoolEntry ConstantPoolInfo::create_entry(u1 tag, va_list& vargs) {
	ConstantPoolEntry entry;
	entry.tag = tag;

	switch (tag) {
		case CONSTANT_Class:
		case CONSTANT_Module:
		case CONSTANT_Package: {
			entry.info = std::shared_ptr<u1[]>(new u1[sizeof(NameIndexEntryInfo)]);
			NameIndexEntryInfo* entry_info = (NameIndexEntryInfo*)entry.info.get();

			entry_info->name_index = va_arg(vargs, u2);
			break;
		}
		case CONSTANT_Fieldref:
		case CONSTANT_Methodref:
		case CONSTANT_InterfaceMethodref: {
			entry.info = std::shared_ptr<u1[]>(new u1[sizeof(RefEntryInfo)]);
			RefEntryInfo* entry_info = (RefEntryInfo*)entry.info.get();

			entry_info->class_index         = va_arg(vargs, u2);
			entry_info->name_and_type_index = va_arg(vargs, u2);
			break;
		}
		case CONSTANT_String: {
			entry.info = std::shared_ptr<u1[]>(new u1[sizeof(StringEntryInfo)]);
			StringEntryInfo* entry_info = (StringEntryInfo*)entry.info.get();

			entry_info->string_index = va_arg(vargs, u2);
			break;
		}
		case CONSTANT_Integer: {
			entry.info = std::shared_ptr<u1[]>(new u1[sizeof(IntegerEntryInfo)]);
			IntegerEntryInfo* entry_info = (IntegerEntryInfo*)entry.info.get();

			entry_info->value = va_arg(vargs, int);
			break;
		}
		case CONSTANT_Float: {
			entry.info = std::shared_ptr<u1[]>(new u1[sizeof(FloatEntryInfo)]);
			FloatEntryInfo* entry_info = (FloatEntryInfo*)entry.info.get();

			entry_info->value = static_cast<float>(va_arg(vargs, double));
			break;
		}
		case CONSTANT_Long: {
			entry.info = std::shared_ptr<u1[]>(new u1[sizeof(LongEntryInfo)]);
			LongEntryInfo* entry_info = (LongEntryInfo*)entry.info.get();

			entry_info->value = va_arg(vargs, long long);
			break;
		}
		case CONSTANT_Double: {
			entry.info = std::shared_ptr<u1[]>(new u1[sizeof(DoubleEntryInfo)]);
			DoubleEntryInfo* entry_info = (DoubleEntryInfo*)entry.info.get();

			entry_info->value = va_arg(vargs, double);
			break;
		}
		case CONSTANT_NameAndType: {
			entry.info = std::shared_ptr<u1[]>(new u1[sizeof(NameAndTypeEntryInfo)]);
			NameAndTypeEntryInfo* entry_info = (NameAndTypeEntryInfo*)entry.info.get();

			entry_info->name_index       = va_arg(vargs, u2);
			entry_info->descriptor_index = va_arg(vargs, u2);
			break;
		}
		case CONSTANT_MethodHandle: {
			entry.info = std::shared_ptr<u1[]>(new u1[sizeof(MethodHandleEntryInfo)]);
			MethodHandleEntryInfo* entry_info = (MethodHandleEntryInfo*)entry.info.get();

			entry_info->reference_kind  = va_arg(vargs, u1);
			entry_info->reference_index = va_arg(vargs, u2);
			break;
		}
		case CONSTANT_MethodType: {
			entry.info = std::shared_ptr<u1[]>(new u1[sizeof(MethodTypeEntryInfo)]);
			MethodTypeEntryInfo* entry_info = (MethodTypeEntryInfo*)entry.info.get();

			entry_info->descriptor_index = va_arg(vargs, u2);
			break;
		}
		case CONSTANT_Dynamic:
		case CONSTANT_InvokeDynamic: {
			entry.info = std::shared_ptr<u1[]>(new u1[sizeof(DynamicEntryInfo)]);
			DynamicEntryInfo* entry_info = (DynamicEntryInfo*)entry.info.get();

			entry_info->bootstrap_method_attr_index = va_arg(vargs, u2);
			entry_info->name_and_type_index         = va_arg(vargs, u2);
			break;
		}
		case CONSTANT_Utf8: {
			const char* c_str = va_arg(vargs, const char*);

			entry.info = std::shared_ptr<u1[]>(new u1[2 + strlen(c_str)]);
			Utf8EntryInfo* entry_info = (Utf8EntryInfo*)entry.info.get();

			entry_info->length = static_cast<u2>(strlen(c_str));
			std::memcpy(&entry_info->str, c_str, entry_info->length);
			break;
		}
	}

	if (entry.info.get() != nullptr) return entry;
	return {};
}