// adXL.cpp

#include "adXL.h"
#include <exception>
using namespace std;
using namespace FreeXL;

/////////////////////////////////////////////////////////////////////////////////////
// global varibles
map<string, xlCell::SS_TYPE> xlCell::SS_TYPE_MAP::StrToType;
map<xlCell::SS_TYPE, string> xlCell::SS_TYPE_MAP::TypeToStr;
bool xlCell::SS_TYPE_MAP::_initialized = false;

xlCell::SS_TYPE_MAP::SS_TYPE_MAP()
{
	if (!_initialized)
	{
		// 在这里添加字符串到类型的映射
		StrToType["String"] = xlCell::T_String;
		StrToType["Number"] = xlCell::T_Number;

		StrToType["char"] = xlCell::T_String; //从系统内置类型识别后转换到ssType类型
		StrToType["unsigned char"] = xlCell::T_String;
		StrToType["int"] = xlCell::T_Number;
		StrToType["long int"] = xlCell::T_Number;
		StrToType["unsigned int"] = xlCell::T_Number;
		StrToType["long"] = xlCell::T_Number;
		StrToType["double"] = xlCell::T_Number;
		StrToType["long double"] = xlCell::T_Number;
		StrToType["float"] = xlCell::T_Number;

		//在这里添加类型到字符串的映射
		TypeToStr[xlCell::T_String] = "String";
		TypeToStr[xlCell::T_Number] = "Number";

		_initialized = true;
	}
}


/////////////////////////////////////////////////////////////////////////////////////
// cell //
xlCell::xlCell(string _data, SS_TYPE type) :
		_pSh(NULL), data(_data), ss_type(type), _colIndex(0), _rowIndex(0)
{
}

xlCell::xlCell(int _data) :
		_pSh(NULL), _colIndex(0), _rowIndex(0)
{
	data = intToStr(_data);
	ss_type = xlCell::T_Number;
}

xlCell::xlCell(double _data) :
		_pSh(NULL), _colIndex(0), _rowIndex(0)
{
	data = doubleToStr(_data);
	ss_type = xlCell::T_Number;
}

xlCell::xlCell(TiXmlNode* _cell, $ xQ) :
		_pSh(NULL), _colIndex(0), _rowIndex(0)
{
	parseToCells(_cell);
	parseToStyle(xQ);
}

xlCell::xlCell(UINT rIndex, UINT cIndex, xlSheet* psh) :
		_pSh(psh), data(""), ss_type(T_String), _colIndex(cIndex), _rowIndex(
				rIndex)
{
}

xlCell& xlCell::parseToCells(TiXmlNode* _cell)
{
	data = $(_cell).children("Data").text();
	
	// debug : adolli
	//			以下两句代码在xlWorkbook::parseToSheets()函数中传入，不由xlCell控制
	//_colIndex	=$(_cell).iAttr("ss:Index");
	//_rowIndex	=$(_cell).parent().iAttr("ss:Index");
	ss_type = CellType[$(_cell).children("Data").attr("ss:Type")];
	_style.styleID = $(_cell).attr("ss:StyleID");
	return *this;
}

string xlCell::value()
{
	return data;
}

int xlCell::iValue()
{
	int i = ERR_FAIL_TO_GetValue;
	getValue(i);
	return i;
}

double xlCell::dValue()
{
	double d = ERR_FAIL_TO_GetValue;
	getValue(d);
	return d;
}

string xlCell::getValueType()
{
	return __cell_type.TypeToStr[ss_type];
}

UINT xlCell::rowIndex()
{
	return _rowIndex;
}

UINT xlCell::colIndex()
{
	return _colIndex;
}

xlStyle& xlCell::style()
{
	return _style;
}

string xlCell::getStyleID()
{
	return _style.styleID;
}

xlCell& xlCell::parseToStyle($& container)
{
	$ styleClassTag;
	const string StyleID = _style.styleID;
	if (StyleID != "")
	{
		container("Style&[ss:ID=" + StyleID + "]");
	}
	else
	{
		container("Style&[ss:ID=Default]");
	}

	//对齐
	styleClassTag = $(container).children("Alignment");
	if (!styleClassTag.isEmpty())
	{
		_style.alignment.horizontal_ =
				_style.alignment._hAlign_xlStr_toEnum[styleClassTag.attr(
						"ss:Horizontal")];
		_style.alignment.vertical_ =
				_style.alignment._vAlign_xlStr_toEnum[styleClassTag.attr(
						"ss:Vertical")];
		_style.alignment.rotation =
				styleClassTag.iAttr("ss:Rotate") == $::QA_NO_ATTRIBUTE ?
						_style.alignment.rotation :
						styleClassTag.iAttr("ss:Rotate");
		_style.alignment.wrapText = styleClassTag.bAttr("ss:WrapText");
	}

	//边框
	for (map<string, sBorder>::iterator it = _style.borders.begin();
			it != _style.borders.end(); it++)
	{
		if (it->first == "All")
			continue;	// ignore this case
		styleClassTag = $(container).children("Borders").children(
				"[ss:Position=" + it->first + "]");
		if (!styleClassTag.isEmpty())
		{
			it->second.weight_ =
					styleClassTag.iAttr("ss:Weight") == $::QA_NO_ATTRIBUTE ?
							it->second.weight_ :
							styleClassTag.iAttr("ss:Weight");
			it->second.lineStyle_ =
					it->second._lStyleMap_xlStr_toEnum[styleClassTag.attr(
							"ss:LineStyle")];
			it->second.color_ = styleClassTag.attr("ss:Color");
		}
	}

	//字体
	styleClassTag = $(container).children("Font");
	if (!styleClassTag.isEmpty())
	{
		_style.font.bold_ = styleClassTag.bAttr("ss:Bold");
		_style.font.italic_ = styleClassTag.bAttr("ss:Italic");
		_style.font.strikeThrough_ = styleClassTag.bAttr("ss:StrikeThrough");
		_style.font.charSet = styleClassTag.attr("x:CharSet");
		_style.font.color_ = styleClassTag.attr("ss:Color");
		_style.font.name = styleClassTag.attr("ss:FontName");
		_style.font.size_ =
				styleClassTag.dAttr("ss:Size") == $::QA_NO_ATTRIBUTE ?
						_style.font.size_ : styleClassTag.dAttr("ss:Size");
		_style.font.vertical_Align_ =
				_style.font._vAlign_xlStr_toEnum[styleClassTag.attr(
						"ss:VerticalAlign")];
		_style.font.underlineStyle_ =
				_style.font._underLnMap_xlStr_toEnum[styleClassTag.attr(
						"ss:Underline")];
	}

	//填充
	styleClassTag = $(container).children("Interior");
	if (!styleClassTag.isEmpty())
	{
		_style.interior.color_ = styleClassTag.attr("ss:Color");
		_style.interior.pattern =
				_style.interior._patternMap_xlStr_toEnum[styleClassTag.attr(
						"ss:Pattern")];
	}

	return *this;
}

xlCell& xlCell::setStyleBy(const xlStyle& _style)
{
	this->_style = _style;
	return *this;
}

xlCell& xlCell::printCell()
{
	cout << data << "  type:" << CellTypeStr[ss_type] << endl;
	return *this;
}

