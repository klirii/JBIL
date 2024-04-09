#include "../include/ClassFileParser.hpp"

ClassFileParser::ClassFileParser(ClassFile* cf) : cf(cf) {
	cf->magic_dword   = cf->stream.get_u4();
	cf->minor_version = cf->stream.get_u2();
	cf->major_version = cf->stream.get_u2();

	parse_constant_pool();

	cf->access_flags = cf->stream.get_u2();
	cf->this_class   = cf->stream.get_u2();
	cf->super_class  = cf->stream.get_u2();

	parse_interfaces();
	parse_fields();
	parse_methods();
	parse_attributes();
}

void ClassFileParser::parse_constant_pool() {
	cf->cp_info.entries_count = cf->stream.get_u2();

	for (u2 index = 1; index < cf->cp_info.entries_count; index++) {
		ConstantPoolEntry entry;
		entry.tag = cf->stream.get_u1();
		
		switch (entry.tag) {
			case CONSTANT_Class:
			case CONSTANT_Module:
			case CONSTANT_Package: {
				entry.info = std::shared_ptr<u1[]>(new u1[sizeof(NameIndexEntryInfo)]);
				NameIndexEntryInfo* entry_info = (NameIndexEntryInfo*)entry.info.get();

				entry_info->name_index = cf->stream.get_u2();
				break;
			}
			case CONSTANT_Fieldref:
			case CONSTANT_Methodref:
			case CONSTANT_InterfaceMethodref: {
				entry.info = std::shared_ptr<u1[]>(new u1[sizeof(RefEntryInfo)]);
				RefEntryInfo* entry_info = (RefEntryInfo*)entry.info.get();

				entry_info->class_index         = cf->stream.get_u2();
				entry_info->name_and_type_index = cf->stream.get_u2();
				break;
			}
			case CONSTANT_String: {
				entry.info = std::shared_ptr<u1[]>(new u1[sizeof(StringEntryInfo)]);
				StringEntryInfo* entry_info = (StringEntryInfo*)entry.info.get();

				entry_info->string_index = cf->stream.get_u2();
				break;
			}
			case CONSTANT_Integer: {
				entry.info = std::shared_ptr<u1[]>(new u1[sizeof(IntegerEntryInfo)]);
				IntegerEntryInfo* entry_info = (IntegerEntryInfo*)entry.info.get();

				entry_info->value = cf->stream.get_u4();
				break;
			}
			case CONSTANT_Float: {
				entry.info = std::shared_ptr<u1[]>(new u1[sizeof(FloatEntryInfo)]);
				FloatEntryInfo* entry_info = (FloatEntryInfo*)entry.info.get();

				u4 bytes = cf->stream.get_u4();
				entry_info->value = *(float*)&bytes;
				break;
			}
			case CONSTANT_Long: {
				entry.info = std::shared_ptr<u1[]>(new u1[sizeof(LongEntryInfo)]);
				LongEntryInfo* entry_info = (LongEntryInfo*)entry.info.get();

				entry_info->value = cf->stream.get_u8();
				index++;
				break;
			}
			case CONSTANT_Double: {
				entry.info = std::shared_ptr<u1[]>(new u1[sizeof(DoubleEntryInfo)]);
				DoubleEntryInfo* entry_info = (DoubleEntryInfo*)entry.info.get();

				u4 high_bytes = cf->stream.get_u4();
				u4 low_bytes  = cf->stream.get_u4();

				*reinterpret_cast<u4*>(&entry_info->value) = low_bytes;
				*(reinterpret_cast<u4*>(&entry_info->value) + 1) = high_bytes;

				index++;
				break;
			}
			case CONSTANT_NameAndType: {
				entry.info = std::shared_ptr<u1[]>(new u1[sizeof(NameAndTypeEntryInfo)]);
				NameAndTypeEntryInfo* entry_info = (NameAndTypeEntryInfo*)entry.info.get();

				entry_info->name_index       = cf->stream.get_u2();
				entry_info->descriptor_index = cf->stream.get_u2();
				break;
			}
			case CONSTANT_MethodHandle: {
				entry.info = std::shared_ptr<u1[]>(new u1[sizeof(MethodHandleEntryInfo)]);
				MethodHandleEntryInfo* entry_info = (MethodHandleEntryInfo*)entry.info.get();

				entry_info->reference_kind  = cf->stream.get_u1();
				entry_info->reference_index = cf->stream.get_u2();
				break;
			}
			case CONSTANT_MethodType: {
				entry.info = std::shared_ptr<u1[]>(new u1[sizeof(MethodTypeEntryInfo)]);
				MethodTypeEntryInfo* entry_info = (MethodTypeEntryInfo*)entry.info.get();

				entry_info->descriptor_index = cf->stream.get_u2();
				break;
			}
			case CONSTANT_Dynamic:
			case CONSTANT_InvokeDynamic: {
				entry.info = std::shared_ptr<u1[]>(new u1[sizeof(DynamicEntryInfo)]);
				DynamicEntryInfo* entry_info = (DynamicEntryInfo*)entry.info.get();

				entry_info->bootstrap_method_attr_index = cf->stream.get_u2();
				entry_info->name_and_type_index         = cf->stream.get_u2();
				break;
			}
			case CONSTANT_Utf8: {
				u2 length = cf->stream.get_u2();

				entry.info = std::shared_ptr<u1[]>(new u1[2 + length]);
				Utf8EntryInfo* entry_info = (Utf8EntryInfo*)entry.info.get();

				entry_info->length = length;
				std::memcpy(&entry_info->str, cf->stream.current, length);
				cf->stream.current += length;

				break;
			}
			default: {
				return;
			}
		}

		cf->cp_info.entries.push_back(entry);

		if (entry.tag == CONSTANT_Long || entry.tag == CONSTANT_Double)
			cf->cp_info.entries.push_back(ConstantPoolEntry());
	}

	cf->cp_info.attribute_indexes.code = cf->cp_info.find_entry(CONSTANT_Utf8, "Code");
	cf->cp_info.attribute_indexes.line_number_table = cf->cp_info.find_entry(CONSTANT_Utf8, "LineNumberTable");
}

