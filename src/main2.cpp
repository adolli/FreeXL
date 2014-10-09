// main.cpp

#include "adXL.h"
#include <iostream>
#include <vector>
#include <map>
using namespace std;
using namespace FreeXL;




int GetMaxLevel(xlSheet& sheet, unsigned int startRow, unsigned int col_index)
{
	const unsigned int MAX_ROW = sheet.maxRow();
	unsigned int max_level = 0;
	for (unsigned int r = startRow; r <= MAX_ROW; ++r)
	{
		max_level = max(max_level, sheet.cells(r, col_index).iValue());
	}
	return max_level;
}

void BatchLevelCalc()
{
	cout<<"---  批量层级处理工具 v1.0  ---"<<endl<<endl;

	string filename = "输入表";
	string sheetname = "Sheet1";
	string strColLevel = "A";
	string strColQty = "B";
	string strColMass = "C";

	// 打开 “xx总表” 的 “sheetname”
	cout<<endl<<"请确保 “输入表” 已经被关闭"<<endl;
	system("pause");

	cout<<endl<<"正在读取……请稍候……"<<endl;
	xlWorkbook wb(filename + ".xml");
	xlSheet& sh = wb.sheets(sheetname);
	cout<<"正在计算……请稍候……"<<endl;


	const unsigned int COL_LEVEL = A1_STYLE(strColLevel).Col1;			//<! 层级
	const unsigned int COL_QUANTITY = A1_STYLE(strColQty).Col1;			//<! 数量
	const unsigned int COL_MASS = A1_STYLE(strColMass).Col1;			//<! 单个质量
	const unsigned int COL_CAL_MASS = sh.maxCol() + 1;					//<! 计算质量

	// 获取该表最大行号
	const unsigned int MAX_ROW = sh.maxRow();
	
	// 默认数据起始行为第2行
	unsigned int START_ROW = 2;

	for (unsigned int r = 2; r <= MAX_ROW; ++r)
	{
		unsigned int level = sh.cells(r, COL_LEVEL).iValue();
		if (level <= 100 && level >= 1)
		{
			START_ROW = r;
			break;
		}
	}

	// 获取最底的层级和该表最大行号
	const unsigned int MAX_LEVEL = GetMaxLevel(sh, 2, COL_LEVEL);


	// 先填充单件质量
	sh.cells(START_ROW - 1, COL_CAL_MASS) = "computedMass";
	for (unsigned int r = MAX_ROW; r >= START_ROW; --r)
	{
		sh.cells(r, COL_CAL_MASS) = sh.cells(r, COL_MASS).dValue();
	}

	// 从最底的层级开始一层一层归并计算
	for (unsigned int level = MAX_LEVEL; level >= 2; --level)
	{
		// 用于保存继承质量
		double inheritMass = 0;

		// 每一层计算从表的最后一行往上到起始行
		for (unsigned int r = MAX_ROW; r >= START_ROW; --r)
		{
			// 把当前层级的数据累加起来
			if (sh.cells(r, COL_LEVEL).iValue() == level)
			{
				inheritMass += sh.cells(r, COL_CAL_MASS).dValue() * sh.cells(r, COL_QUANTITY).iValue();
			}
			// 遇到当前层级更高一层的就把累加的数据保存到更高一层
			else if (sh.cells(r, COL_LEVEL).iValue() == level - 1 && inheritMass != 0)
			{
				sh.cells(r, COL_CAL_MASS) = inheritMass;
				inheritMass = 0;
			}
		}
	}


	cout<<"计算完成，正在保存……请稍候……"<<endl;

	// 另存为
	wb.saveAs("output.xml");
	cout<<"保存完毕！ 输出文件名为 “output.xml”"<<endl;
	cout<<"请打开 “output.xml” 的最后一列将数据取走"<<endl;
}


/*
 * @brief	零件
 */
class Part
{
public:
	Part(string id, string name, string engineer, int qty)
		: id_(id), name_(name), productEngineer(engineer), quantity(qty)
	{
	}

	string id_;
	string name_;
	string productEngineer;
	int quantity;

};


class ListItem
{
public:

	ListItem()
		: quantity(0)
	{}

	string name_;
	string productEngineer;
	int quantity;
};

void OutputToSubmitList(map<string, ListItem>& m, const vector<Part>& p)
{
	for (auto it = p.begin(); it != p.end(); ++it)
	{
		m[it->id_].name_ = it->name_;
		m[it->id_].productEngineer = it->productEngineer;
		m[it->id_].quantity += it->quantity;
	}
}

