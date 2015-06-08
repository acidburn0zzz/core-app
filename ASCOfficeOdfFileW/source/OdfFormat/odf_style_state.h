#pragma once

#include <string>
#include <vector>

#include "office_elements_create.h"

#include "stylefamily.h"

namespace cpdoccore {
namespace odf_writer {

class style;
class odf_conversion_context;
class odf_style_context;

class style_text_properties;
class style_paragraph_properties;
class style_graphic_properties;
class style_table_properties;
class style_section_properties;
class style_table_cell_properties;
class style_table_row_properties;
class style_table_column_properties;
class style_chart_properties;
//class style_drawing_page_properties;

class odf_style_state;
typedef shared_ptr<odf_style_state>::Type odf_style_state_ptr;

class odf_style_state
{
public:
	odf_style_state(office_element_ptr & elm, const odf_types::style_family::type family );

	void set_automatic(bool val);
	void set_root(bool val);
	void set_default(bool val);
	void set_conditional(bool val);

	office_element_ptr & get_office_element();

	void apply_from(style* other_style);

	void set_dont_write(bool val);

	void add_child(office_element_ptr & child);
/////////////////////////////////////////////////////////////////////////////////////////
	void			set_name(std::wstring & name);
	std::wstring 	get_name();

	odf_types::style_family::type get_family_type();

	void	set_number_format(int id)	{num_fmt_id_ = id;}
	int		get_number_format()			{return num_fmt_id_;}

	void	set_parent_style_name(std::wstring & name) ;	
	void	set_data_style_name(std::wstring & name);
	void	set_list_style_name(std::wstring & name);

	void	set_display_name(std::wstring & name);
//////////////////////////////////////////////////////////////////////////////////////////
    style_text_properties *			get_text_properties();
    style_paragraph_properties *	get_paragraph_properties() ;
    style_graphic_properties *		get_graphic_properties() ;
    style_table_properties *		get_table_properties();
    style_section_properties *		get_section_properties();
    style_table_cell_properties *	get_table_cell_properties();
    style_table_row_properties *	get_table_row_properties() ;
    style_table_column_properties * get_table_column_properties() ;
    style_chart_properties *		get_chart_properties() ;
	//style_drawing_page_properties*	get_drawing_page_properties();

	bool	get_list_style_exist();
	int		get_list_style_level();
	int		get_list_style_id();

	void	set_list_style_exist(bool val);
	void	set_list_style_level(int val);
	void	set_list_style_id(int val);

private:
	std::wstring	odf_style_name_;
	std::wstring	oox_style_name_;

//���� ��� �������� �����
	int				style_oox_id_;
	int				num_fmt_id_; //default =0 (general) 
///////	
	bool automatic_;
	bool root_;
	bool default_; // ��� ��� ��� ������ ��� � root
	bool conditional_; //����� ������������� �� �������

	bool writable_;

	odf_types::style_family::type style_family_;
	
	office_element_ptr	odf_style_;

	friend class odf_style_context;

	bool	list_style_exist;
	int		list_style_level;
	int		list_style_id;
};


}
}

