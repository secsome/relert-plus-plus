#include <CRGBClass.h>

#include <CHSVClass.h>

CRGBClass::CRGBClass()
{
	R = G = B = 0;
}

CRGBClass::CRGBClass(byte r, byte g, byte b)
{
    R = r;
    G = g;
    B = b;
}

CRGBClass::operator CHSVClass() const
{
	int H = 0, S = 0, V = 0;
	int white = std::min(R, G, B);

	if (V = std::max(R, G, B))
		S = ((V - white) * 255) / V;

	if (S != 0)
	{
		unsigned int tmp = V - white;
		unsigned int r1 = ((V - R) * 255) / tmp;
		unsigned int g1 = ((V - G) * 255) / tmp;
		unsigned int b1 = ((V - B) * 255) / tmp;

		if (V == R)
			if (white == G)
				tmp = 5 * 256 + b1;
			else
				tmp = 1 * 256 - g1;
		else if (V == G)
			if (white == B)
				tmp = 1 * 256 + r1;
			else
				tmp = 3 * 256 - b1;
		else
			if (white == R)
				tmp = 3 * 256 + g1;
			else
				tmp = 5 * 256 - r1;
		H = tmp / 6;
	}

	return { (byte)H, (byte)S, (byte)V };
}
