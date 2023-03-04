#pragma once

enum class RinexReaderState
{
	IDLE = 0,
	PARSE_OBS_TYPES,
	PARSE_EPOCH,	
};