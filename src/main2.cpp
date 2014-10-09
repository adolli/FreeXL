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
	cout<<"---  �����㼶������ v1.0  ---"<<endl<<endl;

	string filename = "�����";
	string sheetname = "Sheet1";
	string strColLevel = "A";
	string strColQty = "B";
	string strColMass = "C";

	// �� ��xx�ܱ� �� ��sheetname��
	cout<<endl<<"��ȷ�� ������� �Ѿ����ر�"<<endl;
	system("pause");

	cout<<endl<<"���ڶ�ȡ�������Ժ򡭡�"<<endl;
	xlWorkbook wb(filename + ".xml");
	xlSheet& sh = wb.sheets(sheetname);
	cout<<"���ڼ��㡭�����Ժ򡭡�"<<endl;


	const unsigned int COL_LEVEL = A1_STYLE(strColLevel).Col1;			//<! �㼶
	const unsigned int COL_QUANTITY = A1_STYLE(strColQty).Col1;			//<! ����
	const unsigned int COL_MASS = A1_STYLE(strColMass).Col1;			//<! ��������
	const unsigned int COL_CAL_MASS = sh.maxCol() + 1;					//<! ��������

	// ��ȡ�ñ�����к�
	const unsigned int MAX_ROW = sh.maxRow();
	
	// Ĭ��������ʼ��Ϊ��2��
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

	// ��ȡ��׵Ĳ㼶�͸ñ�����к�
	const unsigned int MAX_LEVEL = GetMaxLevel(sh, 2, COL_LEVEL);


	// ����䵥������
	sh.cells(START_ROW - 1, COL_CAL_MASS) = "computedMass";
	for (unsigned int r = MAX_ROW; r >= START_ROW; --r)
	{
		sh.cells(r, COL_CAL_MASS) = sh.cells(r, COL_MASS).dValue();
	}

	// ����׵Ĳ㼶��ʼһ��һ��鲢����
	for (unsigned int level = MAX_LEVEL; level >= 2; --level)
	{
		// ���ڱ���̳�����
		double inheritMass = 0;

		// ÿһ�����ӱ�����һ�����ϵ���ʼ��
		for (unsigned int r = MAX_ROW; r >= START_ROW; --r)
		{
			// �ѵ�ǰ�㼶�������ۼ�����
			if (sh.cells(r, COL_LEVEL).iValue() == level)
			{
				inheritMass += sh.cells(r, COL_CAL_MASS).dValue() * sh.cells(r, COL_QUANTITY).iValue();
			}
			// ������ǰ�㼶����һ��ľͰ��ۼӵ����ݱ��浽����һ��
			else if (sh.cells(r, COL_LEVEL).iValue() == level - 1 && inheritMass != 0)
			{
				sh.cells(r, COL_CAL_MASS) = inheritMass;
				inheritMass = 0;
			}
		}
	}


	cout<<"������ɣ����ڱ��桭�����Ժ򡭡�"<<endl;

	// ���Ϊ
	wb.saveAs("output.xml");
	cout<<"������ϣ� ����ļ���Ϊ ��output.xml��"<<endl;
	cout<<"��� ��output.xml�� �����һ�н�����ȡ��"<<endl;
}


/*
 * @brief	���
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
	cout<<"---  ����������Զ��嵥���� v1.0  ---"<<endl<<endl;

	string filename = "���������";
	string sheetname = "Input";
	string strColLevel = "A";
	string strColId = "B";
	string strColName = "C";
	string strColEng = "D";
	const unsigned int COL_LEVEL = A1_STYLE(strColLevel).Col1;		//<! �㼶
	const unsigned int COL_ID = A1_STYLE(strColId).Col1;			//<! �������
	const unsigned int COL_NAME = A1_STYLE(strColName).Col1;		//<! �������
	const unsigned int COL_ENG = A1_STYLE(strColEng).Col1;			//<! ��Ʒ����ʦ

	cout<<endl<<"��ȷ�� ��" + filename + "�� �Ѿ����ر�"<<endl;
	system("pause");

	cout<<endl<<"���ڽ������ݡ������Ժ򡭡�"<<endl;
	xlWorkbook wb(filename + ".xml");
	xlSheet& sh = wb.sheets(sheetname);
	xlSheet& output_sh = wb.sheets("Output");
	xlSheet& String_sh = wb.sheets("String");
	cout<<"���ڼ��㡭�����Ժ򡭡�"<<endl;


	// �ҳ�Ҫ�õ���String
	map<string, string> stringTable;
	for (unsigned int r = 2; r <= String_sh.maxRow(); ++r)
	{
		stringTable[String_sh.cells(r, 1).value()] = String_sh.cells(r, 2).value();
	}


	const unsigned int MAX_ROW = sh.maxRow();
	const unsigned int MAX_COL = sh.maxCol();
	
	// Ĭ��������ʼ��Ϊ��5��
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
					// �����������ɨ��һ����ڲ�������һ��
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

		// ���һ������
		if (!vParts.empty())
		{
			CombineToSubmitList(mapPartsPerType, vParts, CarQty);
			OutputToSubmitList(mapParts, vParts);
			vParts.clear();
		}
	}	
	
	// ������̨����
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

	cout<<"���ڱ��桭�����Ժ򡭡�"<<endl;
	wb.saveAs(filename + "_out.xml"); 
	cout<<"����ɹ�������� ��" + filename + "_out�� ��ȡ"<<endl;

	system("pause");
	return 0;
}

