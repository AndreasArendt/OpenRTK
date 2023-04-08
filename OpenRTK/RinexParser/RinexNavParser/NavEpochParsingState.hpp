#pragma once

enum NavEpochParsingState
{
	NavEpochParsingState_IDLE = 0,
	NavEpochParsingState_CLOCK_ERROR,
	NavEpochParsingState_ORBIT,
	NavEPochParsingState_SKIP
};