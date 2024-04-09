#include "../include/ClassFileDumper.hpp"
#include <vector>

ClassFileDumper::ClassFileDumper(ClassFile* cf) : cf(cf) {
	dump_u4(data, cf->magic_dword);
	dump_u2(data, cf->minor_version);
	dump_u2(data, cf->major_version);

	dump_constant_pool();

	dump_u2(data, cf->access_flags);
	dump_u2(data, cf->this_class);
	dump_u2(data, cf->super_class);

	dump_interfaces();
	dump_fields();
	dump_methods();
	dump_attributes();
}

void ClassFileDumper::dump_u2(std::vector<u1>& buffer, u2 value) {
	value = _byteswap_ushort(value);
	buffer.insert(buffer.end(), (u1*)&value, (u1*)&value + sizeof(u2));
}

void ClassFileDumper::dump_u4(std::vector<u1>& buffer, u4 value) {
	value = _byteswap_ulong(value);
	buffer.insert(buffer.end(), (u1*)&value, (u1*)&value + sizeof(u4));
}

void ClassFileDumper::dump_u8(std::vector<u1>& buffer, u8 value) {
	value = _byteswap_uint64(value);
	buffer.insert(buffer.end(), (u1*)&value, (u1*)&value + sizeof(u8));
}

void ClassFileDumper::dump_constant_pool() {
	dump_u2(data, cf->cp_info.entries_count);

	for (u2 index = 1; index < cf->cp_info.entries_count; index++) {
		ConstantPoolEntry entry = cf->cp_info.get_entry(index);
		dump_u1(data, entry.tag);

		switch (entry.tag) {
			case CONSTANT_Class:
			case CONSTANT_Module:
			case CONSTANT_Package: {
				NameIndexEntryInfo* entry_info = (NameIndexEntryInfo*)entry.info.get();
				dump_u2(data, entry_info->name_index);
				break;
			}
			case CONSTANT_Fieldref:
			case CONSTANT_Methodref:
			case CONSTANT_InterfaceMethodref: {
				RefEntryInfo* entry_info = (RefEntryInfo*)entry.info.get();
				dump_u2(data, entry_info->class_index);
				dump_u2(data, entry_info->name_and_type_index);

				break;
			}
			case CONSTANT_String: {
				StringEntryInfo* entry_info = (StringEntryInfo*)entry.info.get();
				dump_u2(data, entry_info->string_index);
				break;
			}
			case CONSTANT_Integer: {
				IntegerEntryInfo* entry_info = (IntegerEntryInfo*)entry.info.get();
				dump_u4(data, entry_info->value);
				break;
			}
			case CONSTANT_Float: {
				FloatEntryInfo* entry_info = (FloatEntryInfo*)entry.info.get();
				dump_u4(data, *(u4*)&entry_info->value);
				break;
			}
			case CONSTANT_Long: {
				LongEntryInfo* entry_info = (LongEntryInfo*)entry.info.get();
				dump_u8(data, entry_info->value);

				index++;
				break;
			}
			case CONSTANT_Double: {
				DoubleEntryInfo* entry_info = (DoubleEntryInfo*)entry.info.get();
				dump_u4(data, *(reinterpret_cast<u4*>(&entry_info->value) + 1));
				dump_u4(data, *reinterpret_cast<u4*>(&entry_info->value));

				index++;
				break;
			}
			case CONSTANT_NameAndType: {
				NameAndTypeEntryInfo* entry_info = (NameAndTypeEntryInfo*)entry.info.get();
				dump_u2(data, entry_info->name_index);
				dump_u2(data, entry_info->descriptor_index);

				break;
			}
			case CONSTANT_MethodHandle: {
				MethodHandleEntryInfo* entry_info = (MethodHandleEntryInfo*)entry.info.get();
				dump_u1(data, entry_info->reference_kind);
				dump_u2(data, entry_info->reference_index);

				break;
			}
			case CONSTANT_MethodType: {
				MethodTypeEntryInfo* entry_info = (MethodTypeEntryInfo*)entry.info.get();
				dump_u2(data, entry_info->descriptor_index);
				break;
			}
			case CONSTANT_Dynamic:
			case CONSTANT_InvokeDynamic: {
				DynamicEntryInfo* entry_info = (DynamicEntryInfo*)entry.info.get();
				dump_u2(data, entry_info->bootstrap_method_attr_index);
				dump_u2(data, entry_info->name_and_type_index);

				break;
			}
			case CONSTANT_Utf8: {
				Utf8EntryInfo* entry_info = (Utf8EntryInfo*)entry.info.get();
				dump_u2(data, entry_info->length);
				data.insert(data.end(), (u1*)&entry_info->str, (u1*)&entry_info->str + entry_info->length);

				break;
			}
			default: {
				return;
			}
		}
	}
}

void ClassFileDumper::dump_interfaces() {
	dump_u2(data, cf->interfaces_count);
	data.insert(data.end(), (u1*)cf->interfaces.get(), (u1*)cf->interfaces.get() + (cf->interfaces_count * sizeof(u2)));
}

void ClassFileDumper::dump_fields() {
	dump_u2(data, cf->fields_count);

	for (u2 i = 0; i < cf->fields_count; i++) {
		dump_u2(data, cf->fields[i].access_flags);
		dump_u2(data, cf->fields[i].name_index);
		dump_u2(data, cf->fields[i].descriptor_index);
		dump_u2(data, cf->fields[i].attributes_count);

		for (u2 j = 0; j < cf->fields[i].attributes_count; j++) {
			dump_u2(data, cf->fields[i].attributes[j].name_index);
			dump_u4(data, cf->fields[i].attributes[j].length);
			data.insert(data.end(), cf->fields[i].attributes[j].info.begin(), cf->fields[i].attributes[j].info.end());
		}
	}
}

void ClassFileDumper::dump_methods() {
	dump_u2(data, cf->methods_count);

	for (u2 i = 0; i < cf->methods_count; i++) {
		dump_u2(data, cf->methods[i].access_flags);
		dump_u2(data, cf->methods[i].name_index);
		dump_u2(data, cf->methods[i].descriptor_index);
		dump_u2(data, cf->methods[i].attributes_count);

		for (u2 j = 0; j < cf->methods[i].attributes_count; j++) {
			dump_u2(data, cf->methods[i].attributes[j].name_index);
			dump_u4(data, cf->methods[i].attributes[j].length);
			data.insert(data.end(), cf->methods[i].attributes[j].info.begin(), cf->methods[i].attributes[j].info.end());
		}
	}
}

void ClassFileDumper::dump_attributes() {
	dump_u2(data, cf->attributes_count);

	for (u2 i = 0; i < cf->attributes_count; i++) {
		dump_u2(data, cf->attributes[i].name_index);
		dump_u4(data, cf->attributes[i].length);
		data.insert(data.end(), cf->attributes[i].info.begin(), cf->attributes[i].info.end());
	}
}