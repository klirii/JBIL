#pragma once
#include "Attributes.hpp"
#include <memory>

class MethodInfo {
public:
	u2 access_flags     = NULL;
	u2 name_index       = NULL;
	u2 descriptor_index = NULL;
	u2 attributes_count = NULL;
	std::shared_ptr<AttributeInfo[]> attributes;

	CodeAttributeInfo get_code_attribute();
};