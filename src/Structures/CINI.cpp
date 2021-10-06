#include <CINI.h>

bool IndexBasedComparator::operator()(const CString& s1, const CString& s2)
{
    const int l1 = s1.GetLength();
    const int l2 = s2.GetLength();
    return l1 < l2 || l1 == l2 && strcmp(s1, s2) < 0;
}
