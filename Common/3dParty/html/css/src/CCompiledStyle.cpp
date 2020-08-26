#include "CCompiledStyle.h"

#include <cctype>
#include <vector>
#include <codecvt>

#include <iostream>

namespace NSCSS
{
    CCompiledStyle::CCompiledStyle()
    {
    }

    CCompiledStyle::CCompiledStyle(std::map<std::wstring, std::wstring> mStyle)
    {
        m_mStyle = mStyle;
    }

    CCompiledStyle::CCompiledStyle(const CCompiledStyle& oStyle)
    {
        m_mStyle = oStyle.m_mStyle;
        m_sId = oStyle.m_sId;
        m_arParentsStyles = oStyle.m_arParentsStyles;
    }

    CCompiledStyle::~CCompiledStyle()
    {
    }

    CCompiledStyle& CCompiledStyle::operator+= (const CCompiledStyle &oElement)
    {
        for(auto& item : oElement.m_mStyle)
        {
            if (m_mStyle[item.first] != L"inherit")
                m_mStyle[item.first] = item.second;
            if (item.second.find(L"!important"))
                m_mStyle[item.first] = item.second.substr(0, item.second.find(L"!important"));
        }

        return *this;
    }

    CCompiledStyle& CCompiledStyle::operator-= (const CCompiledStyle &oElement)
    {
        std::map<std::wstring, std::wstring> oStyle;

        for (auto item : m_mStyle)
        {
            if (oElement.m_mStyle.find(item.first) != oElement.m_mStyle.cend())
                oStyle.emplace(item.first, item.second);
        }
        m_mStyle = oStyle;

        return *this;
    }

    CCompiledStyle& CCompiledStyle::operator= (const CCompiledStyle &oElement)
    {
        m_mStyle = oElement.m_mStyle;
        m_sId = oElement.m_sId;
//        m_arParentsStyles = oElement.m_arParentsStyles;
        return *this;
    }


    bool CCompiledStyle::operator==(const CCompiledStyle &oElement)
    {
        std::wstring sThisName = this->GetId();

        if (sThisName.find(L'-') != std::wstring::npos)
            sThisName = sThisName.substr(0, sThisName.find(L'-'));
        if (sThisName.find(L'#') != std::wstring::npos)
            sThisName = sThisName.substr(0, sThisName.find(L'#'));
        if (sThisName.find(L'.') != std::wstring::npos)
            sThisName = sThisName.substr(0, sThisName.find(L'.'));

        std::wstring sElementName = oElement.m_sId;

        if (sElementName.find(L'-') != std::wstring::npos)
            sElementName = sElementName.substr(0, sElementName.find(L'-'));
        if (sElementName.find(L'#') != std::wstring::npos)
            sElementName = sElementName.substr(0, sElementName.find(L'#'));
        if (sElementName.find(L'.') != std::wstring::npos)
            sElementName = sElementName.substr(0, sElementName.find(L'.'));

        if (sThisName != sElementName)
            return false;

        if (this->m_arParentsStyles.size() != oElement.m_arParentsStyles.size())
            return false;

        for (size_t i = 0; i < this->m_arParentsStyles.size(); i++)
            if (this->m_arParentsStyles[i] != oElement.m_arParentsStyles[i])
                return false;

        if (this->m_mStyle.size() != oElement.m_mStyle.size())
            return false;

        auto iterLeft = this->m_mStyle.begin();
        auto iterRight = oElement.m_mStyle.begin();

        while (iterLeft != this->m_mStyle.cend())
        {
            if (iterLeft->first != iterRight->first ||
                iterLeft->second != iterRight->second)
                return false;

            iterLeft++;
            iterRight++;
        }

        return true;
    }

    bool CCompiledStyle::operator!=(const CCompiledStyle &oElement)
    {
        if (*this > oElement)
            return false;

        if (*this < oElement)
            return false;

        return true;
    }

    bool CCompiledStyle::operator>(const CCompiledStyle &oElement)
    {
        return (m_mStyle.size() > oElement.m_mStyle.size());
    }

    bool CCompiledStyle::operator<(const CCompiledStyle &oElement)
    {
        return (m_mStyle.size() < oElement.m_mStyle.size());
    }

