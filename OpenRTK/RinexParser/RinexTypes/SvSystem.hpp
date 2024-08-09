#pragma once

enum class SvSystem : char
{
	GPS = 'G',
	GLONASS = 'R',
	GALILEO = 'E',
	GZSS = 'J',
	BEIDOU = 'C',
	NAVIC_IRNSS = 'I',
	SBAS = 'S',
	MIXED = 'M',
	UNKNOWN
};