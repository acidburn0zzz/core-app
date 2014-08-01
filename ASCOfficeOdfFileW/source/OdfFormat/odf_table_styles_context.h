#pragma once

#include <string>
#include <vector>

#include "office_elements.h"
#include "office_elements_create.h"


namespace cpdoccore {
namespace odf {

class style_table_cell_properties;
class style_text_properties;
class style_paragraph_properties;
class style_table_properties;

//typedef shared_ptr<style_text_properties>::Type			style_text_properties_ptr;
//typedef shared_ptr<style_paragraph_properties>::Type	style_paragraph_properties_ptr;
//typedef shared_ptr<style_table_cell_properties>::Type	style_table_cell_properties_ptr;
//row, column ???

struct _style_properties
{
	_style_properties(){is = false;}
	office_element_ptr	table_props;
	office_element_ptr	text_props;
	office_element_ptr	paragraph_props;
	office_element_ptr	table_cell_props;

	bool is;
};
struct table_format_state
{
	std::wstring style_name;
	
	_style_properties table_;

	_style_properties band1Horz_;
	_style_properties band1Vert_;
	_style_properties band2Horz_;
	_style_properties band2Vert_;
	_style_properties firstCol_;
	_style_properties firstRow_;
	_style_properties lastCol_;
	_style_properties lastRow_;
	_style_properties neCell_;
	_style_properties nwCell_;
	_style_properties seCell_;
	_style_properties swCell_;
	_style_properties wholeTable_; //???
};

class odf_table_styles_context
{
public:
	odf_table_styles_context(){current = NULL; context_ = NULL; }
	
	void set_odf_context(odf_conversion_context * Context)
	{
		context_ = Context;
	}

//-----------------------------------------------
// input
//-----------------------------------------------

	void start_style(std::wstring styale_name);
	void end_style();

	void add_band1Horz();
	void add_band1Vert();
	void add_band2Horz();
	void add_band2Vert();
	void add_firstCol();
	void add_firstRow();
	void add_lastCol();
	void add_lastRow();
	void add_neCell();
	void add_nwCell();
	void add_seCell();
	void add_swCell();
	void add_wholeTable(); 

	style_table_cell_properties *get_table_cell_properties();
	style_paragraph_properties	*get_paragraph_properties();
	style_text_properties		*get_text_properties();
	style_table_properties		*get_table_properties();

//-----------------------------------------------
// output
//-----------------------------------------------
	bool start_table(std::wstring style_name);
		void set_current_dimension(int col, int row);
		void set_flags(int val);

		void get_table_properties		(					style_table_properties		*props);
		void get_table_cell_properties	(int col, int row,	style_table_cell_properties *props);
		void get_text_properties		(int col, int row,	style_text_properties		*props);
		void get_paragraph_properties	(int col, int row,	style_paragraph_properties	*props);
	void end_table();

private:

	std::vector<table_format_state> table_format_array_;

	_style_properties *current;

	odf_conversion_context *context_;

	struct _use_style
	{
		int table_style_;
		int table_col_count_;
		int table_row_count_;

		bool cols;
		bool rows;
		bool first_row;
		bool last_row;
		bool first_col;
		bool last_col;
	};

	std::vector<_use_style> current_used_;

//////////////////


};
}
}