    std::map<std::wstring, std::wstring> CCompiledStyle::GetStyleMap()
    {
        return m_mStyle;
    }

    std::wstring CCompiledStyle::GetStyleW()
    {
        std::wstring sStyle;
        for (auto iter = m_mStyle.begin(); iter != m_mStyle.cend(); iter++)
            sStyle += iter->first + L":" + iter->second + L";";
        return sStyle;
    }

    std::string CCompiledStyle::GetStyle()
    {
        std::wstring sStyle = GetStyleW();
        std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
        return converter.to_bytes(sStyle);
    }

    size_t CCompiledStyle::GetSize()
    {
        return m_mStyle.size();
    }

    bool CCompiledStyle::Empty()
    {
        if (m_mStyle.size() == 0)
            return true;

//        if (m_arParentsStyles.size() == 0)
//            return true;

        return false;
    }

    void CCompiledStyle::Clear()
    {
        m_mStyle.clear();
        m_sId.clear();
        m_arParentsStyles.clear();
    }

    std::map<std::wstring, std::wstring>::iterator CCompiledStyle::GetBegin()
    {
        return m_mStyle.begin();
    }

    std::map<std::wstring, std::wstring>::iterator CCompiledStyle::GetEnd()
    {
        return m_mStyle.end();
    }

    void CCompiledStyle::AddPropSel(std::wstring sProperty, std::wstring sValue)
    {
        m_mStyle.emplace(sProperty, sValue);
    }

    void CCompiledStyle::InsertStyle(std::map<std::wstring, std::wstring> mStyle)
    {
        m_mStyle.insert(mStyle.begin(), mStyle.end());
    }

    void CCompiledStyle::SetStyle(std::map<std::wstring, std::wstring> mStyle)
    {
        m_mStyle = mStyle;
    }

    void CCompiledStyle::AddStyle(std::wstring sStyle)
    {
        std::wstring sProperty;
        std::wstring sValue;
        int nPosition = 0;

        while(nPosition < (int)sStyle.length())
        {
            while (nPosition < (int)sStyle.length() && sStyle[nPosition] != L':')
            {
                if (!isspace(sStyle[nPosition]))
                    sProperty += sStyle[nPosition];
                nPosition++;
            }
            nPosition++;
            while (nPosition < (int)sStyle.length() && sStyle[nPosition] != L';' &&
                   sStyle[nPosition] != L'\'' && sStyle[nPosition] != L'"')
            {
//                if (!isspace(sStyle[nPosition]))
                    sValue += sStyle[nPosition];
                nPosition++;
            }
            nPosition++;
            if (!sProperty.empty() && !sValue.empty())
            {
                if (sValue.find(L'!') != std::wstring::npos)
                    sValue = sValue.substr(0, sValue.find(L'!') - 1);
                AddPropSel(sProperty, sValue);
                sProperty.clear();
                sValue.clear();
            }
        }
    }

    void CCompiledStyle::AddParent(std::wstring sParentName)
    {
        if (!sParentName.empty())
            m_arParentsStyles.push_back(sParentName);
    }

    std::vector<std::wstring> CCompiledStyle::GetParentsName()
    {
        return m_arParentsStyles;
    }

    void CCompiledStyle::SetID(std::wstring sId)
    {
        m_sId = sId;
    }

    std::wstring CCompiledStyle::GetId()
    {
        return m_sId;
    }

    /* FONT */

        std::wstring CCompiledStyle::GetFont()
        {
            if (m_mStyle.find(L"font") != m_mStyle.cend())
                return m_mStyle[L"font"];

            std::wstring sValue = GetFontStyle()   + L" " +
                                  GetFontVariant() + L" " +
                                  GetFontWeight()  + L" " +
                                  GetFontSize()    + L"/" +
                                  GetLineHeight()  + L" " +
                                  GetFontFamily();
        }

        std::wstring CCompiledStyle::GetFontFamily()
        {
            if (m_mStyle.find(L"font-family") != m_mStyle.cend())
                return m_mStyle[L"font-family"];

            std::wstring sFont;

            if (m_mStyle.find(L"font") != m_mStyle.cend())
                sFont = m_mStyle[L"font"];

            if (sFont.empty())
                return L"";

            int nPos1;

            for (size_t i = sFont.length() - 1; i >= 0; i--)
            {
                if(isdigit(sFont[i]))
                {
                    nPos1 = i;
                    break;
                }
            }

            std::wstring sValue = sFont.substr(nPos1);

            if(sValue.find(L" ") != std::wstring::npos)
                sValue = sValue.substr(sValue.find(L" ") + 1);

            if (!sValue.empty())
                return sValue;

            return L"";
        }

