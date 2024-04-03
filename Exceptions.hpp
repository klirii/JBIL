#pragma once
#include "Types.hpp"

struct ExceptionHandlerInfo {
	u2 start_pc   = 0;
	u2 end_pc     = 0;
	u2 handler_pc = 0;
	u2 catch_type = 0;
};