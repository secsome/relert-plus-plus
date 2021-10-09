#pragma once

#include <Always.h>

// Codes from CnC_Remastered_Collection

class CCRC 
{
public:
	CCRC(int initial = 0);

	int operator() () const { return(Value()); };
	void operator() (char datum);
	int operator() (void const* buffer, int length);
	operator int() const { return(Value()); };

protected:
	int Value() const;

	int CRC;
	int Index;
	union 
	{
		long Composite;
		char Buffer[sizeof(int)];
	} StagingBuffer;
};