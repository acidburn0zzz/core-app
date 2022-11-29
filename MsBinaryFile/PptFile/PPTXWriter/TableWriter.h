/*
 * (c) Copyright Ascensio System SIA 2010-2019
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
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
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

#include "../../../OOXML/PPTXFormat/Logic/GraphicFrame.h"
#include "../Drawing/Elements.h"
#include "ImageManager.h"

class TCell
{
public:
	TCell(PPT_FORMAT::CElementPtr ptrShape, int row, int col, CRelsGenerator* pRels, TCell* pParent = nullptr);

	void FillTc(PPTX::Logic::TableCell &oTc, PPT_FORMAT::CTextCFRun *pLastCF);

	enum eMergeDirection {
		none = 0b00,
		vert = 0b01,
		horz = 0b10,
		hove = (horz | vert)
	};

	enum eBorderPossition
	{
		lnL,
		lnR,
		lnT,
		lnB,
		lnTlToBr,
		lnBlToTr
	};
	void setBorder(eBorderPossition borderPos, PPT_FORMAT::CElementPtr ptrBorder);

	eMergeDirection parentDirection() const;
	int getHeight()const;

	void setPParent(TCell *pParent);
	void setPShape(PPT_FORMAT::CElementPtr ptrShape);

	void setRowSpan(int rowSpan);

	void setGridSpan(int gridSpan);

	bool isRealCell()const;

private:
	void FillTxBody(PPTX::Logic::TxBody &oTxBody, PPT_FORMAT::CTextCFRun *pLastCF);
	void FillTcPr(PPTX::Logic::TableCellProperties& oTcPr);
	void SetTcPrInvisibleBorders(PPTX::Logic::TableCellProperties& oTcPr)const;
	void SetTcPrInvisibleBorder(PPTX::Logic::TableCellProperties& oTcPr, eBorderPossition eBP)const;
	static bool isInvisibleBorder(const PPT_FORMAT::CElementPtr ptrBorder);
	bool isInvisibleBorders() const;
	void FillLn(PPTX::Logic::Ln& Ln, eBorderPossition eBP, PPT_FORMAT::CElementPtr ptrBorder);
	void SetLnName(PPTX::Logic::Ln& Ln, eBorderPossition eBP) const;
	void ApplyLn(PPTX::Logic::TableCellProperties& oTcPr, PPTX::Logic::Ln *pLn, eBorderPossition eBP) const;
	void FillMergeDirection(PPTX::Logic::TableCell& oTc);
	void setParentDirection();

private:
	PPT_FORMAT::CElementPtr m_ptrSpElCell;

	std::map<eBorderPossition, PPT_FORMAT::CElementPtr> m_mapBorders;

	// Proto table's coord
	int m_row, m_col;

	int m_rowSpan;
	int m_gridSpan;

	// Parent
	TCell* m_pParent;
	eMergeDirection m_parentDirection;

	// Common
	CRelsGenerator* m_pRels;
};


typedef std::vector<TCell> ProtoTableRow;
typedef std::vector<ProtoTableRow> MProtoTable;


class ProtoTable
{
public:
	ProtoTable(std::vector<PPT_FORMAT::CElementPtr> &arrCells,
		std::vector<PPT_FORMAT::CElementPtr>& arrSpliters,
		CRelsGenerator* pRels);

	static std::vector<int> getWidth(const std::vector<PPT_FORMAT::CElementPtr> &arrCells, long tableWidth = -1, bool isWidth = true);
	static std::vector<int> getHeight(const std::vector<PPT_FORMAT::CElementPtr> &arrCells, long tableHeight = -1, bool isHeight = true);
	static bool checkRowForZeroHeight(const ProtoTableRow& oRow);
	MProtoTable getTable() const;

private:
	void initProtoTable();
	bool fillProtoTable(std::vector<PPT_FORMAT::CElementPtr> &arrCells, std::vector<PPT_FORMAT::CElementPtr> &arrSpliters);
	bool fillCells(std::vector<PPT_FORMAT::CElementPtr> &arrCells);
	void fillBorders(std::vector<PPT_FORMAT::CElementPtr> &arrSpliters);
	bool findCellPos(const int top, const int left, UINT& posRow, UINT& posCol);
	std::list<TCell*> getParentCellFromTable(const UINT posFRow, const UINT posFCol, const UINT posLRow, const UINT posLCol);
	void setBorders(const UINT posFRow, const UINT posFCol, const UINT posLRow, const UINT posLCol, PPT_FORMAT::CElementPtr ptrBorder);

	bool isDefaultBoard(const PPT_FORMAT::CShapeElement *pBorder);

	std::vector<std::vector<PPT_FORMAT::CShapeElement *> >
		getRows(std::vector<PPT_FORMAT::CShapeElement *> &arrCells);

private:
	std::vector<int> m_arrLeft;
	std::vector<int> m_arrTop;
	MProtoTable m_table;
	CRelsGenerator* m_pRels;
};

class TableWriter
{
public:
	TableWriter(PPT_FORMAT::CTableElement *pTableElement, CRelsGenerator* pRels);

	void Convert(PPTX::Logic::GraphicFrame& oGraphicFrame);

private:
	void FillNvGraphicFramePr(PPTX::Logic::NvGraphicFramePr& oNvGFPr);
	void FillXfrm(PPTX::Logic::Xfrm& oXFRM);
	void FillTable(PPTX::Logic::Table &oTable);

	void FillTblPr(PPTX::Logic::TableProperties& oTblPr);
	void FillTblGrid(std::vector<PPTX::Logic::TableCol>& tblGrid,
		std::vector<PPT_FORMAT::CElementPtr> &arrCells);
	void prepareShapes(std::vector<PPT_FORMAT::CElementPtr> &arrCells,
		std::vector<PPT_FORMAT::CElementPtr> &arrSpliters);

	void FillRow(PPTX::Logic::TableRow& oRow, ProtoTableRow &arrCells);

public:
	std::wstring getXmlForGraphicFrame(int ID, int idx)const;
private:
	void CorrectGraphicFrame(PPTX::Logic::GraphicFrame& oGraphicFrame);
	void FillnvPr(PPTX::Logic::NvPr& oNvPr);

private:
	PPT_FORMAT::CTableElement* m_pTableElement;
	CRelsGenerator* m_pRels;
	nullable<ProtoTable> m_nPTable;
};
