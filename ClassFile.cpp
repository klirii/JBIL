#include "ClassFile.hpp"
#include "ClassFileParser.hpp"
#include "ClassFileDumper.hpp"

#include <fstream>

ClassFile::ClassFile(const char* path) {
	std::ifstream is(path, std::ios::binary);

	if (is.is_open()) {
		is.seekg(0, std::ios::end);
		std::streamsize size = is.tellg();
		is.seekg(0, std::ios::beg);

		temp_buffer = std::shared_ptr<u1[]>(new u1[size]);
		is.read((char*)temp_buffer.get(), size);

		this->ClassFile::ClassFile(temp_buffer.get(), static_cast<u4>(size));
	}
}

ClassFile::ClassFile(u1* buffer, u4 size) : stream(buffer, size) {
	ClassFileParser(this);
}

MethodInfo* ClassFile::find_method(u2 name_index, u2 descriptor_index) {
	for (u2 i = 0; i < methods_count; i++)
		if (methods[i].name_index == name_index && methods[i].descriptor_index == descriptor_index)
			return &methods[i];

	return nullptr;
}

std::vector<u1> ClassFile::dump() {
	return ClassFileDumper(this).get_data();
}

void ClassFile::dump(const char* path) {
	std::ofstream os(path, std::ios::binary);

	if (os.is_open()) {
		std::vector<u1> class_data = dump();
		os.write((const char*)class_data.data(), class_data.size());
	}
}