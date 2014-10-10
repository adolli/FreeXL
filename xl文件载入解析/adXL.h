// adXL.h

#ifndef _ADXL_H_
#define _ADXL_H_

#include "tinyxml/tinyxml.h"
#include "xmlSel.h"
#include "ImageStone/ImageStone.h"
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;


namespace FreeXL
{
	
	#define		_Temp_T				template<typename T>
	#define		CellType			xlCell::__cell_type.StrToType
	#define		CellTypeStr			xlCell::__cell_type.TypeToStr
	
	
	// enums
	enum ERRORS
	{
		ERR_FAIL_TO_GetValue =unsigned int(-1)
	};
	
	enum ShiftType
	{
		xlShiftUp,
		xlShiftLeft,
		xlDetermineBySize
	};
	
	
	enum CornerCell
	{
		xlTopLeft,
		xlTopRight,
		xlBottomLeft,
		xlBottomRight
	};
	
	// forward decl
	class A1_STYLE;
	class RC_STYLE;
	class xlStyle;
	class xlWorkbook;
	class xlSheet;
	class xlRange;
	class xlRow;
	class xlColumn;
	
	
	
	// global functions
	//////////////////////////////////////////////////////////////////////////////////////////
	void setupMapsByXML(			//加载style类的样式映射的函数
		string xmlFilename,			//xml文件名
		string tagName,				//xml中要读取的标签名
		map<string, int>& strToEnum,	//映射到枚举类型的map
		map<int, string>& enumToStr,	//从枚举映射到string的map
		map<string, int>& xlStrToEnum);
	
	
	class xlStyle
	{
	
		friend class xlCell;
		friend class xlSheet;
		friend class xlWorkbook;
	
	private:
		static string _fileTargetForSettings;							//用来设置映射的xml文件路径
	
	public:
		
		// 以下样式可能有关于某种样式的枚举类型
		// 弄个枚举类型主要是编程的时候有常量参考
		class StyleBase
		{
			friend class xlStyle;
			friend class xlCell;
			friend class xlWorkbook;
		
		private:
			StyleBase();											//默认不允许用户定义这个类
			static bool _alignMapInitialized;							//初次从xml载入映射的标记
			static bool _bordrMapInitialized;								//每个map都必须独立
			static bool _fontMapInitialized;									//同上
			static bool _interiorMapInitialized;								//
			static bool _numberFormatMapInitialized;							//
			static bool _protectionMapInitialized;								//
		
		};
	
		class Alignment_t : public StyleBase										//对齐方式
		{
			friend class xlStyle;
			friend class xlCell;
			friend class xlWorkbook;
		
			enum hAlign_t
			{														//水平对齐类型
				/* there's some type missing as they're not common used*/
				XL_hDefault,														//
				XL_Center,														//水平居中
				XL_Left,														//水平居左
				XL_Right,														//水平居右
				XL_CAcross,														//水平跨列居中
				XL_Justify														//水平两端对齐
			};
			
			enum vAlign_t
			{														//垂直对齐类型
				/* there's some type missing as they're not common used*/
				XL_vDefault, XL_Middle,/*in xl is center, 
				 but in order to distinguish them, we use middle*/			//垂直居中
				XL_Bottom,														//垂直居下
				XL_Top															//垂直居上
			};
		
			static map<string, int> _hAlign;						//用作字符串映射到Align_t
			static map<string, int> _vAlign;
			static map<string, int> _hAlign_xlStr_toEnum;			//用作把xml属性值映射到enum值
			static map<string, int> _vAlign_xlStr_toEnum;
			static map<int, string> _hAlign_toStr;					//用作Align_t映射到string
			static map<int, string> _vAlign_toStr;
			int vertical_;												//垂直对齐方式
			int horizontal_;											//水平对齐方式
			int rotation;												//旋转(范围：-90~+90)
			bool wrapText;												//自动换行
		
		public:
			Alignment_t();											//构造函数用于初始化alignMap
			string getVerticalAlign()
			{
				return _vAlign_toStr[vertical_];
			}
			
			string getHorizontalAlign()
			{
				return _hAlign_toStr[horizontal_];
			}
			
			int getRotation()
			{
				return rotation;
			}
			
			bool isWrappedText()
			{
				return wrapText;
			}
			
			Alignment_t & setVerticalAlign(vAlign_t enumValue);
			Alignment_t & setHorizontalAlign(hAlign_t enumValue);
			Alignment_t & setVerticalAlign(string strValue);
			Alignment_t & setHorizontalAlign(string strValue);
			Alignment_t & setWrappedText(bool value);
			Alignment_t & setRotation(int value);
			Alignment_t & operator=(string expr);
			bool operator==(const Alignment_t& rhs);			//比较两个Alignment_t值是否完全相同
		
		};
		
