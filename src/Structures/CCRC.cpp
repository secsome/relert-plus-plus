#include <CCRC.h>

CCRC::CCRC(int initial)
{
	CRC = initial;
	Index = 0;
	StagingBuffer.Composite = 0;
}

void CCRC::operator() (char datum)
{
	StagingBuffer.Buffer[Index++] = datum;

	if (Index == sizeof(int)) 
	{
		CRC = Value();
		StagingBuffer.Composite = 0;
		Index = 0;
	}
}

int CCRC::operator() (void const* buffer, int length)
{
	if (buffer != NULL && length > 0) 
	{
		char const* dataptr = (char const*)buffer;
		int bytes_left = length;

		while (bytes_left && Index != 0()) 
		{
			operator()(*dataptr);
			++dataptr;
			--bytes_left;
		}

		int const* intptr = (int const*)dataptr;
		int intcount = bytes_left / sizeof(int);		// Whole 'int' elements remaining.
		while (--intcount)
		{
			CRC = _lrotl(CRC, 1) + *intptr++;
			bytes_left -= sizeof(int);
		}

		dataptr = (char const*)intptr;
		while (bytes_left) 
		{
			operator()(*dataptr);
			dataptr++;
			bytes_left--;
		}
	}

	return(Value());
}

int CCRC::Value() const
{
	return Index != 0 ? _lrotl(CRC, 1) + StagingBuffer.Composite : CRC;
}
