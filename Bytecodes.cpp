#include "Bytecodes.hpp"
#include "ClassFileDumper.hpp"

#include <stdarg.h>

std::vector<u1> Bytecodes::build_instruction(Opcode opcode, va_list& vargs) {
	std::vector<u1> instruction;
	instruction.push_back(opcode);

	switch (opcode) {
		case _bipush:
		case _ldc:
		case _iload:
		case _lload: 
		case _fload: 
		case _dload:
		case _aload:
		case _istore:
		case _lstore:
		case _fstore:
		case _dstore:
		case _astore:
		case _ret:
		case _newarray: {
			ClassFileDumper::dump_u1(instruction, va_arg(vargs, u1));
			break;
		}
		case _iinc: {
			ClassFileDumper::dump_u1(instruction, va_arg(vargs, u1)); // index
			ClassFileDumper::dump_u1(instruction, va_arg(vargs, u1)); // const
			break;
		}
		case _sipush:
		case _ldc_w:
		case _ldc2_w:
		case _ifeq:
		case _ifne:
		case _iflt:
		case _ifge:
		case _ifgt:
		case _ifle:
		case _if_icmpeq:
		case _if_icmpne:
		case _if_icmplt:
		case _if_icmpge:
		case _if_icmpgt:
		case _if_icmple:
		case _if_acmpeq:
		case _if_acmpne:
		case _goto:
		case _jsr:
		case _getstatic:
		case _putstatic:
		case _getfield:
		case _putfield:
		case _invokevirtual:
		case _invokespecial:
		case _invokestatic:
		case _new:
		case _anewarray:
		case _checkcast:
		case _instanceof:
		case _ifnull:
		case _ifnonnull: {
			ClassFileDumper::dump_u2(instruction, va_arg(vargs, u2));
			break;
		}
		case _multianewarray: {
			ClassFileDumper::dump_u2(instruction, va_arg(vargs, u2)); // index
			ClassFileDumper::dump_u1(instruction, va_arg(vargs, u1)); // dimensions
			break;
		}
		case _wide: {
			u1 format = va_arg(vargs, u1); // instruction modification format
			
			if (format == 1) {
				ClassFileDumper::dump_u1(instruction, va_arg(vargs, u1)); // opcode
				ClassFileDumper::dump_u2(instruction, va_arg(vargs, u2)); // wide index
			}
			else if (format == 2) {
				ClassFileDumper::dump_u1(instruction, va_arg(vargs, u1)); // iinc opcode
				ClassFileDumper::dump_u2(instruction, va_arg(vargs, u2)); // wide index
				ClassFileDumper::dump_u2(instruction, va_arg(vargs, u2)); // wide const
			}
			else {
				return {};
			}

			break;
		}
		case _invokeinterface: {
			ClassFileDumper::dump_u2(instruction, va_arg(vargs, u2)); // index
			ClassFileDumper::dump_u1(instruction, va_arg(vargs, u1)); // arguments count, where an argument of type long or double is two arguments
			ClassFileDumper::dump_u1(instruction, 0); // reserved byte
			break;
		}
		case _invokedynamic: {
			ClassFileDumper::dump_u2(instruction, va_arg(vargs, u2)); // index
			ClassFileDumper::dump_u2(instruction, 0); // reserved bytes?
			break;
		}
		case _goto_w:
		case _jsr_w: {
			ClassFileDumper::dump_u4(instruction, va_arg(vargs, u4));
			break;
		}
		case _lookupswitch: {
			u1 pad_size = va_arg(vargs, u1); // padding size in bytes
			for (u1 i = 0; i < pad_size; i++) ClassFileDumper::dump_u1(instruction, 0); // pad [0 - 3]

			ClassFileDumper::dump_u4(instruction, va_arg(vargs, u4)); // default (offset)

			signed int npairs = va_arg(vargs, signed int);
			ClassFileDumper::dump_u4(instruction, npairs);

			for (signed int i = 0; i < npairs; i++) {
				ClassFileDumper::dump_u4(instruction, va_arg(vargs, u4)); // match
				ClassFileDumper::dump_u4(instruction, va_arg(vargs, u4)); // offset
			}

			break;
		}
		case _tableswitch: {
			u1 pad_size = va_arg(vargs, u1); // padding size in bytes
			for (u1 i = 0; i < pad_size; i++) ClassFileDumper::dump_u1(instruction, 0); // pad [0 - 3]

			ClassFileDumper::dump_u4(instruction, va_arg(vargs, u4)); // default (offset)

			signed int low  = va_arg(vargs, signed int);
			signed int high = va_arg(vargs, signed int);

			ClassFileDumper::dump_u4(instruction, low);
			ClassFileDumper::dump_u4(instruction, high);
			
			for (signed int i = 0; i < (high - low + 1); i++)
				ClassFileDumper::dump_u4(instruction, va_arg(vargs, u4)); // jump offset

			break;
		}
		default: {
			return {};
		}
	}

	return instruction;
}