        std::wstring CCompiledStyle::GetFontSize()
        {
            if (m_mStyle.find(L"font-size") != m_mStyle.cend())
                return m_mStyle[L"font-size"];

            std::wstring sFont;

            if (m_mStyle.find(L"font") != m_mStyle.cend())
                sFont = m_mStyle[L"font"];

            if (sFont.empty())
                return L"medium";

            std::vector<std::wstring> arValues = {  L"xx-small",L"x-small", L"small",
                                                    L"medium",  L"large",   L"x-large",
                                                    L"xx-large",L"larger",  L"smaller"};

            for (size_t i = 0; i < arValues.size(); i++)
                if (sFont.find(arValues[i]))
                    return arValues[i];

            std::vector<std::wstring> arUnitMeasure = {L"mm", L"cm",
                                                       L"in", L"pt",
                                                       L"pc", L"%",
                                                       L"em", L"rem",
                                                       L"ex"};
            int nPos = 0;
            std::wstring sUnitMeasure;

            for (size_t i = 0; i < arUnitMeasure.size(); i++)
            {
                if (sFont.find(arUnitMeasure[i]))
                {
                    nPos = sFont.find(arUnitMeasure[i]);
                    sUnitMeasure = arUnitMeasure[i];
                    if (isdigit(sFont[nPos - 1]) || sFont[nPos - 1] == L'.')
                        break;
                    else
                    {
                        nPos = 0;
                        sUnitMeasure = L"";
                    }
                }
            }

            if (nPos != 0)
            {
                int num = 0;
                std::wstring sValue;
                while ((nPos - num) > 0 &&
                       (isdigit(sFont[nPos - num]) ||
                        sFont[nPos - num] == '.'))
                {
                    sValue = sFont[nPos - num] + sValue;
                    num++;
                }
                if (!sValue.empty())
                    return sValue + sUnitMeasure;
            }

            return L"medium";
        }

        std::wstring CCompiledStyle::GetFontSizeAdjust()
        {
            if (m_mStyle.find(L"font-size-adjust") != m_mStyle.cend())
                return m_mStyle[L"font-size-adjust"];
            return L"";
        }

        std::wstring CCompiledStyle::GetFontStretch()
        {
            if (m_mStyle.find(L"font-stretch") != m_mStyle.cend())
                return m_mStyle[L"font-stretch"];
            return L"";
        }

        std::wstring CCompiledStyle::GetFontStyle()
        {
            if (m_mStyle.find(L"font-style") != m_mStyle.cend())
                return m_mStyle[L"font-style"];

            std::wstring sFont;

            if (m_mStyle.find(L"font") != m_mStyle.cend())
                sFont = m_mStyle[L"font"];

            if (sFont.empty())
                return L"";

            std::vector<std::wstring> arValues = {L"italic", L"oblique"};

            for (size_t i = 0; i < arValues.size(); i++)
                if (sFont.find(arValues[i]))
                    return arValues[i];

            if (isdigit(sFont[0]))
                return L"normal";

            std::wstring sValue;
            if (sFont.find(L" ") != std::wstring::npos)
                sValue = sFont.substr(0, sFont.find(L" "));

            if(!sValue.empty())
                return sValue;

            return L"normal";
        }

        std::wstring CCompiledStyle::GetFontVariant()
        {
            if (m_mStyle.find(L"font-variant") != m_mStyle.cend())
                return m_mStyle[L"font-variant"];

            std::wstring sFont;

            if (m_mStyle.find(L"font") != m_mStyle.cend())
                sFont = m_mStyle[L"font"];

            if (sFont.empty())
                return L"normal";

            if (sFont.find(L"small-caps"))
                return L"small-caps";

            if (isdigit(sFont[0]))
                return L"normal";

            std::wstring sValue;
            if (sFont.find(L" ") != std::wstring::npos && !isdigit(sFont[0]))
                sValue = sFont.substr(sFont.find(L" ") + 1);
            else
                return L"normal";

            if (sValue.find(L" ") != std::wstring::npos && !isdigit(sValue[0]))
                sValue = sValue.substr(0, sFont.find(L" "));
            else
                return L"normal";

            if(!sValue.empty())
                return sValue;

            return L"normal";
        }

