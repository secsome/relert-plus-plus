#include <CHSVClass.h>

#include <CRGBClass.h>

CHSVClass::CHSVClass()
{
	H = S = V = 0;
}

CHSVClass::CHSVClass(byte h, byte s, byte v)
{
    H = h;
    S = s;
    V = v;
}

CHSVClass::operator CRGBClass() const
{
	if (S == 0u)
		return { V, V, V };

	byte region = H / 43;
	byte remainder = (H - (region * 43)) * 6;

	byte p = (V * (255 - S)) >> 8;
	byte q = (V * (255 - ((S * remainder) >> 8))) >> 8;
	byte t = (V * (255 - ((S * (255 - remainder)) >> 8))) >> 8;

	switch (region) {
	case 0:
		return { V, t, p };
	case 1:
		return { q, V, p };
	case 2:
		return { p, V, t };
	case 3:
		return { p, q, V };
	case 4:
		return { t, p, V };
	default:
		return { V, p, q };
	}
}
