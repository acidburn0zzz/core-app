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
#pragma once

#include <iosfwd>
#include <iostream>
#include <cpdoccore/CPScopedPtr.h>
#include <cpdoccore/CPOptional.h>
#include <cpdoccore/xml/attributes.h>

#include "oox_drawing.h"
#include "mediaitems.h"
#include "../odf/style_graphic_properties.h"

namespace cpdoccore {
namespace oox {

class _docx_drawing : public _oox_drawing
{ 
public:
	_docx_drawing() : _oox_drawing(), parallel(0), isInline(false), number_wrapped_paragraphs(0), posOffsetV(0), posOffsetH(0)
	{
	}
	bool isInline;
    
	unsigned int parallel;
	
	//_CP_OPT(run_through)				styleRunThrough	;
	_CP_OPT(odf_types::horizontal_rel)	styleHorizontalRel;
    _CP_OPT(odf_types::horizontal_pos)	styleHorizontalPos;
    _CP_OPT(odf_types::vertical_pos)	styleVerticalPos;
    _CP_OPT(odf_types::vertical_rel)	styleVerticalRel;

	_CP_OPT(odf_types::style_wrap)		styleWrap;

	int number_wrapped_paragraphs;

    std::wstring relativeHeight;
    std::wstring behindDoc;

	int posOffsetV;
	int posOffsetH;

	_CP_OPT(int) pctWidth;
	_CP_OPT(int) pctHeight;
  
	int margin_rect[4];		//0-left, 1 -top, 2- right, 3 - bottom

	std::wstring content_group_;

	void serialize_text	(std::wostream & strm);
    void serialize		(std::wostream & strm/*, bool insideOtherDrawing*/);    
};
}
}
