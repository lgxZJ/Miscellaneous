///
///	@file		Others module.
///	@author		lgxZJ@outlook.com
///	@date		2017/6/13
///	@version	1.0
///

#ifndef EASY_MISCELLANEOUS_H
#define EASY_MISCELLANEOUS_H

namespace EasyXPlus
{
	///
	///	@brief	A class contains miscellaneous functionalities.
	///
	class Others
	{
	public:
		///
		///	@brief	Pause the calling thread for the given time.
		///	@param	milliSecond	A positive time in millisecond unit.
		///	@throw	EasyExcept	If the time passed is 0, it throws an exception.
		///
		static void pause(unsigned milliSecond);
	};
}

#endif