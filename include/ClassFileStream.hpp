#pragma once
#include "Types.hpp"

class ClassFileStream {
public:
	u1* start = nullptr;
	u1* current = nullptr;
	u1* end = nullptr;

	ClassFileStream() = default;
	ClassFileStream(u1* buffer, u4 size) {
		start = buffer;
		current = buffer;
		end = buffer + size;
	}

	u4 length() { return static_cast<u4>(end - start); }
	u4 remaining() { return static_cast<u4>(end - current); }

	u1 get_u1();
	u2 get_u2();
	u4 get_u4();
	u8 get_u8();
};