		class Borders_t : public StyleBase											//边框
		{
			friend class xlStyle;
			friend class xlCell;
			friend class xlWorkbook;
		
			enum LineStyle_t
			{													//边框线型类型
				XL_None,														//无线条
				XL_Continuous,														//实线
				XL_Solid,														//实线（同上）
				XL_Dash,															//虚线
				XL_DashDot,														//点划线
				XL_DashDotDot,													//点点划线
				XL_Dot,																//点
				XL_Double,														//双实线
				XL_SlantDashDot
			};
		
			static map<string, int> _lStyleMap;
			static map<string, int> _lStyleMap_xlStr_toEnum;
			static map<int, string> _lStyleMap_toStr;
			int lineStyle_;												//边框线型
			int weight_;			//边框宽度(取值范围，0~3，但实际上在excel中会自动舍去小数，其实只有4种线厚度0,1,2,3)
			string color_;													//边框颜色
		
		public:
			Borders_t();
			
			string getLineStyle()
			{
				return _lStyleMap_toStr[lineStyle_];
			}
			
			int getBorderWeight()
			{
				return weight_;
			}
			
			string getBorderColor()
			{
				return color_;
			}
			
			Borders_t & setLineStyle(string strValue);
			Borders_t & setLineStyle(LineStyle_t enumValue);
			Borders_t & setBorderWeight(int iValue);
			Borders_t & setBorderColor(string strValue);
			Borders_t & operator=(string expr);
			bool operator==(const Borders_t& rhs);				//比较两个Borders_t值是否完全相同
		
		};
		
		class Font_t : public StyleBase												//字体
		{
			friend class xlStyle;
			friend class xlCell;
			friend class xlWorkbook;
		
			enum UnderlineStyle_t
			{												//下划线类型
				XL_None,														//无下划线
				XL_Single,														//单下划线
				XL_Double														//双下划线
			};
			enum VerticalAlign_t
			{												//上下标类型
				XL_Default,															//常规
				XL_Subscript,														//下标
				XL_Superscript														//上标
			};
		
			static map<string, int> _underLnMap;							//下划线样式映射
			static map<string, int> _underLnMap_xlStr_toEnum;
			static map<int, string> _underLnMap_toStr;					//下划线样式映射到字符串
			static map<string, int> _vAlign;									//上下标映射
			static map<string, int> _vAlign_xlStr_toEnum;
			static map<int, string> _vAlign_toStr;									//
			string name;													//字体名
			string charSet;												//字符集，默认134
			double size_;													//字体大小
			string color_;													//字体颜色
			bool strikeThrough_;											//是否有删除线
			bool italic_;												//是否倾斜
			bool bold_;													//是否加粗
			int underlineStyle_;										//下划线样式
			int vertical_Align_;										//上下标样式
		
		public:
			Font_t();
			
			string getFontName()
			{
				return name;
			}
			
			double getFontSize()
			{
				return size_;
			}
			
			string getFontColor()
			{
				return color_;
			}
			
			bool getStrikeThrough()
			{
				return strikeThrough_;
			}
			
			bool getItalic()
			{
				return italic_;
			}
			
			bool getBold()
			{
				return bold_;
			}
			
			string getUnderlineStyle()
			{
				return _underLnMap_toStr[underlineStyle_];
			}
			
			string getVertical_Align()
			{
				return _vAlign_toStr[vertical_Align_];
			}
			
			Font_t & setUnderlineStyle(string strValue);
			Font_t & setUnderlineStyle(UnderlineStyle_t enumValue);
			Font_t & setVertical_Align(string strValue);
			Font_t & setVertical_Align(VerticalAlign_t enumValue);
			Font_t & setFontName(string value);
			Font_t & setCharSet(string value);
			Font_t & setFontSize(double value);
			Font_t & setFontColor(string value);
			Font_t & setStrikeThrough(bool value);
			Font_t & setItalic(bool value);
			Font_t & setBold(bool value);
			Font_t & operator=(string expr);
			bool operator==(const Font_t& rhs);						//比较两个Font_t值是否完全相同
		
		};
	
		class Interior_t : public StyleBase											//填充
		{
			friend class xlStyle;
			friend class xlCell;
			friend class xlWorkbook;
		
