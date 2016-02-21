
#include "CellRef.h"
#include "CellRangeRef.h"

#include "Auxiliary/HelpFunc.h"


namespace XLS
{


CellRef::CellRef()
:	row(0),
	column(0),
	rowRelative(true),
	colRelative(true),
	fQuoted(false)
{
}



CellRef::CellRef(const std::wstring str_ref)
:	fQuoted(false)
{
	fromString(str_ref);
}


CellRef::CellRef(const int row_init, const int column_init, const bool row_relative_init, const bool col_relative_init)
:	row(row_init),
	column(column_init),
	rowRelative(row_relative_init),
	colRelative(col_relative_init),
	fQuoted(false)
{
}


BiffStructurePtr CellRef::clone()
{
	return BiffStructurePtr(new CellRef(*this));
}

const std::wstring CellRef::toString() const
{
	if(to_string_cache.empty())
	{
		to_string_cache = AUX::loc2str(row, rowRelative, column, colRelative);
	}
	return to_string_cache;
}


void CellRef::fromString(const std::wstring& str)
{
	AUX::str2loc(str, row, rowRelative, column, colRelative);
	to_string_cache.clear();
}



CellRef::operator std::wstring  () const
{
	return toString().c_str();
}


const int CellRef::getRow() const
{
	return row;
}
const int CellRef::getColumn() const
{
	return column;
}
const bool CellRef::getRowRelative() const
{
	return rowRelative;
}
const bool CellRef::getColumnRelative() const
{
	return colRelative;
}


void CellRef::operator+=(const CellRef& appended_ref)
{
	row += rowRelative ? appended_ref.getRow() : 0;
	column += colRelative ? appended_ref.getColumn() : 0;
	to_string_cache.clear();
}


void CellRef::operator-=(const CellRef& subtracted_ref)
{
	row -= rowRelative ? subtracted_ref.getRow() : 0;
	column -= colRelative ? subtracted_ref.getColumn() : 0;
	to_string_cache.clear();
}

bool operator==(const CellRef& ref1, const CellRef& ref2)
{
	return (ref1.getRow() == ref2.getRow() && ref1.getColumn() == ref2.getColumn());
}

} // namespace XLS