bool xlCell::isEmpty()
{
	return "" == data;
}

xlCell& xlCell::clear()
{
	data = "";
	ss_type = T_String;
	return *this;
}

xlCell& xlCell::clearStyle()
{
	_style = xlStyle();	//default constructor
	return *this;
}

void xlCell::Delete(ShiftType _type)
{
	_pSh->DeleteCell(_rowIndex, _colIndex, _type);
}

//////////////////////////////////////////////////////////////////////////////////////
// sheet //
xlSheet::xlSheet() :
		_pWB(NULL)
{
}

xlSheet::xlSheet(string _name, xlWorkbook* _pwb) :
		_sheetName(_name), _pWB(_pwb)
{
}

xlCell& xlSheet::cells(int r, int c)
{
	r = r <= 0 ? 0 : r;
	c = c <= 0 ? 0 : c;
	if (r > maxRow())
		setMaxRow(r);
	if (c > maxCol())
		setMaxCol(c);
	return ss_table[r - 1][c - 1];
}

xlCell& xlSheet::cells(string a1_style)
{
	A1_STYLE a1(a1_style);
	return cells(a1.Row1, a1.Col1);
}

const UINT xlSheet::maxCol() const
{
	if (ss_table.empty())
	{
		return 0;
	}
	else
	{
		return ss_table[0].size();
	}
}

const UINT xlSheet::maxRow() const
{
	return ss_table.size();
}

xlSheet& xlSheet::setMaxRow(UINT r)
{
	if (r <= maxRow())
	{
		return *this;
	}

	UINT originMaxRow = maxRow();				//读取原始行数
	UINT originMaxCol = maxCol();				//读取原始列数

	for (UINT _r = originMaxRow; _r < r; _r++)
	{
		// table new added should be resized to max col
		ss_table.push_back(vector<xlCell>());
		for (UINT _c = 0; _c < originMaxCol; _c++)
		{
			// 新扩展的行需要给每个单元格初始化
			ss_table.back().push_back(xlCell(_r + 1, _c + 1, this));
		}
	}
	return *this;
}

xlSheet& xlSheet::setMaxCol(UINT c)
{
	if (c <= maxCol())
	{
		return *this;
	}

	UINT originMaxRow = maxRow();				//读取原始行数
	UINT originMaxCol = maxCol();				//读取原始列数

	for (UINT _r = 0; _r < originMaxRow; _r++)
	{
		for (UINT _c = originMaxCol; _c < c; _c++)
		{
			ss_table[_r].push_back(xlCell(_r + 1, _c + 1, this));
		}
	}
	return *this;
}

string xlSheet::sheetName()
{
	return _sheetName;
}

xlSheet& xlSheet::sheetName(string setName)
{
	_sheetName = setName;
	return *this;
}

xlSheet& xlSheet::printSheet()
{
	cout << "sheetName:" << _sheetName << "------------" << endl;
	for (UINT r = 0; r < this->ss_table.size(); r++)
	{
		cout << "---------Row " << r + 1 << " ----------" << endl;
		for (UINT c = 0; c < this->ss_table[r].size(); c++)
		{
			ss_table[r][c].printCell();
		}
		cout << "--end of Row " << r + 1 << " ----------" << endl;

	}
	cout << endl;
	return *this;
}

xlRange xlSheet::range(int r1, int c1, int r2, int c2)
{
	return xlRange(this, r1, c1, r2, c2);
}

xlRange xlSheet::range(Cell cell1, Cell cell2)
{
	return xlRange(this, cell1, cell2);
}

xlRange xlSheet::range(string a1_style)
{
	return xlRange(this, a1_style);
}

xlRow xlSheet::row(UINT startIndex, UINT endIndex)
{
	return xlRow(this, startIndex, endIndex);
}

xlRow xlSheet::row(string a1_style)
{
	A1_STYLE a1(a1_style);
	return xlRow(this, a1.Row1, a1.Row2);
}

xlColumn xlSheet::column(UINT startIndex, UINT endIndex)
{
	return xlColumn(this, startIndex, endIndex);
}

xlColumn xlSheet::column(string a1_style)
{
	A1_STYLE a1(a1_style);
	return xlColumn(this, a1.Col1, a1.Col2);
}

xlWorkbook& xlSheet::Delete()
{
	return _pWB->DeleteSheet(_sheetName);
}

xlSheet& xlSheet::DeleteCell(int r, int c, ShiftType _type)
{
	if (r <= 0 || c <= 0 || c > maxCol() || r > maxRow())
	{
		return *this;
	}
	
	UINT maxcol = maxCol();
	UINT maxrow = maxRow();
	switch (_type)
	{
	case xlShiftLeft:
		ss_table[r - 1].erase(ss_table[r - 1].begin() + c - 1);
		ss_table[r - 1].resize(maxcol);
		break;
	case xlShiftUp:
	default:
		for (UINT _r = r - 1; _r < maxrow - 1; _r++)
		{
			// 如果删除的是边缘的单元格，for里的语句是不会执行的
			ss_table[_r][c - 1] = ss_table[_r + 1][c - 1];
		}
		ss_table[maxrow - 1][c - 1].clear().clearStyle();
	}
	return *this;
}

xlSheet& xlSheet::DeleteCell(Cell cell, ShiftType _type)
{
	return DeleteCell(cell.row, cell.col, _type);
}

xlSheet& xlSheet::DeleteRange(int r1, int c1, int r2, int c2, ShiftType _type)
{
	UINT maxcol = maxCol();
	UINT maxrow = maxRow();
	r1 = r1 <= 0 ? 1 : r1 > maxrow ? maxrow : r1;
	r2 = r2 <= 0 ? 1 : r2 > maxrow ? maxrow : r2;
	c1 = c1 <= 0 ? 1 : c1 > maxcol ? maxcol : c1;
	c2 = c2 <= 0 ? 1 : c2 > maxcol ? maxcol : c2;

	int rStart = min(r1, r2);
	int cStart = min(c1, c2);
	int rEnd = max(r1, r2);
	int cEnd = max(c1, c2);
	int rSpan = rEnd - rStart + 1;
	int cSpan = cEnd - cStart + 1;
	switch (_type)
	{
	case xlShiftLeft:
		if (cStart == 1 && cEnd == maxcol)
		{
			// 整行清空的情况
			for (UINT r = rStart - 1; r <= rEnd - 1; r++)
			{
				ss_table[r].clear();
				ss_table[r].resize(maxcol);
			}
		}
		else
		{
			for (UINT r = rStart - 1; r <= rEnd - 1; r++)
			{
				ss_table[r].erase(ss_table[r].begin() + cStart - 1,
						ss_table[r].begin() + cEnd);
				ss_table[r].resize(maxcol);
			}
		}
		break;
	case xlShiftUp:
		if (cStart == 1 && cEnd == maxcol)
		{
			// 整行删除的情况
			ss_table.erase(ss_table.begin() + rStart - 1,
					ss_table.begin() + rEnd);
			setMaxRow(maxrow);
		}
		else
		{
			for (UINT c = cStart - 1; c <= cEnd - 1; c++)
			{
				for (UINT r = rStart - 1; r < maxrow - rSpan; r++)
				{
					ss_table[r][c] = ss_table[r + rSpan][c];
				}
				for (UINT r = maxrow - rEnd; r < maxrow; r++)
				{
					// 当删除的单元格在边界时可以直接清除，不会经过上面那个循环
					ss_table[r][c].clear().clearStyle();
				}
			}
		}
		break;
	case xlDetermineBySize:
	default:
		if (rSpan > cSpan)
		{
			DeleteRange(r1, c1, r2, c2, xlShiftUp);
		}
		else
		{
			DeleteRange(r1, c1, r2, c2, xlShiftLeft);
		}
	}
	return *this;
}

