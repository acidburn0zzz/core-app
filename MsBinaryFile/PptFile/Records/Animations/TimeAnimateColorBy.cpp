#include "TimeAnimateColorBy.h"


unsigned long TimeAnimateColorBy::FRGB(BYTE alpha)
{
    return ((component2 <<  16) |	(component1 << 8) | (component0 <<   0) | (alpha << 24));
}
