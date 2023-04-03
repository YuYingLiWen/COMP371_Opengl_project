#pragma once

#include <iostream>

#ifdef _DEBUG
#define DEBUG_LOG(x) std::cout << "DEBUG: " << x << std::endl
#define BLANK_LINE std::cout << std::endl
#else
#define DEBUG_LOG(x)
#define BLANK_LINE 
#endif // _DEBUG

#define PRINT_LOG(x) std::cout << ">> " << x << std::endl
#define ERROR_LOG(x) std::cout << "ERROR: " << x << std::endl
