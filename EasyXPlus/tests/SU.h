#ifndef SIMPLE_UNIT_H
#define SIMPLE_UNIT_H

#include <cassert>

///////////////////////////////////////////////////////////////////////////////
//								Useful Macors

#define SU_ASSERT_THROW(Exp, ExceptType)				\
	try													\
	{													\
		Exp;											\
		assert(false && "Exception not throw!");		\
	}													\
	catch (ExceptType)									\
	{}													\
	catch (...)											\
	{													\
		assert(false && "Different exception thrown!");	\
	}

#endif