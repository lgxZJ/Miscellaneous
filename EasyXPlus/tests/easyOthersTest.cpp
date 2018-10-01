#include "..\src\easyOthers.h"
#include "..\src\easyExcept.h"

#include "SU.h"

using namespace EasyXPlus;

///////////////////////////////////////////////////////////////////////////////
//										Tests

void PauseAll_PassZero_ThrowExcept()
{
	SU_ASSERT_THROW( Others::pause(0), EasyExcept);
}