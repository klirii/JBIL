#include "../include/ClassFile.hpp"
#include "../include/Bytecodes.hpp"

int main() {
	// Initialization of the class object for further work with it, the path or buffer and size must be passed to the constructor
	ClassFile cf("net\\minecraft\\client\\renderer\\EntityRenderer.class");

	// Search indexes of the name and descriptor of the method whose bytecode you want to change
	u2 method_name_index = cf.cp_info.find_entry(CONSTANT_Utf8, "getMouseOver");
	u2 method_desc_index = cf.cp_info.find_entry(CONSTANT_Utf8, "(F)V");

	// Adding to constant pool a reference to the class to be referred
	u2 test_class_name_index = cf.cp_info.add_entry(CONSTANT_Utf8, "net/minecraft/client/Test");
	u2 test_class_index      = cf.cp_info.add_entry(CONSTANT_Class, test_class_name_index);

	// Adding to constant pool a reference to the field to be referenced in the modified bytecode
	u2 field_name_index = cf.cp_info.add_entry(CONSTANT_Utf8, "reachDistance");
	u2 field_desc_index = cf.cp_info.add_entry(CONSTANT_Utf8, "D");
	u2 field_name_and_type_index = cf.cp_info.add_entry(CONSTANT_NameAndType, field_name_index, field_desc_index);
	u2 fieldref_index = cf.cp_info.add_entry(CONSTANT_Fieldref, test_class_index, field_name_and_type_index);

	// Finding required method and changing its bytecode
	MethodInfo*       get_mouse_over      = cf.find_method(method_name_index, method_desc_index);
	CodeAttributeInfo get_mouse_over_code = get_mouse_over->get_code_attribute();
	get_mouse_over_code.change_code(513, 1, Bytecodes::_getstatic, fieldref_index);
	
	// Class dump to file
	cf.dump("path/to/dump");
	return 0;
 }
