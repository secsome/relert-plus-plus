#pragma once

#include <Always.h>

// Codes from CnC_Remastered_Collection

class CCRC
{
public:
	CCRC(uint32_t initial = 0);

	uint32_t operator()() const;
	uint32_t operator()(void const* buffer, size_t length);
	uint32_t operator()(CString s);

	void Reset(uint32_t val = 0);

private:
	static uint32_t CRCTable[0x100];

	uint32_t Value;
};