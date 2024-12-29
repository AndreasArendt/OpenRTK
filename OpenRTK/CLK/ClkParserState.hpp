#pragma once

enum class ClkParserState
{	
	PARSE_HEADER = 0,
	PARSE_WAIT_HEADER_END,
	PARSE_SATELLITE_CLK_BIAS,	
	PARSE_DONE
};