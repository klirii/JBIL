#pragma once
#include "ConstantPool.hpp"
#include "ClassFileStream.hpp"
#include "Exceptions.hpp"

#include <vector>
#include <memory>

class AttributeInfo {
public:
	ConstantPoolInfo* cp_info = nullptr;

	u2 name_index = NULL;
	u4 length     = NULL;
	std::vector<u1> info;
};

struct CodeAttributeInfo {
	AttributeInfo* attribute = nullptr;

	CodeAttributeInfo() = default;
	CodeAttributeInfo(AttributeInfo* attribute);

	u2 max_stack   = NULL;
	u2 max_locals  = NULL;

	u4 code_length = NULL;
	std::vector<u1> code;

	u2 exception_table_length = NULL;
	std::shared_ptr<ExceptionHandlerInfo[]> exception_table;

	u2 attributes_count = NULL;
	std::shared_ptr<AttributeInfo[]> attributes;

	std::vector<u1> dump();
	void update();

	void change_code(std::vector<u1>& code);
	void change_code(u4 pos, u1 instructions_count, ...);

private:
	void parse_code(ClassFileStream& stream);
	void parse_exception_table(ClassFileStream& stream);
	void parse_attributes(ClassFileStream& stream);

	void dump_code(std::vector<u1>& buffer);
	void dump_exception_table(std::vector<u1>& buffer);
	void dump_attributes(std::vector<u1>& buffer);
};