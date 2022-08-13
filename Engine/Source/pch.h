#pragma once

extern "C"
{
#include "../../ThirdParty/lua542/include/lua.h"
#include "../../ThirdParty/lua542/include/lauxlib.h"
#include "../../ThirdParty/lua542/include/lualib.h"
}

// Link to lua library
#ifdef _WIN32
#pragma comment(lib, "../../ThirdParty/lua542/liblua54.a")
#endif

// RayLib
#include <raylib.h>
#include <raymath.h>
#include "rlgl.h"

// Standard
#include <string>
#include <string_view>
#include <exception>
#include <stdexcept>
#include <chrono>
#include <iostream>
#include <sstream>
#include <fstream>
#include <fcntl.h>
#include <cstdio>
#include <memory>
#include <cassert> 
#include <unordered_map>
#include <vector>
#include <array>
#include <set>
#include <queue>
#include <condition_variable>
#include <bitset>
#include <exception>
#include <filesystem>

// Defines & Macros
#ifdef DEBUG
	#define LOG(c_str) printf("%s\n", c_str);
	#define LOG_ERROR(c_str) printf("\n\n%s\n", c_str);
#else
	#define LOG(x)
	#define LOG_ERROR(x)
#endif // DEBUG