xlSheet& xlSheet::DeleteRange(Cell cell1, Cell cell2, ShiftType _type)
{
	return DeleteRange(cell1.row, cell1.col, cell2.row, cell2.col, _type);
}

xlSheet& xlSheet::DeleteRange(string a1_Style, ShiftType _type)
{
	A1_STYLE a1(a1_Style);
	return DeleteRange(a1.Row1, a1.Col1, a1.Row2, a1.Col2, _type);
}

//////////////////////////////////////////////////////////////////////////////////////
// workbook //
xlWorkbook::xlWorkbook(string filename) :
		_filename(filename), XL(filename.c_str())
{
	parseToSheets();
	parseDefaultStyle();
}

xlWorkbook& xlWorkbook::load(string filename)
{
	XL.loadXml(filename.c_str());
	parseToSheets();
	// 最后把默认样式保存进style_map[0]的位置
	parseDefaultStyle();
	return *this;
}

xlWorkbook& xlWorkbook::parseToSheets()
{
	// 把整个xml表格解析到内存中

	// 读取表格数以及ss:Name
	XL("Worksheet");
	for (UINT i = 0; i < XL.length(); i++)
	{

		// 直接push进去效率高，同时设置pwb指针和sheet name
		_theSheets.push_back(xlSheet($(XL[i]).attr("ss:Name"), this));
		// 读取expandRow
		_theSheets[i].setMaxRow(
				strToInt(
						$(XL[i]).children("Table").attr(
								"ss:ExpandedRowCount")));
		// 读取expandCol
		_theSheets[i].setMaxCol(
				strToInt(
						$(XL[i]).children("Table").attr(
								"ss:ExpandedColumnCount")));

		// 读取每一行
		$ rows = $(XL[i]).children("Table").children("Row");
		int rowIndex = 0;
		int rowSpan = 0;
		for (UINT j = 0; j < rows.length(); j++)
		{

			// 给这一行编号
			if ($(rows[j]).attr("ss:Index") == "")
			{
				rowIndex++;
				$(rows[j]).attr("ss:Index", rowIndex);
			}
			else
			{
				rowSpan =
						$(rows[j]).iAttr("ss:Span") != $::QA_NO_ATTRIBUTE ?
								$(rows[j]).iAttr("ss:Span") : 0;
				rowIndex = $(rows[j]).iAttr("ss:Index") + rowSpan;
			}

			// 在遍历每一行时读取cells
			$ cells = $(rows[j]).children("Cell");
			int cellIndex = 0;
			for (UINT k = 0; k < cells.length(); k++)
			{
				// 给每一个cell写上ss:Index
				if ($(cells[k]).attr("ss:Index") == "")
				{
					cellIndex++;
					$(cells[k]).attr("ss:Index", cellIndex);
				}
				else
				{
					cellIndex = $(cells[k]).iAttr("ss:Index");
				}

				// 读取到ss_table中
				_theSheets[i].ss_table[rowIndex - 1][cellIndex - 1] = xlCell(
						cells[k], XL);	//载入单元格
				_theSheets[i].ss_table[rowIndex - 1][cellIndex - 1]._pSh =
						&_theSheets[i];	//为单元格设置指向该sheet的指针
				_theSheets[i].ss_table[rowIndex - 1][cellIndex - 1]._rowIndex =
						rowIndex;		//单元格行号
				_theSheets[i].ss_table[rowIndex - 1][cellIndex - 1]._colIndex =
						cellIndex;	//单元格列号
			}
		}
	}
	return *this;
}

xlWorkbook& xlWorkbook::inverseParse()
{

	// 从模版里拷贝一个文件过来，然后进行编辑
	XL.loadXml("settings/_xl_template_.xml");
	XL.save("_temp_xl");	//临时文件不要文件名
	XL.loadXml("_temp_xl");

	$ pasteTo = XL("Workbook");
	for (UINT i = 0; i < _theSheets.size(); i++)
	{

		// 拷贝Worksheet节点，并设置新Worksheet的属性值
		XL("Workbook>Worksheet&[ss:Name=_st_template_]").clone(pasteTo,
				$::APOS_INSIDE | $::APOS_AFTER, $::CM_DeepCopy).attr("ss:Name",
				_theSheets[i]._sheetName).children("Table").attr(
				"ss:ExpandedColumnCount", int(_theSheets[i].maxCol())).attr(
				"ss:ExpandedRowCount", int(_theSheets[i].maxRow())).children().remove();

		// 在新的Worksheet下写好Row和Cell节点
		XL(
				"Workbook>Worksheet&[ss:Name=" + _theSheets[i]._sheetName
						+ "]>Table");
		for (UINT r = 0; r < _theSheets[i].ss_table.size(); r++)
		{
			XL.appendElement("Row").children("Row.last").attr("ss:Index",
					int(r + 1));
			for (UINT c = 0; c < _theSheets[i].ss_table[r].size(); c++)
			{
				xlCell& tmpCell = _theSheets[i].ss_table[r][c];
				// 当单元格有内容时才插入到xml电子表格中
				if (!(tmpCell._style == _style_map[0]) || tmpCell.data != "")
				{
					string styleID = "";
					if (!(tmpCell._style == _style_map[0]))
					{
						styleID = "st"
								+ intToStr(registerStyle(tmpCell._style));
						tmpCell._style.styleID = styleID;
					}
					XL.appendElement("Cell").children("Cell.last")//定位到刚插入的Cell
					.attr("ss:Index", int(c + 1))					//设置index
					.attr("ss:StyleID", styleID)					//设置styleID
					.appendElement("Data")						//插入Date节点
					.children("Data")							//定位到刚插入的Data节点
					.attr("ss:Type", tmpCell.getValueType())		//设置ssType
					.appendText(tmpCell.data)					//插入单元格内容
					.parent().parent();						//最后退回到Row节点，进入下一次循环
				}
			}
			XL.parent();										//最后退回Table节点
		}
	}
	// 最后移除模版Worksheet
	XL("Workbook>Worksheet&[ss:Name=_st_template_]").remove();

	// 反解析并插入style节点
	XL("Styles");
	for (map<int, xlStyle>::iterator it_stpool = _style_map.begin();
			it_stpool != _style_map.end(); it_stpool++)
	{
		// 设置江南Style节点
		XL.appendElement("Style").children("Style.last").attr("ss:ID",
				"st" + intToStr(it_stpool->first));

		// 设置Style内的节点，并保证出入点一致
		XL.appendElement("Alignment").children("Alignment").attr(
				"ss:Horizontal",
				it_stpool->second.alignment._hAlign_toStr[it_stpool->second.alignment.horizontal_]).attr(
				"ss:Vertical",
				it_stpool->second.alignment._vAlign_toStr[it_stpool->second.alignment.vertical_]).attr(
				"ss:Rotate", it_stpool->second.alignment.rotation).attr(
				"ss:WrapText", it_stpool->second.alignment.wrapText).parent();

		XL.appendElement("Borders").children("Borders");
		if (!(it_stpool->second.borders["All"]
				== this->_style_map[0].borders["All"]))
		{
			for (map<string, sBorder>::iterator it =
					it_stpool->second.borders.begin();
					it != it_stpool->second.borders.end(); it++)
			{
				if (it->first == "All")
					continue;
				it->second = it_stpool->second.borders["All"];
			}
		}
		for (map<string, sBorder>::iterator it =
				it_stpool->second.borders.begin();
				it != it_stpool->second.borders.end(); it++)
		{
			if (it->first == "All")
				continue;
			XL.appendElement("Border").children(".last").attr("ss:Position",
					it->first).attr("ss:LineStyle",
					it->second._lStyleMap_toStr[it->second.lineStyle_]).attr(
					"ss:Weight", it->second.weight_).attr("ss:Color",
					it->second.color_).parent();				//设置完四个属性后返回到父节点
		}
		XL.parent();

		XL.appendElement("Font").children("Font").attr("ss:FontName",
				it_stpool->second.font.name).attr("x:CharSet",
				it_stpool->second.font.charSet).attr("ss:Size",
				it_stpool->second.font.size_).attr("ss:Color",
				it_stpool->second.font.color_).attr("ss:Bold",
				it_stpool->second.font.bold_).attr("ss:Italic",
				it_stpool->second.font.italic_).attr("ss:StrikeThrough",
				it_stpool->second.font.strikeThrough_).attr("ss:VerticalAlign",
				it_stpool->second.font._vAlign_toStr[it_stpool->second.font.vertical_Align_]).attr(
				"ss:Underline",
				it_stpool->second.font._underLnMap_toStr[it_stpool->second.font.underlineStyle_]).parent();

		XL.appendElement("Interior").children("Interior").attr("ss:Color",
				it_stpool->second.interior.color_).attr("ss:Pattern",
				it_stpool->second.interior._patternMap_toStr[it_stpool->second.interior.pattern]).parent();

		XL.parent();
	}
	return *this;
}

