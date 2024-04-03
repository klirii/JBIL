#pragma once
#include "ClassFile.hpp"

class ClassFileDumper {
public:
	ClassFileDumper(ClassFile* cf);
	std::vector<u1> get_data() { return data; }

	static inline void dump_u1(std::vector<u1>& buffer, u1 value) { buffer.push_back(value); }
	static void dump_u2(std::vector<u1>& buffer, u2 value);
	static void dump_u4(std::vector<u1>& buffer, u4 value);
	static void dump_u8(std::vector<u1>& buffer, u8 value);
private:
	ClassFile* cf;
	std::vector<u1> data;

	void dump_constant_pool();
	void dump_interfaces();
	void dump_fields();
	void dump_methods();
	void dump_attributes();
};