#pragma once

#include <string>

inline std::wstring ToWString(std::string input)
{
	const size_t c_size = input.size();
	std::wstring ret(c_size, L'#');
	mbstowcs(&ret[0], &input[0], c_size);
	return ret;
}