        std::wstring CCompiledStyle::GetFontWeight()
        {
            if (m_mStyle.find(L"font-weight") != m_mStyle.cend())
                return m_mStyle[L"font-weight"];

            std::wstring sFont;

            if (m_mStyle.find(L"font") != m_mStyle.cend())
                sFont = m_mStyle[L"font"];

            if (sFont.empty())
                return L"normal";

            std::vector<std::wstring> arValues = {L"bold",   L"bolder",
                                                  L"lighter"};

            for (size_t i = 0; i < arValues.size(); i++)
                if (sFont.find(arValues[i]))
                    return arValues[i];

            if (isdigit(sFont[0]))
                return L"normal";

            std::wstring sValue = sFont;
            if (sValue.find(L" ") != std::wstring::npos && !isdigit(sValue[0]))
                sValue = sValue.substr(sValue.find(L" ") + 1);
            else
                return L"normal";

            if (sValue.find(L" ") != std::wstring::npos && !isdigit(sValue[0]))
                sValue = sValue.substr(sValue.find(L" ") + 1);
            else
                return L"normal";

            if (sValue.find(L" ") != std::wstring::npos && !isdigit(sValue[0]))
                sValue = sValue.substr(0, sValue.find(L" "));
            else
                return L"normal";

            if(!sValue.empty())
                return sValue;

            return L"normal";
        }

        std::wstring CCompiledStyle::GetLineHeight()
        {
            if (m_mStyle.find(L"line-height") != m_mStyle.cend())
                return m_mStyle[L"line-height"];

            std::wstring sFont;

            if (m_mStyle.find(L"font") != m_mStyle.cend())
                sFont = m_mStyle[L"font"];

            if (sFont.empty())
                return L"normal";

            if (sFont.find(L"/") == std::wstring::npos)
                return L"normal";

            int nPos = sFont.find(L"/");
            std::wstring sValue = sFont.substr(nPos + 1);
            if (sValue.find(L" ") != std::wstring::npos)
                sValue = sValue.substr(0, sValue.find(L" ") );

            if (isalpha(sValue[sValue.length() - 1]))
                return sValue;

            nPos = sFont.find(sValue) + sValue.length();
            sValue += L" ";
            while (nPos < (int)sFont.length() &&
                   isalpha(sFont[nPos]))
            {
                sValue += sFont[nPos];
                nPos++;
            }
            return sValue;

        }

        std::vector<std::wstring> CCompiledStyle::GetFontNames()
        {
            std::wstring sFontNames = GetFontFamily();
            std::vector<std::wstring> arFontNames;

            if (sFontNames.empty())
                return arFontNames;

            sFontNames = sFontNames.substr(0, sFontNames.find_last_of(L","));
            std::wstring sTemp;
            for (int i = 0; i < (int)sFontNames.length(); i++)
            {
                if (sFontNames[i] != L',' && sFontNames[i] != L' ')
                    sTemp += sFontNames[i];

                if (sFontNames[i] == L',')
                {
                    arFontNames.push_back(sTemp);
                    sTemp.clear();
                }
            }

            if (!sTemp.empty())
                arFontNames.push_back(sTemp);

            return arFontNames;
        }

    /* MARGIN */
        std::wstring CCompiledStyle::GetMargin()
        {
            if (m_mStyle.find(L"margin") != m_mStyle.cend())
                return m_mStyle[L"margin"];

            std::wstring sValue;

            std::wstring sTop    = GetMarginTop();
            std::wstring sLeft   = GetMarginLeft();
            std::wstring sRight  = GetMarginRight();
            std::wstring sBottom = GetMarginBottom();

            if ((sTop == sLeft) && (sLeft == sRight) && (sRight == sBottom))
                return sTop;

            if ((sTop == sBottom) && (sLeft == sRight))
                return  sTop + L" " + sLeft;

            if (sLeft == sRight)
                return sTop + L" " + sLeft + L" " + sBottom;

            return sTop + L" " + sRight + L" " + sBottom + L" " + sLeft;

            return L"auto";
        }

