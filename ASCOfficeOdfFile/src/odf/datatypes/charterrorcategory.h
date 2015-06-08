#pragma once

#include <iosfwd>
#include <string>
#include "odfattributes.h"


namespace cpdoccore { namespace odf_types { 

class chart_error_category
{
public:
    enum type
    {
        none,
        variance,
        standard_deviation,
        percentage,
        error_margin,
        constant
    };

    chart_error_category() {}
    chart_error_category(type _Type) : type_(_Type)   {}
    type get_type() const   {  return type_; };
    static chart_error_category parse(const std::wstring & Str);

private:
    type type_;

};
	std::wostream & operator << (std::wostream & _Wostream, const chart_error_category & _Val);

} 

APPLY_PARSE_XML_ATTRIBUTES(odf_types::chart_error_category);

}
