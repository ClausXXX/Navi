#ifndef SP3_H
#define SP3_H

struct SP3Ephemeris
{
	short Year;
	char Number[2], Month, Day, Hours, Minutes;
	float Seconds;
	double toc, x, y, z, dt;
};

struct SP3
{
	short Year;
	char Type, SatteliteMask,
		 NOfSattelites,
		 System, TimeSystem[4],
		 Month, Day, Hours, Minutes;
	float Version, Step, Seconds;
	int NOfGPSEphemeris, NOfGLOEphemeris, NOfGALEphemeris, NOfBDSEphemeris;
	struct SP3Ephemeris *GPSEphemeris;
	struct SP3Ephemeris *GLOEphemeris;
	struct SP3Ephemeris *GALEphemeris;
	struct SP3Ephemeris *BDSEphemeris;
};

int ReadSP3Ephemeris(char*, struct SP3*);

#endif