			enum Pattern_t
			{													//填充类型
				XL_None,														//无填充
				XL_Solid,														//实心填充
				XL_Gray75,														//75%灰度
				XL_Gray50,															//50
				XL_Gray25,															//25
				XL_Gray125,														//12.5
				XL_Gray0625,													//6.25
				XL_HorzStripe,													//水平线（粗）
				XL_VertStripe,													//垂直线（粗）
				XL_ReverseDiagStripe,											//副对角线
				XL_DiagStripe,													//主对角线
				XL_ThinHorzStripe,												//水平线（细）
				XL_ThinVertStripe,												//垂直线（细）
				XL_ThinHorzCross,											//水平垂直交叉线（细）
				XL_ThinReverseDiagStripe,									//副对角线（细）
				XL_ThinDiagStripe,//主对角线（细）														
				XL_ThinDiagCross,											//交叉对角线（细）
				XL_DiagCross,													//交叉对角线
				XL_ThickDiagCross											//交叉对角线（粗）
			};
		
			static map<string, int> _patternMap;								//填充样式映射
			static map<string, int> _patternMap_xlStr_toEnum;
			static map<int, string> _patternMap_toStr;
			string color_;													//填充色
			int pattern;												//填充样式
		
		public:
			Interior_t();
			
			string getInteriorColor()
			{
				return color_;
			}
			
			string getInteriorPattern()
			{
				return _patternMap_toStr[pattern];
			}
			
			Interior_t & setInteriorColor(string value);
			Interior_t & setInteriorPattern(string value);
			Interior_t & setInteriorPattern(Pattern_t enumValue);
			Interior_t & operator=(string expr);
			bool operator==(const Interior_t& rhs);				//比较两个Interior_t值是否完全相同
			
		};
	
		class NumberFormat_t : public StyleBase									//数字格式
		{
		public:
		
		};
		
		class Protection_t : public StyleBase									//单元格保护
		{
		public:
		
		};
	
		typedef Borders_t sBorder;
		
		// style members
		Alignment_t alignment;												//对齐
		map<string, sBorder> borders;												//边框
		Font_t font;													//字体
		Interior_t interior;												//填充
		NumberFormat_t numberFormat;										//数字格式(未完善)
		Protection_t protection;											//保护(未完善)
		string styleID;												//ss:StyleID
		
		xlStyle();
		bool operator==(const xlStyle& rhs);						//比较两个style对象值是否完全相同
		
	};
	// typedef change into short-form
	
	typedef xlStyle::StyleBase sBase;
	typedef xlStyle::Alignment_t sAlign;
	typedef xlStyle::Borders_t sBorder;
	typedef xlStyle::Font_t sFont;
	typedef xlStyle::Interior_t sInterior;
	typedef xlStyle::NumberFormat_t sNumFormat;
	typedef xlStyle::Protection_t sProtect;
	
	class Cell															//这个只是单元格坐标类
	{
	
	public:
		int row;
		int col;
		Cell(int r = 0, int c = 0) :
				row(r), col(c)
		{
		}
		
	};
	//////////////////////////////////////////////////////////////////////////////////////////
	class xlCell
	{
		
		friend class xlWorkbook;
		friend class xlSheet;
		friend class xlRange;
	
	public:
		
		enum SS_TYPE
		{
			T_String,				//文本(default type)
			T_Number,				//数字
			T_Date					//日期
		};
		
		static class SS_TYPE_MAP
		{										//该类用于保存单元格类型的映射
		
		private:
			static bool _initialized;
		
		public:
			SS_TYPE_MAP();
			static map<string, SS_TYPE> StrToType;
			static map<SS_TYPE, string> TypeToStr;
		
		} __cell_type;
		
		protected:
			SS_TYPE_MAP __TO_INITIALIZE_CellTYPE;						//用于初始化SS_TYPE_MAP
			string data;													//单元格数据
			SS_TYPE ss_type;												//单元格数据类型
			UINT _rowIndex;												//行标
			UINT _colIndex;												//列标
			xlStyle _style;													//单元格style对象
			xlSheet * _pSh;													//指向外层的sheet对象
		
		private:
			xlCell(UINT rIndex, UINT cIndex, xlSheet* psh);
			xlCell(TiXmlNode* _cell, $ xQ);							//直接在解析cells
			xlCell & parseToCells(TiXmlNode* _cell);					//从Cell节点解析到xlCell对象
			xlCell & parseToStyle($& container);							//从xml解析单元格style
		
		public:
			xlCell(string _data = "", SS_TYPE type = T_String);
			xlCell(int _data);
			xlCell(double _data);
		
