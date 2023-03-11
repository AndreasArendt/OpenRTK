#pragma once

enum class RinexReaderState
{	
	PARSE_HEADER = 0,
	PARSE_OBS_TYPES,
	PARSE_EPOCH,	
};