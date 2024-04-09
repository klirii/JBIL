#include "../include/MethodInfo.hpp"

CodeAttributeInfo MethodInfo::get_code_attribute() {
	for (u2 i = 0; i < attributes_count; i++)
		if (attributes[i].name_index == attributes[i].cp_info->attribute_indexes.code)
			return CodeAttributeInfo(&attributes[i]);
	
	return {};
}