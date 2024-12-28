#pragma once

enum class ClkParserState
{	
	PARSE_HEADER = 0,
	PARSE_WAIT_HEADER_END,
	PARSE_SATELLITES,	
	PARSE_DONE
};