#pragma once

#include "OfficeArtRecord.h"
#include "../BitMarkedStructs.h"

namespace XLS
{
	class CFRecord;
}


namespace ODRAW
{

class OfficeArtClientAnchorSheet : public OfficeArtRecord
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(OfficeArtClientAnchorSheet)
public:
	OfficeArtClientAnchorSheet();
	XLS::BiffStructurePtr clone();

	static const XLS::ElementType	type = XLS::typeOfficeArtClientAnchorSheet;
	
	virtual void loadFields(XLS::CFRecord& record);
	virtual void storeFields(XLS::CFRecord& record);

	int serialize(std::wostream & stream);

	bool fMove;
	bool fSize;

	XLS::Col256U	colL;
	short			dxL;
	XLS::RwU		rwT;
	short			dyT;
	XLS::Col256U	colR;
	short			dxR;
	XLS::RwU		rwB;
	short			dyB;

	int column;
	int row;

	_UINT32		_dxL;
	_UINT32		_dyT;
	_UINT32		_dxR;
	_UINT32		_dyB;

	int _x;
	int _y;
	int _cx;
	int _cy;
};

typedef boost::shared_ptr<OfficeArtClientAnchorSheet> OfficeArtClientAnchorSheetPtr;

class OfficeArtChildAnchor : public OfficeArtRecord
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(OfficeArtChildAnchor)
public:
	OfficeArtChildAnchor();
	XLS::BiffStructurePtr clone();

	static const XLS::ElementType	type = XLS::typeOfficeArtChildAnchor;
	
	virtual void loadFields(XLS::CFRecord& record);
	virtual void storeFields(XLS::CFRecord& record){}

	int serialize(std::wostream & stream);

	_INT32 dxL;
	_INT32 dyT;
	_INT32 dxR;
	_INT32 dyB;

	int _x;
	int _y;
	int _cx;
	int _cy;
};

typedef boost::shared_ptr<OfficeArtChildAnchor> OfficeArtChildAnchorPtr;
} // namespace XLS