		_Temp_T inline bool getValue(T& assignedTo)
		{
			stringstream sstream(data);
			sstream >> assignedTo;
			if (sstream.fail())
				return false;
			else
				return true;
		}																//读取单元格值(inline)
		
		_Temp_T inline bool setValue(const T& setBy)
		{
			stringstream sstream;
			sstream<<setBy;
			data=sstream.str();
			if ( sstream.fail() ) return false;
			else
			this->ss_type=CellType[typeid(T).name()];
			return true;
		}																		//设置单元格值
		
		_Temp_T inline xlCell & operator=(const T& setBy)
		{
			setValue(setBy);
			return *this;
		}
		
		string value();												//取字符串值
		int iValue();//取整数
		double dValue();//取浮点数
		string getValueType();//返回Excel中的数据类型
		inline UINT rowIndex();//返回行号，1开始
		inline UINT colIndex();//返回列号，1开始
		
		xlStyle & style();//返回单元格的style样式对象
		string getStyleID();
		xlCell & setStyleBy(const xlStyle& _style);//设置Style
		
		inline xlCell & printCell();//在屏幕上显示单元格内容
		inline bool isEmpty();//单元格为空？
		xlCell & clear();//清楚单元格内容，但保留样式
		xlCell & clearStyle();//清楚单元格样式，但保留文本内容
		void Delete(ShiftType _type=xlShiftUp);
	
	};
	
	
	//////////////////////////////////////////////////////////////////////////////////////////
	class xlSheet
	{
		
		friend class xlCell;
		friend class xlWorkbook;
		friend class xlRange;
		typedef vector<vector<xlCell> > SS_TABLE;
	
	protected:
		SS_TABLE ss_table;												//单元格表
		string _sheetName;												//工作表名称
		xlWorkbook * _pWB;											//指向所在外壳Workbook
	
	public:
		xlSheet();
		xlSheet(string _name, xlWorkbook* _pwb = nullptr);
	
		xlCell & cells(int r, int c);										//读取单元格
		xlCell & cells(string a1_style);									//读取单元格
	
		xlRange range(int r1, int c1, int r2, int c2);				//返回该表中的一个区域引用
		xlRange range(Cell cell1, Cell cell2);
		xlRange range(string a1_style);
	
		xlRow row(UINT startIndex, UINT endIndex = 0);				//返回表中区域的引用（行）
		xlRow row(string a1_style);
	
		xlColumn column(UINT startIndex, UINT endIndex = 0);		//返回表中区域的引用（列）
		xlColumn column(string a1_style);
	
		const UINT maxRow() const;
		const UINT maxCol() const;
		xlSheet & setMaxRow(UINT r);										//设置最大行数
		xlSheet & setMaxCol(UINT c);										//设置最大列数
		inline string sheetName();									//返回sheet name
		inline xlSheet & sheetName(string setName);					//设置sheet name
	
		xlSheet & printSheet();
	
		xlWorkbook & Delete();												//删除该工作表
		xlSheet & DeleteCell(int r, int c, ShiftType _type = xlShiftUp);	//删除单元格
		xlSheet & DeleteCell(Cell cell, ShiftType _type = xlShiftUp);
		xlSheet & DeleteRange(int r1, int c1, int r2, int c2, ShiftType _type = xlDetermineBySize);		//删除选区
		xlSheet & DeleteRange(Cell cell1, Cell cell2, ShiftType _type = xlDetermineBySize);
		xlSheet & DeleteRange(string a1_Style, ShiftType _type = xlDetermineBySize);
	
	};
	
	//////////////////////////////////////////////////////////////////////////////////////////
	class xlWorkbook
	{
		
		friend class xlSheet;
		friend class xlCell;
		friend class xlStylePool;
	
	protected:
		string _filename;
		$ XL;														//装载了xml电子表格的选择器
		vector<xlSheet> _theSheets;
		map<int, xlStyle> _style_map;						//styleID到xlStyle对象的映射
	
	public:
		xlWorkbook(string filename = "");
		xlWorkbook & load(string filename);									//载入文件
		xlWorkbook & save();												//保存文件
		xlWorkbook & saveAs(string filename);								//另存为文件
		int registerStyle(const xlStyle& _style);			//向样式池里注册一个样式，返回注册后的样式ID
	
		xlSheet & newSheet(string _sheetName);							//新建sheet
		xlWorkbook & newSheets(int number_of_new);						//新建n个工作表
		xlWorkbook & DeleteSheet(string _sheetName);					//删除sheet
		xlWorkbook & DeleteSheet(int index);
		xlSheet & sheets(int index);
		xlSheet & sheets(string sheetName);
		UINT sheetsCount() const;									//返回工作表个数
		bool hasSheet(string name) const;								//测试是否有该工作表
	
