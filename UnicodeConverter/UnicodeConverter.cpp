﻿#include "./UnicodeConverter.h"

#include "unicode/utypes.h"   /* Basic ICU data types */
#include "unicode/ucnv.h"     /* C   Converter API    */

#include "../DesktopEditor/common/File.h"

namespace NSUnicodeConverter
{
    class CUnicodeConverter_Private
    {
    public:
        CUnicodeConverter_Private()
        {
#if 0
            std::wstring sDumpPath = NSFile::GetProcessDirectory() + L"/codepages.txt";
            NSFile::CFileBinary oFile;
            oFile.CreateFileW(sDumpPath);

            int32_t count = ucnv_countAvailable();

            for (int i = 0; i < count; ++i)
            {
                std::string sCodePage = ucnv_getAvailableName(i);

                UErrorCode _error = U_ZERO_ERROR;
                int nCountAliases = ucnv_countAliases(sCodePage.c_str(), &_error);

                char** palices = new char*[nCountAliases];
                ucnv_getAliases(sCodePage.c_str(), (const char**)palices, &_error);

                oFile.WriteStringUTF8(L"-----------------------------\r\n");
                oFile.WriteFile((BYTE*)sCodePage.c_str(), sCodePage.length());
                oFile.WriteStringUTF8(L"\r\n");

                for (int j = 0; j < nCountAliases; ++j)
                {
                    oFile.WriteFile((BYTE*)palices[j], strlen(palices[j]));
                    oFile.WriteStringUTF8(L"\r\n");
                }
            }

            oFile.CloseFile();
#endif
        }
        ~CUnicodeConverter_Private()
        {
        }

    public:
        std::string fromUnicode(const wchar_t* sInput, const unsigned int& nInputLen, const char* converterName)
        {
            std::string sRes = "";
            UErrorCode status = U_ZERO_ERROR;
            UConverter* conv = ucnv_open(converterName, &status);
            if (U_SUCCESS(status))
            {
                int32_t nUCharCapacity = (int32_t)nInputLen;// UTF-16 uses 2 code-points per char
                UChar* pUChar = (UChar*)malloc(nUCharCapacity * sizeof(UChar));
                const UChar* pUCharStart = pUChar;
                int32_t nUCharLength = 0;
                u_strFromWCS(pUChar, nUCharCapacity, &nUCharLength, sInput, nInputLen, &status);
                if (U_SUCCESS(status))
                {
                    const UChar* pUCharLimit = pUCharStart + nUCharLength;
                    sRes.resize(nUCharLength * ucnv_getMaxCharSize(conv));// UTF-16 uses 2 code-points per char
                    char *sResStart = &sRes[0];
                    char *sResCur = sResStart;
                    const char *sResLimit = sResCur + sRes.size();
                    ucnv_fromUnicode(conv, &sResCur, sResLimit, &pUCharStart, pUCharLimit, NULL, TRUE, &status);
                    sRes.resize(sResCur - sResStart);
                }
                ucnv_close(conv);
            }
            return sRes;
        }

        std::string fromUnicode(const std::wstring& sInput, const char* converterName)
        {
            return fromUnicode(sInput.c_str(), (unsigned int)sInput.size(), converterName);
        }

        std::wstring toUnicode(const char* sInput, const unsigned int& nInputLen, const char* converterName)
        {
            std::wstring sRes = L"";
            UErrorCode status = U_ZERO_ERROR;
            UConverter* conv = ucnv_open(converterName, &status);

            std::string sss = ucnv_getName(conv, &status);
            int iii = ucnv_getCCSID(conv, &status);

            //UConverter* conv = ucnv_openCCSID(5347, UCNV_IBM, &status);
            if (U_SUCCESS(status))
            {
                const char* source = sInput;
                const char* sourceLimit = source + nInputLen;

                unsigned int uBufSize = (nInputLen / ucnv_getMinCharSize(conv));
                UChar* targetStart = (UChar*)malloc(uBufSize * sizeof(UChar));
                UChar* target = targetStart;
                UChar* targetLimit = target + uBufSize;

                ucnv_toUnicode(conv, &target, targetLimit, &source, sourceLimit, NULL, TRUE, &status);
                if (U_SUCCESS(status))
                {
                    unsigned int nTargetSize = target - targetStart;
                    sRes.resize(nTargetSize * 2);// UTF-16 uses 2 code-points per char
                    int32_t nResLen = 0;
                    u_strToWCS(&sRes[0], sRes.size(), &nResLen, targetStart, nTargetSize, &status);
                    sRes.resize(nResLen);
                }
                ucnv_close(conv);
            }
            return sRes;
        }

        inline std::wstring toUnicode(const std::string& sInput, const char* converterName)
        {
            return toUnicode(sInput.c_str(), (unsigned int)sInput.size(), converterName);
        }
    };
}

namespace NSUnicodeConverter
{
    CUnicodeConverter::CUnicodeConverter()
    {
        m_pInternal = new CUnicodeConverter_Private();
    }
    CUnicodeConverter::~CUnicodeConverter()
    {
        delete m_pInternal;
    }

    std::string CUnicodeConverter::fromUnicode(const wchar_t* sInput, const unsigned int& nInputLen, const char* converterName)
    {
        return m_pInternal->fromUnicode(sInput, nInputLen, converterName);
    }
    std::string CUnicodeConverter::fromUnicode(const std::wstring &sSrc, const char *sCodePage)
    {
        return m_pInternal->fromUnicode(sSrc, sCodePage);
    }

    std::wstring CUnicodeConverter::toUnicode(const char* sInput, const unsigned int& nInputLen, const char* converterName)
    {
        return m_pInternal->toUnicode(sInput, nInputLen, converterName);
    }
    std::wstring CUnicodeConverter::toUnicode(const std::string &sSrc, const char *sCodePage)
    {
        return m_pInternal->toUnicode(sSrc, sCodePage);
    }
}
