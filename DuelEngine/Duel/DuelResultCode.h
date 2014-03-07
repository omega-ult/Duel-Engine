//  [9/9/2013 OMEGA] created


typedef	int DResultCode;

enum 
{
	DRC_Ok					= 0x0000,
	DRC_Error				= 0X0001,
	DRC_InvalidParameter	= 0x0002,
	DRC_IOFailedOpen		= 0x0003,
	DRC_IOFailedRead		= 0x0004,
	DRC_IOFailedWrite		= 0x0005,
	DRC_FileIncomplete		= 0x0006,
	DRC_FileCorrupted		= 0x0007

};