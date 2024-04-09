#pragma once
#include "ClassFile.hpp"

class ClassFileParser {
public:
	ClassFileParser(ClassFile* cf);
private:
	ClassFile* cf;

	void parse_constant_pool();
	void parse_interfaces();
	void parse_fields();
	void parse_methods();
	void parse_attributes();
};