        std::wstring CCompiledStyle::GetMarginTop()
        {
            if (m_mStyle.find(L"margin-top") != m_mStyle.cend())
                return m_mStyle[L"margin-top"];

            std::wstring sValue;
            if (m_mStyle.find(L"margin") != m_mStyle.cend())
                sValue = m_mStyle[L"margin"];

            if (sValue.empty())
                return L"auto";

            std::vector<std::wstring> arValues;

            std::wstring sTemp;
            for (int  i = 0; i < (int)sValue.length(); i++)
            {
                sTemp += sValue[i];

                if (sValue[i] == L' ')
                {
                    arValues.push_back(sTemp);
                    sTemp.clear();
                }
            }
            if (!sTemp.empty())
                arValues.push_back(sTemp);

            if (arValues.size() != 0)
                return arValues[0];

            return L"auto";
        }

        std::wstring CCompiledStyle::GetMarginLeft()
        {
            if (m_mStyle.find(L"margin-left") != m_mStyle.cend())
                return m_mStyle[L"margin-left"];

            std::wstring sValue;
            if (m_mStyle.find(L"margin") != m_mStyle.cend())
                sValue = m_mStyle[L"margin"];

            if (sValue.empty())
                return L"auto";

            std::vector<std::wstring> arValues;

            std::wstring sTemp;
            for (int  i =0; i < (int)sValue.length(); i++)
            {
                sTemp += sValue[i];

                if (sValue[i] == L' ')
                {
                    arValues.push_back(sTemp);
                    sTemp.clear();
                }
            }
            if (!sTemp.empty())
                arValues.push_back(sTemp);

            if (arValues.size() == 4)
                return arValues[3];
            else if (arValues.size() > 1)
                return arValues[1];
            else if (arValues.size() == 1)
                return arValues[0];

            return L"auto";
        }

        std::wstring CCompiledStyle::GetMarginRight()
        {
            if (m_mStyle.find(L"margin-right") != m_mStyle.cend())
                return m_mStyle[L"margin-right"];

            std::wstring sValue;
            if (m_mStyle.find(L"margin") != m_mStyle.cend())
                sValue = m_mStyle[L"margin"];

            if (sValue.empty())
                return L"auto";

            std::vector<std::wstring> arValues;

            std::wstring sTemp;
            for (int  i =0; i < (int)sValue.length(); i++)
            {
                sTemp += sValue[i];

                if (sValue[i] == L' ')
                {
                    arValues.push_back(sTemp);
                    sTemp.clear();
                }
            }
            if (!sTemp.empty())
                arValues.push_back(sTemp);

            if (arValues.size() > 1)
                return arValues[1];
            else if (arValues.size() == 1)
                return arValues[0];

            return L"auto";
        }

        std::wstring CCompiledStyle::GetMarginBottom()
        {
            if (m_mStyle.find(L"margin-bottom") != m_mStyle.cend())
                return m_mStyle[L"margin-bottom"];

            std::wstring sValue;
            if (m_mStyle.find(L"margin") != m_mStyle.cend())
                sValue = m_mStyle[L"margin"];

            if (sValue.empty())
                return L"auto";

            std::vector<std::wstring> arValues;

            std::wstring sTemp;
            for (int  i =0; i < (int)sValue.length(); i++)
            {
                sTemp += sValue[i];

                if (sValue[i] == L' ')
                {
                    arValues.push_back(sTemp);
                    sTemp.clear();
                }
            }
            if (!sTemp.empty())
                arValues.push_back(sTemp);

            if (arValues.size() > 2)
                return arValues[2];
            else if (arValues.size() > 0)
                return arValues[0];

            return L"auto";
        }

    /* PADDING */
        std::wstring CCompiledStyle::GetPadding()
        {
            if (m_mStyle.find(L"padding") != m_mStyle.cend())
                return m_mStyle[L"padding"];

            std::wstring sValue;

            std::wstring sTop    = GetPaddingTop();
            std::wstring sLeft   = GetPaddingLeft();
            std::wstring sRight  = GetPaddingRight();
            std::wstring sBottom = GetPaddingBottom();

            if ((sTop == sLeft) && (sLeft == sRight) && (sRight == sBottom))
                return sTop;

            if ((sTop == sBottom) && (sLeft == sRight))
                return  sTop + L" " + sLeft;

            if (sLeft == sRight)
                return sTop + L" " + sLeft + L" " + sBottom;

            return sTop + L" " + sRight + L" " + sBottom + L" " + sLeft;
        }

