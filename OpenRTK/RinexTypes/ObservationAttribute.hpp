#pragma once

enum ObservationAttribute
{
	Channel_A = 'A',  // A channel(GAL, NavIC / IRNSS, GLO)
	Channel_B = 'B',  // B channel(GAL, NavIC / IRNSS, GLO)
	Channel_C = 'C',  // C channel(GAL, NavIC / IRNSS)
				      // C code - based(SBAS, GPS, GLO, QZSS)
	Channel_D = 'D',  // Semi - codeless(GPS, QZSS)
				      // Data Channel(BDS)
	Channel_E = 'E',  // E Channel(QZSS)
	Channel_I = 'I',  // I channel(GPS, GAL, QZSS, BDS) 
	Channel_L = 'L',  // L channel(L2C GPS, QZSS)
				      // P channel(GPS, QZSS)
	Channel_M = 'M',  // M code - based(GPS)
	Channel_N = 'N',  // Codeless(GPS)
	Channel_P = 'P',  // P code - based(GPS, GLO)
				      // Pilot Channel(BDS)
	Channel_Q = 'Q',  // Q channel(GPS, GAL, QZSS, BDS)
	Channel_S = 'S',  // D channel(GPS, QZSS)
		              // M channel(L2C GPS, QZSS)
	Channel_W = 'W',  // Based on Z - tracking(GPS)(see text)
	Channel_X = 'X',  // B + C channels(GAL, NavIC / IRNSS)
    				  // I + Q channels(GPS, GAL, QZSS, BDS)
	 			      // M + L channels(GPS, QZSS)
				      // D + P channels(GPS, QZSS, BDS)
	Channel_Y = 'Y',  // Y code - based(GPS)
	Channel_Z = 'Z',  // A + B + C channels(GAL)
					  // D + P channels(BDS)
					  // I + Q channels. (QZSS)
				      // D + E channels. (QZSS)
	Channel_Unknown
};
