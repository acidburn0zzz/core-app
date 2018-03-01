﻿/*
 * (c) Copyright Ascensio System SIA 2010-2018
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */

#include "tabletype.h"

#include <boost/algorithm/string.hpp>
#include <ostream>

namespace cpdoccore { namespace odf_types { 

std::wostream & operator << (std::wostream & _Wostream, const table_type & _Val)
{
    switch(_Val.get_type())
    {
    case table_type::none:
        _Wostream << L"none";
        break;
    case table_type::member_difference:
        _Wostream << L"member-difference";
        break;
    case table_type::member_percentage:
        _Wostream << L"member-percentage";
        break;
    case table_type::member_percentage_difference:
        _Wostream << L"member-percentage-difference";
        break;
    case table_type::running_total:
        _Wostream << L"running-total";
        break;
    case table_type::row_percentage:
        _Wostream << L"row-percentage";
        break;
    case table_type::column_percentage:
        _Wostream << L"column-percentage";
        break;
    case table_type::total_percentage:
        _Wostream << L"total-percentage";
        break;
    default:
        _Wostream << L"index";
        break;
    }
    return _Wostream;    
}

table_type table_type::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"none")
        return table_type( none );
    else if (tmp == L"member-difference")
        return table_type( member_difference );
    else if (tmp == L"member-percentage")
        return table_type( member_percentage );
    else if (tmp == L"member-percentage-difference")
        return table_type( member_percentage_difference );
    else if (tmp == L"running-total")
        return table_type( running_total );
    else if (tmp == L"row-percentage")
        return table_type( row_percentage );
    else if (tmp == L"column-percentage")
        return table_type( column_percentage );
	else if (tmp == L"total-percentage")
        return table_type( total_percentage );
	else
    {
        return table_type( index );
    }
}

} }