        std::wstring CCompiledStyle::GetPaddingTop()
        {
            if (m_mStyle.find(L"padding-top") != m_mStyle.cend())
                return m_mStyle[L"padding-top"];

            std::wstring sValue;
            if (m_mStyle.find(L"padding") != m_mStyle.cend())
                sValue = m_mStyle[L"padding"];

            if (sValue.empty())
                return L"auto";

            std::vector<std::wstring> arValues;

            std::wstring sTemp;
            for (int  i =0; i < (int)sValue.length(); i++)
            {
                sTemp += sValue[i];

                if (sValue[i] == L' ')
                {
                    arValues.push_back(sTemp);
                    sTemp.clear();
                }
            }
            if (!sTemp.empty())
                arValues.push_back(sTemp);

            if (arValues.size() != 0)
                return arValues[0];

            return L"auto";
        }

        std::wstring CCompiledStyle::GetPaddingLeft()
        {
            if (m_mStyle.find(L"padding-left") != m_mStyle.cend())
                return m_mStyle[L"padding-left"];

            std::wstring sValue;
            if (m_mStyle.find(L"padding") != m_mStyle.cend())
                sValue = m_mStyle[L"padding"];

            if (sValue.empty())
                return L"auto";

            std::vector<std::wstring> arValues;

            std::wstring sTemp;
            for (int  i =0; i < (int)sValue.length(); i++)
            {
                sTemp += sValue[i];

                if (sValue[i] == L' ')
                {
                    arValues.push_back(sTemp);
                    sTemp.clear();
                }
            }
            if (!sTemp.empty())
                arValues.push_back(sTemp);

            if (arValues.size() == 4)
                return arValues[3];
            else if (arValues.size() > 1)
                return arValues[1];
            else if (arValues.size() == 1)
                return arValues[0];

            return L"auto";
        }

        std::wstring CCompiledStyle::GetPaddingRight()
        {
            if (m_mStyle.find(L"padding-right") != m_mStyle.cend())
                return m_mStyle[L"padding-right"];

            std::wstring sValue;
            if (m_mStyle.find(L"padding") != m_mStyle.cend())
                sValue = m_mStyle[L"padding"];

            if (sValue.empty())
                return L"auto";

            std::vector<std::wstring> arValues;

            std::wstring sTemp;
            for (int  i =0; i < (int)sValue.length(); i++)
            {
                sTemp += sValue[i];

                if (sValue[i] == L' ')
                {
                    arValues.push_back(sTemp);
                    sTemp.clear();
                }
            }
            if (!sTemp.empty())
                arValues.push_back(sTemp);

            if (arValues.size() > 1)
                return arValues[1];
            else if (arValues.size() == 1)
                return arValues[0];

            return L"auto";
        }

        std::wstring CCompiledStyle::GetPaddingBottom()
        {
            if (m_mStyle.find(L"padding-bottom") != m_mStyle.cend())
                return m_mStyle[L"padding-bottom"];

            std::wstring sValue;
            if (m_mStyle.find(L"padding") != m_mStyle.cend())
                sValue = m_mStyle[L"padding"];

            if (sValue.empty())
                return L"auto";

            std::vector<std::wstring> arValues;

            std::wstring sTemp;
            for (int  i =0; i < (int)sValue.length(); i++)
            {
                sTemp += sValue[i];

                if (sValue[i] == L' ')
                {
                    arValues.push_back(sTemp);
                    sTemp.clear();
                }
            }
            if (!sTemp.empty())
                arValues.push_back(sTemp);

            if (arValues.size() > 2)
                return arValues[2];
            else if (arValues.size() > 0)
                return arValues[0];

            return L"auto";
        }

    /*  SPACING */
        std::wstring CCompiledStyle::GetLetterSpacing()
        {
            if (m_mStyle.find(L"letter-spacing") != m_mStyle.cend())
                return m_mStyle[L"letter-spacing"];
            return L"normal";
        }

