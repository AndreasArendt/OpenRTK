#pragma once

enum class RinexParserState
{	
	PARSE_HEADER = 0,
	PARSE_OBS_TYPES,
	PARSE_EPOCH,	
};