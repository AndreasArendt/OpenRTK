#pragma once

enum NavEpochParsingState
{
	NavEpochParsingState_IDLE = 0,
	NavEpochParsingState_CLOCK_ERROR,
	NavEpochParsingState_ORBIT_1,
	NavEpochParsingState_ORBIT_2,
	NavEpochParsingState_ORBIT_3,
	NavEpochParsingState_ORBIT_4,
	NavEpochParsingState_ORBIT_5,
	NavEpochParsingState_ORBIT_6,
	NavEpochParsingState_ORBIT_7,
	NavEPochParsingState_SKIP
};