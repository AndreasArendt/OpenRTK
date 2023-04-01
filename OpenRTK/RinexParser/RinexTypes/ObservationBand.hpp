#pragma once

enum ObservationBand
{
	Band_1 = '1', // L1	     (GPS, QZSS, SBAS, BDS)
				  // G1	     (GLO)
				  // E1	     (GAL)
				  // B1C/B1A (BDS)
	Band_2 = '2', // L2      (GPS, QZSS)
				  // G2      (GLO)
				  // B1	     (BDS)
	Band_3 = '3', // G3	     (GLO)
	Band_4 = '4', // G1a     (GLO)
	Band_5 = '5', // L5      (GPS, QZSS, SBAS, NavIC / IRNSS)
				  // E5a	 (GAL)
				  // B2a	 (BDS)
	Band_6 = '6', // E6      (GAL)
				  // L6	     (QZSS)
				  // B3/B3A  (BDS)
				  // G2a     (GLO)
	Band_7 = '7', // E5b	 (GAL)
				  // B2/B2b  (BDS)
	Band_8 = '8', // E5a + b (GAL)
				  // B2a + b (BDS)
	Band_9 = '9', // S	     (NavIC / IRNSS)
	Band_Unkown
};