	private:
		xlWorkbook & parseToSheets();										//解析文件
		xlWorkbook & inverseParse();					//反解析，即把内存中的excel转换到xml电子表格
		xlWorkbook & parseToStylePool();							//从xml读取类型到类型池中
		xlWorkbook & parseDefaultStyle();	//在xml文件中读取default样式并保存在style_map[0]的位置
	
	};
	
	//////////////////////////////////////////////////////////////////////////////////////////
	class xlRange
	{
		
		typedef vector<vector<xlCell*> > pSS_TABLE;
	
	protected:
		pSS_TABLE p_table;
		xlSheet * _pSh;													//指向sheet的指针
	
	public:
		xlRange(xlSheet* _xlsheet, int r1, int c1, int r2, int c2);
		xlRange(xlSheet* _xlsheet, string a1_style);
		xlRange(xlSheet* _xlsheet, Cell cell1, Cell cell2);
		
		~xlRange()
		{
		}											//析构时不做任何事情
	
		_Temp_T inline xlRange & setValue(const T& setBy)
		{
			for (UINT r = 0; r < p_table.size(); r++)
			{
				for (UINT c = 0; c < p_table[r].size(); c++)
				{
					p_table[r][c]->setValue(setBy);
				}
			}
			return *this;
		}														//给range中每一个cell赋值
		
		_Temp_T inline xlRange & operator=(const T& setBy)
		{
			return setValue(setBy);
		}
		
		xlCell & at(UINT rel_r,UINT rel_c);
		xlCell & corner(CornerCell _corner);						//返回选区角落的单元格引用
	
		UINT rangeWidth();			//选区宽度
		UINT rangeHeight();			//选区大小
	
		xlRange & setStyleBy(const xlStyle& _style);			//设置Style
		xlRange & clear();												//清除数据
		xlRange & clearStyle();										//清楚样式
	
		xlRange & offset(int r_shift,int c_shift);//将选区相对偏移
	
	private:
		xlRange & __Initialize_p_Range(xlSheet* _xlsheet,int r1,int c1,int r2,int c2);//用于在构造函数中减少代码量
	
	};
	
	///////////////////////////////////////////////////////////////////////////////////////////
	class xlRow : public xlRange
	{
	
	protected:
		UINT _startRowIndex;											//起始行号
		int _span;									//共有行数(lastrow=startrow+span-1)
	
	public:
		xlRow(xlSheet* _xlsheet, UINT startRowIndex, UINT endRowIndex = 0);
		xlRow(xlSheet* _xlsheet, string a1_style);
		
		_Temp_T inline xlRange & operator=(const T& setBy)
		{
			return setValue(setBy);
		}
	
	};
	
	
	class xlColumn: public xlRange
	{
	
	protected:
		UINT _startColIndex;											//起始列号
		int _span;									//共有列数(lastrow=startrow+span-1)
	
	public:
		xlColumn(xlSheet* _xlsheet, UINT startColIndex, UINT endColIndex = 0);
		xlColumn(xlSheet* _xlsheet, string a1_style);_Temp_Tinline
		xlRange & operator=(const T& setBy)
		{
			return setValue(setBy);
		}
	
	};
	
	//////////////////////////////////////////////////////////////////////////////////////////
	class A1_STYLE									//用来处理A1引用样式选择的字符串（e.g. A1:B3）
	{
		
		friend class RC_STYLE;
	
	private:
		string _expr;
	
	public:
		static string ALPHABET;
		int Row1;													//起始行号
		int Col1;
		int Row2;													//终止行号
		int Col2;
		map<string, bool> absolute;
		A1_STYLE(const string a1_style = "");
		A1_STYLE & operator=(const string a1_style);
		A1_STYLE & operator=(RC_STYLE rc_style);
		RC_STYLE convertToR1C1(string exp = "");
		bool isValid(string exp = "");
		bool isRange();
	
	};
	
	class RC_STYLE
	{
		
		friend class A1_STYLE;
	
	private:
		string _expr;
	
	public:
		int Row1;													//起始行号
		int Col1;
		int Row2;													//终止行号
		int Col2;
		map<string, bool> relativity;
		RC_STYLE(const string rc_style = "");
		RC_STYLE & operator=(const string rc_style);
		RC_STYLE & operator=(A1_STYLE a1_style);
		A1_STYLE convertToA1(string exp = "");
		bool isValid(string exp = "");
	
	};

} /*end of namespace FreeXL*/

#endif  /*_ADXL_H_*/