void CombineToSubmitList(map<string, ListItem>& m, const vector<Part>& p, unsigned int nCars)
{
	for (auto it = p.begin(); it != p.end(); ++it)
	{
		m[it->id_].name_ = it->name_;
		m[it->id_].productEngineer = it->productEngineer;
		m[it->id_].quantity = max(m[it->id_].quantity, it->quantity / nCars);
	}
}

int main()
{
	cout<<"---  零件基本表自动清单工具 v1.0  ---"<<endl<<endl;

	string filename = "零件基本表";
	string sheetname = "Input";
	string strColLevel = "A";
	string strColId = "B";
	string strColName = "C";
	string strColEng = "D";
	const unsigned int COL_LEVEL = A1_STYLE(strColLevel).Col1;		//<! 层级
	const unsigned int COL_ID = A1_STYLE(strColId).Col1;			//<! 零件代码
	const unsigned int COL_NAME = A1_STYLE(strColName).Col1;		//<! 零件名称
	const unsigned int COL_ENG = A1_STYLE(strColEng).Col1;			//<! 产品工程师

	cout<<endl<<"请确保 “" + filename + "” 已经被关闭"<<endl;
	system("pause");

	cout<<endl<<"正在解析数据……请稍候……"<<endl;
	xlWorkbook wb(filename + ".xml");
	xlSheet& sh = wb.sheets(sheetname);
	xlSheet& output_sh = wb.sheets("Output");
	xlSheet& String_sh = wb.sheets("String");
	cout<<"正在计算……请稍候……"<<endl;


	// 找出要用到的String
	map<string, string> stringTable;
	for (unsigned int r = 2; r <= String_sh.maxRow(); ++r)
	{
		stringTable[String_sh.cells(r, 1).value()] = String_sh.cells(r, 2).value();
	}


	const unsigned int MAX_ROW = sh.maxRow();
	const unsigned int MAX_COL = sh.maxCol();
	
	// 默认数据起始行为第5行
	const unsigned int START_ROW = 5;
	const unsigned int START_COL = 5;

	vector<Part> vParts;
	map<string, ListItem> mapParts;
	map<string, ListItem> mapPartsPerType;
	
	for (unsigned int c = START_COL; c <= MAX_COL; ++c)
	{
		unsigned int FatherQty = 0;
		unsigned int CarQty = sh.cells(2, c).iValue();
		for (unsigned int r = START_ROW; r <= MAX_ROW; ++r)
		{
			if (sh.cells(r, COL_LEVEL).iValue() == 1)
			{
				if (!vParts.empty())
				{
					// 假设把整个表扫描一遍后内部的排序都一样
					CombineToSubmitList(mapPartsPerType, vParts, CarQty);
					OutputToSubmitList(mapParts, vParts);
					vParts.clear();
				}
				FatherQty = sh.cells(r, c).iValue();
			}
			else
			{
				vParts.push_back(
					Part(
					sh.cells(r, COL_ID).value(), 
					sh.cells(r, COL_NAME).value(), 
					sh.cells(r, COL_ENG).value(), 
					sh.cells(r, c).iValue() * FatherQty * CarQty));
			}
		}

		// 最后一批数据
		if (!vParts.empty())
		{
			CombineToSubmitList(mapPartsPerType, vParts, CarQty);
			OutputToSubmitList(mapParts, vParts);
			vParts.clear();
		}
	}	
	
	// 导出单台用量
	output_sh.cells(1, 5) = "single usage";
	unsigned int rr = 2;
	for (auto it = mapPartsPerType.begin(); it != mapPartsPerType.end(); ++it)
	{
		if (!it->first.empty())
		{
			output_sh.cells(rr, 5) = it->second.quantity;

			// move to next line
			++rr;
		}
	}

	output_sh.cells(1, 6) = "Total Usage";
	output_sh.cells(1, 7) = "Product Engineer";
	rr = 2;
	for (auto it = mapParts.begin(); it != mapParts.end(); ++it)
	{
		if (!it->first.empty())
		{
			output_sh.cells(rr, 1) = rr - 1;
			output_sh.cells(rr, COL_ID) = it->first;
			output_sh.cells(rr, COL_NAME) = it->second.name_;
			output_sh.cells(rr, 6) = it->second.quantity;
			output_sh.cells(rr, 7) = it->second.productEngineer;
			output_sh.cells(rr, 4) 
				= stringTable.find("sample_status") != stringTable.end() ? stringTable["sample_status"] : "undefined";
			
			// move to next line
			++rr;
		}
	}

	cout<<"正在保存……请稍候……"<<endl;
	wb.saveAs(filename + "_out.xml"); 
	cout<<"保存成功……请打开 “" + filename + "_out” 读取"<<endl;

	system("pause");
	return 0;
}

