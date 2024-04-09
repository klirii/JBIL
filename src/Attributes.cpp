#include "../include/Attributes.hpp"
#include "../include/ClassFileDumper.hpp"
#include "../include/Bytecodes.hpp"

#include <stdarg.h>

CodeAttributeInfo::CodeAttributeInfo(AttributeInfo* attribute) : attribute(attribute) {
	ClassFileStream stream(attribute->info.data(), (u4)attribute->info.size());

	max_stack  = stream.get_u2();
	max_locals = stream.get_u2();

	parse_code(stream);
	parse_exception_table(stream);
	parse_attributes(stream);
}

void CodeAttributeInfo::parse_code(ClassFileStream& stream) {
	code_length = stream.get_u4();
	code.insert(code.end(), stream.current, stream.current + code_length);
	stream.current += code_length;
}

void CodeAttributeInfo::parse_exception_table(ClassFileStream& stream) {
	exception_table_length = stream.get_u2();
	exception_table        = std::shared_ptr<ExceptionHandlerInfo[]>(new ExceptionHandlerInfo[exception_table_length]);

	for (u2 i = 0; i < exception_table_length; i++) {
		exception_table[i].start_pc   = stream.get_u2();
		exception_table[i].end_pc     = stream.get_u2();
		exception_table[i].handler_pc = stream.get_u2();
		exception_table[i].catch_type = stream.get_u2();
	}
}

void CodeAttributeInfo::parse_attributes(ClassFileStream& stream) {
	attributes_count = stream.get_u2();
	attributes       = std::shared_ptr<AttributeInfo[]>(new AttributeInfo[attributes_count]);

	for (u2 i = 0; i < attributes_count; i++) {
		attributes[i].name_index = stream.get_u2();
		attributes[i].length     = stream.get_u4();

		attributes[i].info.insert(attributes[i].info.end(), stream.current, stream.current + attributes[i].length);
		stream.current += attributes[i].length;
	}
}

std::vector<u1> CodeAttributeInfo::dump() {
	std::vector<u1> attribute_info;

	ClassFileDumper::dump_u2(attribute_info, max_stack);
	ClassFileDumper::dump_u2(attribute_info, max_locals);

	dump_code(attribute_info);
	dump_exception_table(attribute_info);
	dump_attributes(attribute_info);

	return attribute_info;
}

void CodeAttributeInfo::update() {
	std::vector<u1> attribute_info = dump();
	attribute->info   = attribute_info;
	attribute->length = static_cast<u4>(attribute_info.size());
}

void CodeAttributeInfo::dump_code(std::vector<u1>& buffer) {
	ClassFileDumper::dump_u4(buffer, code_length);
	buffer.insert(buffer.end(), code.begin(), code.end());
}

void CodeAttributeInfo::dump_exception_table(std::vector<u1>& buffer) {
	ClassFileDumper::dump_u2(buffer, exception_table_length);

	for (u2 i = 0; i < exception_table_length; i++) {
		ClassFileDumper::dump_u2(buffer, exception_table[i].start_pc);
		ClassFileDumper::dump_u2(buffer, exception_table[i].end_pc);
		ClassFileDumper::dump_u2(buffer, exception_table[i].handler_pc);
		ClassFileDumper::dump_u2(buffer, exception_table[i].catch_type);
	}
}

void CodeAttributeInfo::dump_attributes(std::vector<u1>& buffer) {
	ClassFileDumper::dump_u2(buffer, attributes_count);

	for (u2 i = 0; i < attributes_count; i++) {
		ClassFileDumper::dump_u2(buffer, attributes[i].name_index);
		ClassFileDumper::dump_u4(buffer, attributes[i].length);
		buffer.insert(buffer.end(), attributes[i].info.begin(), attributes[i].info.end());
	}
}

void CodeAttributeInfo::change_code(std::vector<u1>& code) {
	this->code = code;
	code_length = static_cast<u4>(this->code.size());

	update();
}

void CodeAttributeInfo::change_code(u4 pos, u1 instructions_count, ...) {
	if (instructions_count < 1) return;

	va_list vargs;
	va_start(vargs, instructions_count);

	std::vector<u1> instructions;

	for (u1 i = 0; i < instructions_count; i++) {
		std::vector<u1> instruction = Bytecodes::build_instruction(va_arg(vargs, Bytecodes::Opcode), vargs);
		instructions.insert(instructions.end(), instruction.begin(), instruction.end());
	}

	va_end(vargs);

	std::memcpy(code.data() + pos, instructions.data(), instructions.size());
	update();
}