        std::wstring CCompiledStyle::GetWordSpacing()
        {
            if (m_mStyle.find(L"word-spacing") != m_mStyle.cend())
                return m_mStyle[L"word-spacing"];
            return L"normal";
        }

        std::wstring CCompiledStyle::GetBorderSpacing()
        {
            if (m_mStyle.find(L"border-spacing") != m_mStyle.cend())
                return m_mStyle[L"border-spacing"];
            return L"";
        }

    /* COLOR */
        std::wstring CCompiledStyle::GetTextDecorationColor()
        {
            if (m_mStyle.find(L"text-decoration-color") != m_mStyle.cend())
                return m_mStyle[L"text-decoration-color"];
            return L"";
        }

        std::wstring CCompiledStyle::GetBackgroundColor()
        {
            if (m_mStyle.find(L"background-color") != m_mStyle.cend())
                return m_mStyle[L"background-color"];

            std::wstring sBackground = GetBackground();
            if (!sBackground.empty() && sBackground.find(L'#') != std::wstring::npos)
            {
                return sBackground.substr(sBackground.find(L"#"), sBackground.find(sBackground.find(L"#"), L' '));
            }
            return L"";
        }

        std::wstring CCompiledStyle::GetColor()
        {
            if (m_mStyle.find(L"color") != m_mStyle.cend())
                return m_mStyle[L"color"];
            return L"";
        }

        std::wstring CCompiledStyle::GetBorderBottomColor()
        {
            if (m_mStyle.find(L"border-bottom-color") != m_mStyle.cend())
                return m_mStyle[L"border-bottom-color"];
            return L"";
        }

        std::wstring CCompiledStyle::GetBorderLeftColor()
        {
            if (m_mStyle.find(L"border-left-color") != m_mStyle.cend())
                return m_mStyle[L"border-left-color"];
            return L"";
        }

        std::wstring CCompiledStyle::GetBorderRightColor()
        {
            if (m_mStyle.find(L"border-right-color") != m_mStyle.cend())
                return m_mStyle[L"border-right-color"];
            return L"";
        }

        std::wstring CCompiledStyle::GetOutlineColor()
        {
            if (m_mStyle.find(L"outline-color") != m_mStyle.cend())
                return m_mStyle[L"outline-color"];
            return L"";
        }

        std::wstring CCompiledStyle::GetColumnRuleColor()
        {
            if (m_mStyle.find(L"column-rule-color") != m_mStyle.cend())
                return m_mStyle[L"column-rule-color"];
            return L"";
        }

        std::wstring CCompiledStyle::GetBackground()
        {
            std::wstring sBackground;
            if (m_mStyle.find(L"background") != m_mStyle.cend())
                return  m_mStyle[L"background"];

            return L"";
        }

    /* TEXT */
        std::wstring CCompiledStyle::GetTextAlign()
        {
            if (m_mStyle.find(L"text-align") != m_mStyle.cend())
                return m_mStyle[L"text-align"];
            return L"";
        }

        std::wstring CCompiledStyle::GetTextIndent()
        {
            if (m_mStyle.find(L"text-indent") != m_mStyle.cend())
                return m_mStyle[L"text-indent"];
            return L"";
        }

        std::wstring CCompiledStyle::GetTextDecoration()
        {
            if (m_mStyle.find(L"text-decoration") != m_mStyle.cend())
                return m_mStyle[L"text-decoration"];
            return L"";
        }

    /* BORDER */

        std::wstring CCompiledStyle::GetBorder()
        {
            if (m_mStyle.find(L"border") != m_mStyle.cend())
                return m_mStyle[L"border"];

            return L"";
        }

        std::wstring CCompiledStyle::GetBorderWidth()
        {
            if (m_mStyle.find(L"border-width") != m_mStyle.cend())
                return m_mStyle[L"border-width"];

            return L"";
        }

        std::wstring CCompiledStyle::GetBorderStyle()
        {
            if (m_mStyle.find(L"border-style") != m_mStyle.cend())
                return m_mStyle[L"border-style"];

            return L"";
        }

        std::wstring CCompiledStyle::GetBorderColor()
        {
            if (m_mStyle.find(L"border-color") != m_mStyle.cend())
                return m_mStyle[L"border-color"];

            return L"";
        }

}
