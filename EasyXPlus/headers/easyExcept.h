///
///	@file		Exception module.
///	@author		lgxZJ@outlook.com
///	@date		2017/6/12
///	@version	1.0
///

#ifndef EASY_EXCEPT_H
#define EASY_EXCEPT_H

#include <stdexcept>

namespace EasyXPlus
{
	///
	///	@brief	The exception type used by EasyXPlus library.
	///
	class EasyExcept : public std::logic_error
	{
	public:
		///
		///	@brief	Creates an exception from the given error string.
		///
		EasyExcept(std::string str) : logic_error(str)	{}
	};
}

#endif