xlWorkbook& xlWorkbook::save()
{
	inverseParse();
	XL.save(_filename.c_str());
	XL.loadXml(_filename.c_str());
	XL.save(_filename.c_str());
	return *this;
}

xlWorkbook& xlWorkbook::saveAs(string filename)
{
	inverseParse();
	XL.save(filename.c_str());
	XL.loadXml(filename.c_str());
	XL.save(filename.c_str());
	return *this;
}

const UINT xlWorkbook::sheetsCount() const
{
	return _theSheets.size();
}

xlSheet& xlWorkbook::sheets(int index)
{
	if (sheetsCount() == 0)
		_theSheets.push_back(xlSheet("newSheet1", this));
	index = index > sheetsCount() ? sheetsCount() : index <= 0 ? 1 : index;
	return _theSheets[index - 1];
}

xlSheet& xlWorkbook::sheets(string sheetName)
{
	for (UINT i = 0; i < sheetsCount(); i++)
	{
		if (this->_theSheets[i]._sheetName == sheetName)
		{
			return _theSheets[i];
		}
	}
	throw "sheets not find! check your sheet name!";
	return sheets(1);
}

xlSheet& xlWorkbook::newSheet(string _sheetName)
{
	_sheetName =
			_sheetName == "" ?
					"newSheet" + intToStr(sheetsCount() + 1) : _sheetName;
	_theSheets.push_back(xlSheet(_sheetName, this));
	return _theSheets.back();
}

xlWorkbook& xlWorkbook::newSheets(int number_of_new)
{
	for (int i = 0; i < number_of_new; i++)
	{
		int index = 0;
		string newSheetName = "";
		do
		{
			index++;
			newSheetName = "newSheet" + intToStr(sheetsCount() + index);
		} while (hasSheet(newSheetName));
		newSheet(newSheetName);
	}
	return *this;
}

bool xlWorkbook::hasSheet(string name) const
{
	for (UINT i = 0; i < sheetsCount(); i++)
	{
		if (_theSheets[i]._sheetName == name)
		{
			return true;
		}
	}
	return false;
}

xlWorkbook& xlWorkbook::DeleteSheet(string _sheetName)
{
	for (UINT i = 0; i < sheetsCount(); i++)
	{
		if (_sheetName == _theSheets[i]._sheetName)
		{
			DeleteSheet(i + 1);
			return *this;
		}
	}
	return *this;
}

xlWorkbook& xlWorkbook::DeleteSheet(int index)
{
	if (index <= 0 || index > sheetsCount())
		return *this;
	_theSheets.erase(_theSheets.begin() + index - 1);
	return *this;
}


////////////////////////////////////////////////////////////////////////////////////////////////
// xlStylePool
xlWorkbook& xlWorkbook::parseToStylePool()
{
	$ styleClassTag;
	XL("Style");
	xlStyle _style;
	string _styleID;
	for (UINT i = 0; i < XL.length(); i++)
	{
		_styleID = $(XL[i]).attr("ss:ID");

		//对齐
		styleClassTag = $(XL[i]).children("Alignment");
		if (!styleClassTag.isEmpty())
		{
			_style.alignment.horizontal_ =
					_style.alignment._hAlign_xlStr_toEnum[styleClassTag.attr(
							"ss:Horizontal")];
			_style.alignment.vertical_ =
					_style.alignment._vAlign_xlStr_toEnum[styleClassTag.attr(
							"ss:Vertical")];
			_style.alignment.rotation =
					styleClassTag.iAttr("ss:Rotate") == $::QA_NO_ATTRIBUTE ?
							_style.alignment.rotation :
							styleClassTag.iAttr("ss:Rotate");
			_style.alignment.wrapText = styleClassTag.bAttr("ss:WrapText");
		}

		//边框
		for (map<string, sBorder>::iterator it = _style.borders.begin();
				it != _style.borders.end(); it++)
		{
			if (it->first == "All")
				continue;								// ignore this case
			styleClassTag = $(XL[i]).children("Borders").children(
					"[ss:Position=" + it->first + "]");
			if (!styleClassTag.isEmpty())
			{
				it->second.weight_ =
						styleClassTag.iAttr("ss:Weight") == $::QA_NO_ATTRIBUTE ?
								it->second.weight_ :
								styleClassTag.iAttr("ss:Weight");
				it->second.lineStyle_ =
						it->second._lStyleMap_xlStr_toEnum[styleClassTag.attr(
								"ss:LineStyle")];
				it->second.color_ = styleClassTag.attr("ss:Color");
			}
		}

		//字体
		styleClassTag = $(XL[i]).children("Font");
		if (!styleClassTag.isEmpty())
		{
			_style.font.bold_ = styleClassTag.bAttr("ss:Bold");
			_style.font.italic_ = styleClassTag.bAttr("ss:Italic");
			_style.font.strikeThrough_ = styleClassTag.bAttr(
					"ss:StrikeThrough");
			_style.font.charSet = styleClassTag.attr("x:CharSet");
			_style.font.color_ = styleClassTag.attr("ss:Color");
			_style.font.name = styleClassTag.attr("ss:FontName");
			_style.font.size_ =
					styleClassTag.dAttr("ss:Size") == $::QA_NO_ATTRIBUTE ?
							_style.font.size_ : styleClassTag.dAttr("ss:Size");
			_style.font.vertical_Align_ =
					_style.font._vAlign_xlStr_toEnum[styleClassTag.attr(
							"ss:VerticalAlign")];
			_style.font.underlineStyle_ =
					_style.font._underLnMap_xlStr_toEnum[styleClassTag.attr(
							"ss:Underline")];
		}

		//填充
		styleClassTag = $(XL[i]).children("Interior");
		if (!styleClassTag.isEmpty())
		{
			_style.interior.color_ = styleClassTag.attr("ss:Color");
			_style.interior.pattern =
					_style.interior._patternMap_xlStr_toEnum[styleClassTag.attr(
							"ss:Pattern")];
		}

		// 在_style_map里添加style映射
		while (!_styleID.empty() && (_styleID[0] > '9' || _styleID[0] < '0'))
		{
			_styleID = _styleID.substr(1);
		}
		_style_map[strToInt(_styleID)] = _style;
	}

	// 最后把默认样式保存进style_map[0]的位置
	parseDefaultStyle();
	return *this;
}