void ClassFileParser::parse_interfaces() {
	cf->interfaces_count = cf->stream.get_u2();
	cf->interfaces       = std::shared_ptr<u2[]>(new u2[cf->interfaces_count]);

	std::memcpy(cf->interfaces.get(), cf->stream.current, cf->interfaces_count * sizeof(u2));
	cf->stream.current += cf->interfaces_count * sizeof(u2);
}

void ClassFileParser::parse_fields() {
	cf->fields_count = cf->stream.get_u2();
	cf->fields       = std::shared_ptr<FieldInfo[]>(new FieldInfo[cf->fields_count]);

	for (u2 i = 0; i < cf->fields_count; i++) {
		cf->fields[i].access_flags     = cf->stream.get_u2();
		cf->fields[i].name_index       = cf->stream.get_u2();
		cf->fields[i].descriptor_index = cf->stream.get_u2();
		cf->fields[i].attributes_count = cf->stream.get_u2();
		cf->fields[i].attributes       = std::shared_ptr<AttributeInfo[]>(new AttributeInfo[cf->fields[i].attributes_count]);

		for (u2 j = 0; j < cf->fields[i].attributes_count; j++) {
			cf->fields[i].attributes[j].name_index = cf->stream.get_u2();
			cf->fields[i].attributes[j].length     = cf->stream.get_u4();
			
			std::vector<u1>& attribute_info = cf->fields[i].attributes[j].info;
			attribute_info.insert(attribute_info.end(), cf->stream.current, cf->stream.current + cf->fields[i].attributes[j].length);
			cf->stream.current += cf->fields[i].attributes[j].length;
		}
	}
}

void ClassFileParser::parse_methods() {
	cf->methods_count = cf->stream.get_u2();
	cf->methods       = std::shared_ptr<MethodInfo[]>(new MethodInfo[cf->methods_count]);

	for (u2 i = 0; i < cf->methods_count; i++) {
		cf->methods[i].access_flags     = cf->stream.get_u2();
		cf->methods[i].name_index       = cf->stream.get_u2();
		cf->methods[i].descriptor_index = cf->stream.get_u2();
		cf->methods[i].attributes_count = cf->stream.get_u2();
		cf->methods[i].attributes       = std::shared_ptr<AttributeInfo[]>(new AttributeInfo[cf->methods[i].attributes_count]);
	
		for (u2 j = 0; j < cf->methods[i].attributes_count; j++) {
			cf->methods[i].attributes[j].cp_info    = &cf->cp_info;
			cf->methods[i].attributes[j].name_index = cf->stream.get_u2();
			cf->methods[i].attributes[j].length     = cf->stream.get_u4();
			
			std::vector<u1>& attribute_info = cf->methods[i].attributes[j].info;
			attribute_info.insert(attribute_info.end(), cf->stream.current, cf->stream.current + cf->methods[i].attributes[j].length);
			cf->stream.current += cf->methods[i].attributes[j].length;
		}
	}
}

void ClassFileParser::parse_attributes() {
	cf->attributes_count = cf->stream.get_u2();
	cf->attributes = std::shared_ptr<AttributeInfo[]>(new AttributeInfo[cf->attributes_count]);

	for (u2 i = 0; i < cf->attributes_count; i++) {
		cf->attributes[i].name_index = cf->stream.get_u2();
		cf->attributes[i].length     = cf->stream.get_u4();
		
		std::vector<u1>& attribute_info = cf->attributes[i].info;
		attribute_info.insert(attribute_info.end(), cf->stream.current, cf->stream.current + cf->attributes[i].length);
		cf->stream.current += cf->attributes[i].length;
	}
}