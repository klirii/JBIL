#include "../include/ClassFileStream.hpp"
#include <iostream>

u1 ClassFileStream::get_u1() {
	if (remaining() < 1) return NULL;
	return *current++;
}

u2 ClassFileStream::get_u2() {
	if (remaining() < 2) return NULL;

	u2 result = _byteswap_ushort(*reinterpret_cast<u2*>(current));
	current += 2;
	return result;
}

u4 ClassFileStream::get_u4() {
	if (remaining() < 4) return NULL;

	u4 result = _byteswap_ulong(*reinterpret_cast<u4*>(current));
	current += 4;
	return result;
}

u8 ClassFileStream::get_u8() {
	if (remaining() < 8) return NULL;

	u8 result = _byteswap_uint64(*reinterpret_cast<u8*>(current));
	current += 8;
	return result;
}