xlWorkbook& xlWorkbook::parseDefaultStyle()
{
	_style_map[0] = xlStyle();
	return *this;
}

int xlWorkbook::registerStyle(const xlStyle& _style)
{
	map<int, xlStyle>::iterator it = _style_map.begin();
	for (; it != _style_map.end(); it++)
	{
		if (it->second == _style)
		{
			return it->first;							// 若已经存在该样式了那就直接退出返回ID号
		}
	}
	int newID = _style_map.rbegin()->first;
	while (_style_map.find(++newID) != _style_map.end())
		;								//扫描寻找一个未被占用的ID号
	_style_map[newID] = _style;
	return newID;
}

//////////////////////////////////////////////////////////////////////////
xlRange::xlRange(xlSheet* _xlsheet, int r1, int c1, int r2, int c2) :
		_pSh(_xlsheet)
{
	__Initialize_p_Range(_xlsheet, r1, c1, r2, c2);
}

xlRange::xlRange(xlSheet* _xlsheet, string a1_style) :
		_pSh(_xlsheet)
{
	A1_STYLE a1(a1_style);
	__Initialize_p_Range(_xlsheet, a1.Row1, a1.Col1, a1.Row2, a1.Col2);
}

xlRange::xlRange(xlSheet* _xlsheet, Cell cell1, Cell cell2) :
		_pSh(_xlsheet)
{
	__Initialize_p_Range(_xlsheet, cell1.row, cell1.col, cell2.row, cell2.col);
}

xlRange& xlRange::__Initialize_p_Range(xlSheet* _xlsheet, int r1, int c1,
		int r2, int c2)
{
	if (c1 == 0 && c2 == 0)
	{
		// xlRow情况
		r2 = r2 == 0 ? r1 : r2;
		c1 = 1;
		c2 = _xlsheet->maxCol();
	}
	else if (r1 == 0 && r2 == 0)
	{
		// xlColumn情况
		c2 = c2 == 0 ? c1 : c2;
		r1 = 1;
		r2 = _xlsheet->maxRow();
	}
	UINT rStart = min(r1, r2);
	UINT rEnd = max(r1, r2);
	UINT cStart = min(c1, c2);
	UINT cEnd = max(c1, c2);

	_xlsheet->setMaxRow(rEnd);
	_xlsheet->setMaxCol(cEnd);

	p_table.clear();
	p_table.resize(rEnd - rStart + 1);
	for (UINT i = 0; i < p_table.size(); i++)
	{
		p_table[i].resize(cEnd - cStart + 1);
	}
	for (UINT r = rStart; r <= rEnd; r++)
	{
		for (UINT c = cStart; c <= cEnd; c++)
		{
			p_table[r - rStart][c - cStart] = &_xlsheet->ss_table[r - 1][c - 1];
		}
	}
	return *this;
}

xlCell& xlRange::at(UINT rel_r, UINT rel_c)
{
	rel_r = rel_r > p_table.size() ? p_table.size() : rel_r <= 0 ? 1 : rel_r;
	rel_c = rel_c > p_table[rel_r - 1].size() ? p_table[rel_r - 1].size() :
			rel_c <= 0 ? 1 : rel_c;
	return *p_table[rel_r - 1][rel_c - 1];
}

xlCell& xlRange::corner(CornerCell _corner)
{
	if (!p_table.empty() && !p_table[0].empty())
	{
		switch (_corner)
		{
		case xlTopLeft:
			return *p_table[0][0];
		case xlTopRight:
			return *p_table[0][rangeWidth() - 1];
		case xlBottomLeft:
			return *p_table[rangeHeight() - 1][0];
		case xlBottomRight:
		default:
			return *p_table[rangeHeight() - 1][rangeWidth() - 1];
		}
	}
	else
	{
		cout << "range is empty. return NULL" << endl;
		throw 0;							//希望程序不要经过这里，这里是很危险的
	}
}

UINT xlRange::rangeWidth()
{
	if (p_table.empty())
		return 0;
	return p_table[0].size();
}

UINT xlRange::rangeHeight()
{
	return p_table.size();
}

xlRange& xlRange::setStyleBy(const xlStyle& _style)
{
	for (UINT r = 0; r < p_table.size(); r++)
	{
		for (UINT c = 0; c < p_table[r].size(); c++)
		{
			p_table[r][c]->setStyleBy(_style);
		}
	}
	return *this;
}

xlRange& xlRange::clear()
{
	for (UINT r = 0; r < p_table.size(); r++)
	{
		for (UINT c = 0; c < p_table[r].size(); c++)
		{
			p_table[r][c]->clear();
		}
	}
	return *this;
}

xlRange& xlRange::clearStyle()
{
	for (UINT r = 0; r < p_table.size(); r++)
	{
		for (UINT c = 0; c < p_table[r].size(); c++)
		{
			p_table[r][c]->clearStyle();
		}
	}
	return *this;
}

