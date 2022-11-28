#include "WideColorStruct.h"


void PPT_FORMAT::WideColorStruct::ReadFromStream(POLE::Stream *pStream)
{
    m_nRed      = StreamUtils::ReadWORD(pStream);
    m_nGreen    = StreamUtils::ReadWORD(pStream);
    m_nBlue     = StreamUtils::ReadWORD(pStream);
}