xlRange& xlRange::offset(int r_shift, int c_shift)
{
	int newR1 = corner(xlTopLeft).rowIndex() + r_shift;
	int newC1 = corner(xlTopLeft).colIndex() + c_shift;
	int newR2 = corner(xlBottomRight).rowIndex() + r_shift;
	int newC2 = corner(xlBottomRight).colIndex() + c_shift;
	try
	{
		// 这一块是为了在出错时强制停止，然后手动处理
		if (newR2 <= 0 || newC2 <= 0)
		{
			exception e("the xlRange is out of the table");
			throw e;
		}
	} catch (exception e)
	{
		cout
				<< "the xlRange is out of the table, press any key to exit and debug at //xlRange& xlRange::offset(int r_shift,int c_shift)//"
				<< endl;
		system("pause");
		(*this) = _pSh->range(0, 0, 0, 0);
		return *this;
	}

	newR1 = newR1 <= 0 ? 1 : newR1;
	newC1 = newC1 <= 0 ? 1 : newC1;
	newR2 = newR2 <= 0 ? 1 : newR2;
	newC2 = newC2 <= 0 ? 1 : newC2;
	(*this) = _pSh->range(newR1, newC1, newR2, newC2);
	return *this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
xlRow::xlRow(xlSheet* _xlsheet, UINT startRowIndex, UINT endRowIndex) :
		xlRange(_xlsheet, startRowIndex, 0, endRowIndex, 0), _startRowIndex(
				startRowIndex)
{
	_span = endRowIndex == 0 ? 1 : endRowIndex - startRowIndex + 1;
}

xlRow::xlRow(xlSheet* _xlsheet, string a1_style) :
		xlRange(_xlsheet, A1_STYLE(a1_style).Row1, 0, A1_STYLE(a1_style).Row2,
				0), _startRowIndex(A1_STYLE(a1_style).Row1)
{
	A1_STYLE a1(a1_style);
	_span = a1.Row2 == 0 ? 1 : a1.Row2 - a1.Row1 + 1;
}

xlColumn::xlColumn(xlSheet* _xlsheet, UINT startColIndex, UINT endColIndex) :
		xlRange(_xlsheet, 0, startColIndex, 0, endColIndex), _startColIndex(
				startColIndex)
{
	_span = startColIndex == 0 ? 1 : endColIndex - startColIndex + 1;
}

xlColumn::xlColumn(xlSheet* _xlsheet, string a1_style) :
		xlRange(_xlsheet, 0, A1_STYLE(a1_style).Col1, 0,
				A1_STYLE(a1_style).Col2), _startColIndex(
				A1_STYLE(a1_style).Col1)
{
	A1_STYLE a1(a1_style);
	_span = a1.Col2 == 0 ? 1 : a1.Col2 - a1.Col1 + 1;
}

// RC_A1_STYLE ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// static
string A1_STYLE::ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

// A1_STYLE
A1_STYLE::A1_STYLE(const string a1_style)
{
	_expr = a1_style;
	if (this->isValid())
	{
		Row1 = 0;							//默认0值
		Col1 = 0;
		Row2 = 0;
		Col2 = 0;
		absolute["Row1"] = false;
		absolute["Col1"] = false;
		absolute["Row2"] = false;
		absolute["Col2"] = false;
		vector < string > start_to_ends = split(a1_style, ':');
		if (!start_to_ends.empty())
		{
			//判断绝对区域 col
			if (!start_to_ends[0].empty() && start_to_ends[0][0] == '$')
			{
				absolute["Col1"] = true;
				start_to_ends[0] = start_to_ends[0].substr(1);
			}
			while (!start_to_ends[0].empty()
					&& ALPHABET.find(start_to_ends[0][0]) != string::npos)
			{
				Col1 = Col1 * 26 + ALPHABET.find(start_to_ends[0][0]) + 1;
				start_to_ends[0] = start_to_ends[0].substr(1);
			}
			//判断绝对区域 row
			if (!start_to_ends[0].empty() && start_to_ends[0][0] == '$')
			{
				absolute["Row1"] = true;
				start_to_ends[0] = start_to_ends[0].substr(1);
			}
			if (!start_to_ends[0].empty())
			{
				Row1 = strToInt(start_to_ends[0]);
			}
		}
		if (start_to_ends.size() >= 2)
		{
			//判断绝对区域 col
			if (!start_to_ends[1].empty() && start_to_ends[1][0] == '$')
			{
				absolute["Col2"] = true;
				start_to_ends[1] = start_to_ends[1].substr(1);
			}
			while (!start_to_ends[1].empty()
					&& ALPHABET.find(start_to_ends[1][0]) != string::npos)
			{
				Col2 = Col2 * 26 + ALPHABET.find(start_to_ends[1][0]) + 1;
				start_to_ends[1] = start_to_ends[1].substr(1);
			}
			if (!start_to_ends[1].empty() && start_to_ends[1][0] == '$')
			{
				absolute["Row2"] = true;
				start_to_ends[1] = start_to_ends[1].substr(1);
			}
			if (!start_to_ends[1].empty())
			{
				Row2 = strToInt(start_to_ends[1]);
			}
		}
	}
	else
	{
		*this = RC_STYLE(a1_style);
	}
}

A1_STYLE& A1_STYLE::operator=(const string a1_style)
{
	A1_STYLE copy(a1_style);
	*this = copy;
	return *this;
}

A1_STYLE& A1_STYLE::operator=(RC_STYLE rc_style)
{
	this->_expr = rc_style._expr;
	this->Row1 = rc_style.Row1;
	this->Row2 = rc_style.Row2;
	this->Col1 = rc_style.Col1;
	this->Col2 = rc_style.Col2;
	absolute["Row1"] = !(rc_style.relativity["Row1"]);
	absolute["Row2"] = !(rc_style.relativity["Row2"]);
	absolute["Col1"] = !(rc_style.relativity["Col1"]);
	absolute["Col2"] = !(rc_style.relativity["Col2"]);
	return *this;
}

RC_STYLE A1_STYLE::convertToR1C1(string exp)
{
	return "" == exp ? RC_STYLE(_expr) : RC_STYLE(exp);
}

bool A1_STYLE::isValid(string exp)
{
	exp = "" == exp ? _expr : exp;
	if (exp.find("$") != string::npos)
		return true;
	return !RC_STYLE(exp).isValid();
}

bool A1_STYLE::isRange()
{
	return Row1 != 0 && Row2 != 0 && Col1 != 0 && Col2 != 0;
}

// RC_STYLE
RC_STYLE::RC_STYLE(const string rc_style)
{
	_expr = rc_style;

	//将小写的字母转换到大写转换到大写（主要处理r1c1）
	string rc_exp = rc_style;
	transform(rc_exp.begin(), rc_exp.end(), rc_exp.begin(), toupper);

	Row1 = 0;							//默认0值
	Col1 = 0;
	Row2 = 0;
	Col2 = 0;
	relativity["Row1"] = false;
	relativity["Col1"] = false;
	relativity["Row2"] = false;
	relativity["Col2"] = false;
	vector < string > rc = split(rc_exp, ':');
	int pos_R;
	int pos_C;
	if (!rc.empty())
	{
		relativity["Row1"] = (pos_R = rc[0].find("R[")) != string::npos;
		relativity["Col1"] = (pos_C = rc[0].find("C[")) != string::npos;
		if (relativity["Row1"])
		{
			Row1 = strToInt(
					rc[0].substr(pos_R + 2, rc[0].find(']') - pos_R - 2));
		}
		else
		{
			pos_R = rc[0].find("R");
			pos_C = rc[0].find("C");
			Row1 = strToInt(rc[0].substr(pos_R + 1, pos_C - pos_R - 1));
		}
		if (relativity["Col1"])
		{
			Col1 = strToInt(
					rc[0].substr(pos_C + 2,
							rc[0].find_last_of(']') - pos_C - 2));
		}
		else
		{
			pos_C = rc[0].find("C");
			Col1 = strToInt(rc[0].substr(pos_C + 1));
		}
	}
	if (rc.size() >= 2)
	{
		relativity["Row2"] = (pos_R = rc[1].find("R[")) != string::npos;
		relativity["Col2"] = (pos_C = rc[1].find("C[")) != string::npos;
		if (relativity["Row2"])
		{
			Row2 = strToInt(
					rc[1].substr(pos_R + 2, rc[1].find(']') - pos_R - 2));
		}
		else
		{
			pos_R = rc[1].find("R");
			Row2 = strToInt(
					rc[1].substr(pos_R + 1, rc[1].find('C') - pos_R - 1));
		}
		if (relativity["Col2"])
		{
			Col2 = strToInt(
					rc[1].substr(pos_C + 2,
							rc[1].find_last_of(']') - pos_C - 2));
		}
		else
		{
			pos_C = rc[1].find("C");
			Col2 = strToInt(rc[1].substr(pos_C + 1));
		}
	}
}

RC_STYLE& RC_STYLE::operator=(const string rc_style)
{
	RC_STYLE copy(rc_style);
	*this = copy;
	return *this;
}

RC_STYLE& RC_STYLE::operator=(A1_STYLE a1_style)
{
	this->_expr = a1_style._expr;
	this->Row1 = a1_style.Row1;
	this->Row2 = a1_style.Row2;
	this->Col1 = a1_style.Col1;
	this->Col2 = a1_style.Col2;
	relativity["Row1"] = !(a1_style.absolute["Row1"]);
	relativity["Row2"] = !(a1_style.absolute["Row2"]);
	relativity["Col1"] = !(a1_style.absolute["Col1"]);
	relativity["Col2"] = !(a1_style.absolute["Col2"]);
	return *this;
}

A1_STYLE RC_STYLE::convertToA1(string exp)
{
	return "" == exp ? A1_STYLE(_expr) : A1_STYLE(exp);
}

bool RC_STYLE::isValid(string exp)
{
	exp = "" == exp ? _expr : exp;
	return exp.find("[") != string::npos || exp.find("]") != string::npos
			|| exp.find("r") != string::npos || exp.find("c") != string::npos
			|| (split(exp, ':')[0].find("R") != string::npos
					&& split(exp, ':')[0].find("C") != string::npos);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// StyleBase

// 映射xml初始化标记
bool sBase::_alignMapInitialized = false;
bool sBase::_bordrMapInitialized = false;
bool sBase::_fontMapInitialized = false;
bool sBase::_interiorMapInitialized = false;
bool sBase::_numberFormatMapInitialized = false;
bool sBase::_protectionMapInitialized = false;

sBase::StyleBase()
{
}

/////////////////////////////////////////////////////////////
// sAlign
map<string, int> sAlign::_hAlign;
map<string, int> sAlign::_vAlign;
map<string, int> sAlign::_hAlign_xlStr_toEnum;
map<string, int> sAlign::_vAlign_xlStr_toEnum;
map<int, string> sAlign::_hAlign_toStr;
map<int, string> sAlign::_vAlign_toStr;

sAlign::Alignment_t()
{
	horizontal_ = sAlign::XL_hDefault;
	vertical_ = sAlign::XL_vDefault;
	rotation = 0;
	wrapText = false;
	if (!_alignMapInitialized)
	{
		// 映射类型都在xml文件中定义
		setupMapsByXML(_fileTargetForSettings, "_hAlign", _hAlign,
				_hAlign_toStr, _hAlign_xlStr_toEnum);
		setupMapsByXML(_fileTargetForSettings, "_vAlign", _vAlign,
				_vAlign_toStr, _vAlign_xlStr_toEnum);
		_alignMapInitialized = true;
	}
}

sAlign& sAlign::operator=(string expr)
{
	vector < string > exprs = split(expr, ' ');
	for (UINT i = 0; i < exprs.size(); i++)
	{
		horizontal_ =
				_hAlign[exprs[i]] == XL_hDefault ?
						horizontal_ : _hAlign[exprs[i]];
		vertical_ =
				_vAlign[exprs[i]] == XL_vDefault ?
						vertical_ : _vAlign[exprs[i]];
		rotation = strToInt(exprs[i]) == 0 ? rotation : strToInt(exprs[i]);
		wrapText = exprs[i].find("wrap") == string::npos ? wrapText : true;
	}
	return *this;
}

sAlign& sAlign::setVerticalAlign(vAlign_t enumValue)
{
	vertical_ = enumValue;
	return *this;
}

sAlign& sAlign::setHorizontalAlign(hAlign_t enumValue)
{
	horizontal_ = enumValue;
	return *this;
}

sAlign& sAlign::setVerticalAlign(string strValue)
{
	string temp = strValue;
	transform(temp.begin(), temp.end(), strValue.begin(), tolower);
	vertical_ = _vAlign[strValue];
	return *this;
}

sAlign& sAlign::setHorizontalAlign(string strValue)
{
	string temp = strValue;
	transform(temp.begin(), temp.end(), strValue.begin(), tolower);
	horizontal_ = _hAlign[strValue];
	return *this;
}

sAlign& sAlign::setWrappedText(bool value)
{
	wrapText = value;
	return *this;
}

sAlign& sAlign::setRotation(int value)
{
	rotation = value;
	return *this;
}

/////////////////////////////////////////////////////////////
// sBorders_t()
map<string, int> sBorder::_lStyleMap;
map<string, int> sBorder::_lStyleMap_xlStr_toEnum;
map<int, string> sBorder::_lStyleMap_toStr;

sBorder::Borders_t()
{
	color_ = "#000000";					//默认黑色
	weight_ = 1;							//默认线宽度，0表示没有
	lineStyle_ = XL_None;					//默认没有线条
	if (!_bordrMapInitialized)
	{
		setupMapsByXML(_fileTargetForSettings, "LineStyle_t", _lStyleMap,
				_lStyleMap_toStr, _lStyleMap_xlStr_toEnum);
		_bordrMapInitialized = true;
	}
}

sBorder& sBorder::operator=(string expr)
{
	vector < string > exprs = split(expr, ' ');
	for (UINT i = 0; i < exprs.size(); i++)
	{
		lineStyle_ =
				_lStyleMap[exprs[i]] == XL_None ?
						lineStyle_ : _lStyleMap[exprs[i]];
		weight_ = strToInt(exprs[i]) == 0 ? weight_ : strToInt(exprs[i]);
		color_ = exprs[i].find('#') == string::npos ? color_ : exprs[i];
	}
	return *this;
}

sBorder& sBorder::setLineStyle(string strValue)
{
	string temp = strValue;
	transform(temp.begin(), temp.end(), strValue.begin(), tolower);
	lineStyle_ = _lStyleMap[strValue];
	return *this;
}

sBorder& sBorder::setLineStyle(LineStyle_t enumValue)
{
	lineStyle_ = enumValue;
	return *this;
}

sBorder& sBorder::setBorderWeight(int iValue)
{
	weight_ = iValue;
	return *this;
}

sBorder& sBorder::setBorderColor(string strValue)
{
	color_ = strValue;
	return *this;
}

/////////////////////////////////////////////////////////////
// sFont
map<string, int> sFont::_underLnMap;
map<string, int> sFont::_underLnMap_xlStr_toEnum;
map<int, string> sFont::_underLnMap_toStr;
map<string, int> sFont::_vAlign;
map<string, int> sFont::_vAlign_xlStr_toEnum;
map<int, string> sFont::_vAlign_toStr;

sFont::Font_t()
{
	bold_ = false;
	italic_ = false;
	strikeThrough_ = false;
	underlineStyle_ = XL_None;
	vertical_Align_ = XL_Default;
	color_ = "#000000";
	size_ = 11;
	charSet = "134";
	name = "宋体";
	if (!_fontMapInitialized)
	{
		setupMapsByXML(_fileTargetForSettings, "UnderlineStyle_t", _underLnMap,
				_underLnMap_toStr, _underLnMap_xlStr_toEnum);
		setupMapsByXML(_fileTargetForSettings, "VerticalAlign_t", _vAlign,
				_vAlign_toStr, _vAlign_xlStr_toEnum);
		_fontMapInitialized = true;
	}
}

sFont& sFont::operator=(string expr)
{
	vector < string > exprs = split(expr, ' ');
	for (UINT i = 0; i < exprs.size(); i++)
	{
		color_ = exprs[i].find('#') == string::npos ? color_ : exprs[i];
		size_ = strToDouble(exprs[i]) == 0 ? size_ : strToDouble(exprs[i]);
		underlineStyle_ =
				_underLnMap[exprs[i]] == XL_None ?
						underlineStyle_ : _underLnMap[exprs[i]];
		vertical_Align_ =
				_vAlign[exprs[i]] == XL_Default ?
						vertical_Align_ : _vAlign[exprs[i]];
		strikeThrough_ = exprs[i] == "strikeThrough_" ? true : strikeThrough_;
		italic_ = exprs[i] == "italic_" ? true : italic_;
		bold_ = exprs[i] == "bold_" ? true : bold_;
	}
	return *this;
}

sFont& sFont::setUnderlineStyle(string strValue)
{
	string temp = strValue;
	transform(temp.begin(), temp.end(), strValue.begin(), tolower);
	underlineStyle_ = _underLnMap[strValue];
	return *this;
}

sFont& sFont::setUnderlineStyle(UnderlineStyle_t enumValue)
{
	underlineStyle_ = enumValue;
	return *this;
}

sFont& sFont::setVertical_Align(string strValue)
{
	string temp = strValue;
	transform(temp.begin(), temp.end(), strValue.begin(), tolower);
	vertical_Align_ = _vAlign[strValue];
	return *this;
}

sFont& sFont::setVertical_Align(VerticalAlign_t enumValue)
{
	vertical_Align_ = enumValue;
	return *this;
}

sFont& sFont::setFontName(string value)
{
	name = value;
	return *this;
}

sFont& sFont::setCharSet(string value)
{
	charSet = value;
	return *this;
}

sFont& sFont::setFontSize(double value)
{
	size_ = value;
	return *this;
}

sFont& sFont::setFontColor(string value)
{
	color_ = value;
	return *this;
}

sFont& sFont::setStrikeThrough(bool value)
{
	strikeThrough_ = value;
	return *this;
}

sFont& sFont::setItalic(bool value)
{
	italic_ = value;
	return *this;
}

sFont& sFont::setBold(bool value)
{
	bold_ = value;
	return *this;
}

/////////////////////////////////////////////////////////////
// sInterior
map<string, int> sInterior::_patternMap;
map<string, int> sInterior::_patternMap_xlStr_toEnum;
map<int, string> sInterior::_patternMap_toStr;

sInterior::Interior_t()
{
	pattern = XL_None;	//默认填充样式无
	color_ = "";		//填充色默认无颜色
	if (!_interiorMapInitialized)
	{
		setupMapsByXML(_fileTargetForSettings, "Pattern_t", _patternMap,
				_patternMap_toStr, _patternMap_xlStr_toEnum);
		_interiorMapInitialized = true;
	}
}

sInterior& sInterior::operator=(string expr)
{
	vector < string > exprs = split(expr, ' ');
	for (UINT i = 0; i < exprs.size(); i++)
	{
		color_ = exprs[i].find('#') == string::npos ? color_ : exprs[i];
		pattern =
				_patternMap[exprs[i]] == XL_None ?
						pattern : _patternMap[exprs[i]];
	}
	return *this;
}

sInterior& sInterior::setInteriorColor(string value)
{
	color_ = value;
	return *this;
}

sInterior& sInterior::setInteriorPattern(string value)
{
	pattern = _patternMap[value];
	return *this;
}

sInterior& sInterior::setInteriorPattern(Pattern_t enumValue)
{
	pattern = enumValue;
	return *this;
}

/////////////////////////////////////////////////////////////
// xlStyle
string xlStyle::_fileTargetForSettings = "settings/enumMaps.xml";

xlStyle::xlStyle()
{
	borders["Left"];
	borders["Top"];
	borders["Right"];
	borders["Bottom"];
	//borders["All"];
}

bool xlStyle::operator==(const xlStyle& rhs)
{
	if (&rhs == this)
		return true;

	//由于border是map类型的，所以先判断border，border通过了后再判断别的样式
	map<string, sBorder>::iterator it_lhs = borders.begin();
	map<string, sBorder>::const_iterator it_rhs = rhs.borders.begin();
	for (; it_lhs != borders.end() && it_rhs != rhs.borders.end();
			it_lhs++, it_rhs++)
	{
		if (!(it_lhs->second == it_rhs->second))
		{
			return false;
		}
	}
	return alignment == rhs.alignment && font == rhs.font
			&& interior == rhs.interior;
}

bool sAlign::operator==(const Alignment_t& rhs)
{
	return horizontal_ == rhs.horizontal_ && rotation == rhs.rotation
			&& vertical_ == rhs.vertical_ && wrapText == rhs.wrapText;
}

bool sBorder::operator==(const Borders_t& rhs)
{
	return color_ == rhs.color_ && lineStyle_ == rhs.lineStyle_
			&& weight_ == rhs.weight_;
}

bool sFont::operator==(const Font_t& rhs)
{
	return bold_ == rhs.bold_ && charSet == rhs.charSet && color_ == rhs.color_
			&& italic_ == rhs.italic_ && name == rhs.name && size_ == rhs.size_
			&& strikeThrough_ == rhs.strikeThrough_
			&& underlineStyle_ == rhs.underlineStyle_
			&& vertical_Align_ == rhs.vertical_Align_;
}

bool sInterior::operator==(const Interior_t& rhs)
{
	return this->color_ == rhs.color_ && this->pattern == rhs.pattern;
}


/////////////////////////////////////////////////////////////
// functions
void FreeXL::setupMapsByXML(								//定义的时候要加上namespace
		string xmlFilename,			//xml文件名
		string tagName,				//xml中要读取的标签名
		map<string, int>& strToEnum,	//映射到枚举类型的map
		map<int, string>& enumToStr,	//从枚举映射到string的map
		map<string, int>& xlStrToEnum)
{
	int enumIndex = 0;
	string userStr, xlStr;

	$ xml(xmlFilename.c_str());
	xml(tagName).children(".element");
	for (UINT i = 0; i < xml.length(); i++)
	{
		enumIndex = $(xml[i]).iAttr("index");
		userStr = $(xml[i]).attr("userStr");
		xlStr = $(xml[i]).attr("xlStr");
		enumToStr[enumIndex] = xlStr;					//枚举类型到xl xml中属性str的映射
		strToEnum[userStr] = enumIndex;				//用户编程时输入的字符到枚举类型的映射
		xlStrToEnum[xlStr] = enumIndex;				//从xml读取属性值的时候查询的映射
	}
}

