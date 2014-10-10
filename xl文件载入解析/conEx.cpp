// conEx.cpp

#include "xmlSel.h"
using namespace std;

// conEx //////////////////////////////////////////////////////////////////////////////
//

// constant and static
string conEx::blank = "__THE_CONDITION_EXPRESSION_IS_BLANK__";

// parser ////////////////////////////////////////////////////////
conExParser::conExParser(string exp) :
		originExp(exp)
{
	parse(exp);
}

conExParser& conExParser::parse(string exp)
{
	vecExp = split(exp, ',');
	return *this;
}

vector<TiXmlNode*> conExParser::match(TiXmlNode* root)
{
	int counter = 0;
	vector<TiXmlNode*> ret;
	for (UINT i = 0; i < vecExp.size(); i++)
	{
		int gPos = string::npos, sPos = string::npos;
		gPos = vecExp[i].find('>');
		sPos = vecExp[i].find(' ');

		int pos = string::npos;
		if (sPos == string::npos && gPos != string::npos)
		{
			pos = gPos;
		}
		else if (sPos != string::npos && gPos == string::npos)
		{
			pos = sPos;
		}
		else if (sPos != string::npos && gPos != string::npos)
		{
			pos = min(gPos, sPos);
		}

		if (pos != string::npos && pos == gPos)
		{
			conEx father(vecExp[i].substr(0, pos));
			conExParser son(vecExp[i].substr(pos + 1));
			vector<TiXmlNode*> fatherResult = father.matchTree(root);
			for (UINT j = 0; j < fatherResult.size(); j++)
			{
				VecCombine(ret, son.matchOnce(fatherResult[j]));
			}
		}
		else if (pos != string::npos && pos == sPos)
		{
			conEx father(vecExp[i].substr(0, pos));
			conExParser son(vecExp[i].substr(pos + 1));
			// father 和 son都matchTree
			vector<TiXmlNode*> fatherResult = father.matchTree(root);
			for (UINT j = 0; j < fatherResult.size(); j++)
			{
				VecCombine(ret, son.match(fatherResult[j]));
			}
		}
		else
		{
			conEx result(vecExp[i]);
			VecCombine(ret, result.matchTree(root));
		}
	}
	return ret;
}

vector<TiXmlNode*> conExParser::matchOnce(TiXmlNode* root)
{
	int counter = 0;
	vector<TiXmlNode*> ret;
	for (UINT i = 0; i < vecExp.size(); i++)
	{
		int gPos = string::npos, sPos = string::npos;
		gPos = vecExp[i].find('>');
		sPos = vecExp[i].find(' ');

		int pos = string::npos;
		if (sPos == string::npos && gPos != string::npos)
		{
			pos = gPos;
		}
		else if (sPos != string::npos && gPos == string::npos)
		{
			pos = sPos;
		}
		else if (sPos != string::npos && gPos != string::npos)
		{
			pos = min(gPos, sPos);
		}

		if (pos != string::npos && pos == gPos)
		{
			conEx father(vecExp[i].substr(0, gPos));
			conExParser son(vecExp[i].substr(gPos + 1));
			vector<TiXmlNode*> fatherResult = father.matchOnce(root); //因为表达式要求matchOnce，所以这里也是matchOnce
			for (UINT j = 0; j < fatherResult.size(); j++)
			{
				VecCombine(ret, son.matchOnce(fatherResult[j]));
			}
		}
		else if (pos != string::npos && pos == sPos)
		{
			conEx father(vecExp[i].substr(0, pos));
			conExParser son(vecExp[i].substr(pos + 1));
			vector<TiXmlNode*> fatherResult = father.matchOnce(root);
			for (UINT j = 0; j < fatherResult.size(); j++)
			{
				VecCombine(ret, son.match(fatherResult[j]));
			}
		}
		else
		{
			conEx result(vecExp[i]);
			VecCombine(ret, result.matchOnce(root)); //这里也要对应matchOnce(处理这样的结尾A>B)
		}
	}
	return ret;
}

////////////////////////////////////////////////////////////////
// conEx
// constructor
conEx::conEx(string exp, bool except) :
		exceptThisConEx(except)
{
	pConExGp = NULL;
	pConExGp = new conExGp();
	testAttrName = testAttrValue = testAttrMatchType = testTagName = "";
	parse(exp);
}

conEx& conEx::parse(string exp)
{
	if (exp == "")
		exp = "*";
	removeSpaceBeforeGT(exp);
	int pos = 0;
	if (exp.find("!(") != string::npos)
	{
		// 出现此记号表示不满足此条件的匹配成功
		bool toExcept = false;
		string l = "", r = "";
		if (exp.find("!(") != string::npos)
		{
			removeBraketPairFrom(exp, "!(", ")", l, r);
			toExcept = true;
		}
		pConExGp->conexGp.push_back(conEx(l));
		pConExGp->conexGp.push_back(conEx(r));
		pConExGp->conexGp.push_back(conEx(exp, toExcept));
		pConExGp->conexGp[2].pConExGp->exceptThisConExGp = toExcept;
		pConExGp->_conExRelation = conEx::CONEX_AND; //这里暂且这样，不扩展太多功能
	}
	else if ((pos = exp.find('.')) != string::npos)
	{
		vector < string > exps = split(exp, '.');
		*this = conEx(exps[0]);
		for (UINT j = 1; j < exps.size(); j++)
		{
			filterExGp.push_back(filterEx(exps[j]));
			for (UINT i = 0; i < this->pConExGp->conexGp.size(); i++)
			{
				pConExGp->conexGp[i].filterExGp.push_back(filterEx(exps[j]));
			}
		}
	}
	else if (exp.find('|') != string::npos)
	{
		*pConExGp = conExSplit(exp, '|');
		pConExGp->_conExRelation = conEx::CONEX_OR;
	}
	else if (exp.find('&') != string::npos)
	{
		*pConExGp = conExSplit(exp, '&');
		pConExGp->_conExRelation = conEx::CONEX_AND;
	}
	else
	{
		if (exp[0] == '!')
		{
			pConExGp->_conExRelation = conEx::CONEX_NOT;
			exp = exp.substr(1);
		}
		if (exp.find('#') != string::npos)
		{
			// id
			testAttrName = "id";
			testAttrValue = exp.substr(1);
			testAttrMatchType = "=";
		}
		else if (exp.find('[') != string::npos && exp.find(']') != string::npos)
		{
			// attribute
			int pos = 0, vpos = 0;	//pos用来标记attrName的末位置，vpos用来标记attrValue的开始位置
			exp = exp.substr(1, exp.length() - 2);
			if ((vpos = pos = exp.find("!=")) != string::npos)
			{
				testAttrMatchType = "!=";
			}
			else if ((vpos = pos = exp.find("*=")) != string::npos)
			{
				testAttrMatchType = "*=";
			}
			else if ((vpos = pos = exp.find("^=")) != string::npos)
			{
				testAttrMatchType = "^=";
			}
			else if ((vpos = pos = exp.find("$=")) != string::npos)
			{
				testAttrMatchType = "$=";
			}
			else if ((vpos = pos = exp.find("~=")) != string::npos)
			{
				testAttrMatchType = "~=";
			}
			else if ((vpos = pos = exp.find("=")) != string::npos)
			{
				testAttrMatchType = "=";
				vpos--;
			}
			else
			{
				vpos = exp.length() - 2;
			}
			testAttrName = exp.substr(0, pos);
			testAttrValue = exp.substr(vpos + 2);
		}
		else
		{
			// tagName or value of other tpye of node
			testTagName = exp;
		}
		pConExGp->conexGp.push_back(*this);				//最后conexGp中只有一个元素conEx
	}

	return *this;
}

bool conEx::match(TiXmlNode* node)
{
	if (!node)
		return false;

	if (testTagName != "")
	{
		if (node->ValueStr() == testTagName || testTagName == "*")
		{
			return true && !exceptThisConEx || false && exceptThisConEx;
		}
		else
		{
			return false && !exceptThisConEx || true && exceptThisConEx;
		}
	}

	string value = "";
	if (testAttrName != "")
	{
		if (!node->ToElement())
			return false && !exceptThisConEx || true && exceptThisConEx;
		if (TIXML_SUCCESS
				!= node->ToElement()->QueryValueAttribute(testAttrName, &value))
		{
			return false && !exceptThisConEx || true && exceptThisConEx;
		}
		else if (testAttrValue != "")
		{
			if (testAttrMatchType == "!=" && value != testAttrValue
					|| testAttrMatchType == "*="
							&& value.find(testAttrValue) != string::npos
					|| testAttrMatchType == "^="
							&& value.find(testAttrValue) == 0
					|| testAttrMatchType == "$="
							&& value.find_last_of(testAttrValue)
									== value.length() - 1
					|| //这一条要测试一下
					testAttrMatchType == "~="
							&& testAttrValue.find(value) != string::npos
					|| testAttrMatchType == "=" && value == testAttrValue)
			{
				return true && !exceptThisConEx || false && exceptThisConEx;
			}
			else
			{
				return false && !exceptThisConEx || true && exceptThisConEx;
			}
		}
		else
		{
			return true && !exceptThisConEx || false && exceptThisConEx;
		}
	}

	return false && !exceptThisConEx || true && exceptThisConEx; // 直到最后一个条件都没法判断的，那么只好false了。
}

conExGp& conEx::conExSplit(string exp, char spSymbol)
{
	if (pConExGp)
	{
		pConExGp->conexGp.clear();
		pConExGp->_conExRelation = conEx::CONEX_DEFAULT;
	}
	else
	{
		cout << "pConExGp is NULL! check the code" << endl;
		return *(new conExGp());
	}
	vector < string > strings = split(exp, spSymbol);
	for (UINT i = 0; i < strings.size(); i++)
	{
		pConExGp->conexGp.push_back(conEx(strings[i]));
	}
	return *pConExGp;
}

bool conEx::filterDone()
{
	for (UINT i = 0; i < filterExGp.size(); i++)
	{
		if (!filterExGp[i]._used)
		{
			// 只要有一个filter没被使用，那就肯定还没filte完
			return false;
		}
	}
	return true;
}

vector<TiXmlNode*> conEx::matchTree(TiXmlNode* root)
{
	vector<TiXmlNode*> ret;
	if (!root)
		return ret;

	if (pConExGp->conexGp[0].filterExGp.empty())
	{
		TiXmlNode* loopNode = root->FirstChild();
		while (loopNode)
		{
			// 遍历函数不遍历文本节点，访问文本节点请用.text()
			if (loopNode->Type() != $::XML_TEXT && pConExGp->gpMatch(loopNode))
			{
				ret.push_back(loopNode);
			}
			VecCombine(ret, matchTree(loopNode));
			loopNode = loopNode->NextSibling();
		}
	}
	else
	{
		// 这里用非递归算法，上面保留(测试过，递归算法反而效率高%1)
		for (UINT i = 0; i < filterExGp.size(); i++)
		{
			// 在最前面也把filter全部生效！
			filterExGp[i]._used = false;
		}
		int matchCnt = 0;
		vector<TiXmlNode*> stack;
		TiXmlNode* tempNode = NULL;
		TiXmlNode* saveTheLastNode = NULL;
		stack.push_back(root);
		while (!stack.empty())
		{
			tempNode = stack.back();
			stack.pop_back();
			if (tempNode->Type() != $::XML_TEXT && pConExGp->gpMatch(tempNode))
			{
				// match到节点的处理部分
				matchCnt++;
				for (UINT i = 0; i < filterExGp.size(); i++)
				{  //遍历filter，把满足filter的都集中起来
					if (filterExGp[i].filterType == conEx::FT_AT
							&& filterExGp[i].filterValue == matchCnt
							|| filterExGp[i].filterType == conEx::FT_EVEN
									&& matchCnt % 2 == 0
							|| filterExGp[i].filterType == conEx::FT_ODD
									&& matchCnt % 2 == 1
							|| filterExGp[i].filterType == conEx::FT_FIRST
									&& matchCnt == 1)
					{
						filterExGp[i]._used = true;
						ret.push_back(tempNode);
						break;  //这个break表示或关系的filter,在匹配到一个之后可以不用再测试别的条件
					}
					else if (filterExGp[i].filterType == conEx::FT_NODETYPE
							&& filterExGp[i].testNodeType == tempNode->Type()
							|| filterExGp[i].filterType == conEx::FT_NOT_AT
									&& filterExGp[i].filterValue != matchCnt)
					{
						//在这一层判断里需要把所有match到的节点都判断一次，所以用过的filter不会被删除
						ret.push_back(tempNode);
						break;
					}
					if (filterExGp[i].filterType == conEx::FT_LAST)
					{
						// 选取last这个要另外处理
						// 在上面的if分支中已经有break跳出循环，故匹配到的节点不需要在这里重复
						// 最后只需呀判断saveTheLastNode是否存在，推入ret中就可以了
						saveTheLastNode = tempNode;
					}
				}
				if (filterDone())
				{
					if (saveTheLastNode)
					{
						ret.push_back(saveTheLastNode);
					}
					return ret;
				}
			}
			// 把该节点的子节点全部入栈，顺序遍历则从后往前入栈
			for (TiXmlNode* child = tempNode->LastChild(); child;
					child = child->PreviousSibling())
			{
				stack.push_back(child);
			}
		}
		for (UINT i = 0; i < filterExGp.size(); i++)
		{
			// 在最后面把filter全部生效！
			filterExGp[i]._used = false;
		}
		if (saveTheLastNode)
		{
			ret.push_back(saveTheLastNode);
		}
	}
	return ret;
}

vector<TiXmlNode*> conEx::matchOnce(TiXmlNode* root)
{
	// 仅对孩子节点遍历，所以都不需要递归
	vector<TiXmlNode*> ret;
	if (pConExGp->conexGp[0].filterExGp.empty())
	{
		for (TiXmlNode* loopNode = root->FirstChild(); loopNode; loopNode =
				loopNode->NextSibling())
		{
			// 遍历函数不遍历文本节点，访问文本节点请用.text()
			if (loopNode->Type() != $::XML_TEXT && pConExGp->gpMatch(loopNode))
			{
				ret.push_back(loopNode);
			}
		}
	}
	else
	{
		for (UINT i = 0; i < filterExGp.size(); i++)
		{
			// 在最前面也把filter全部生效！
			filterExGp[i]._used = false;
		}
		int matchCnt = 0;
		TiXmlNode* saveTheLastNode = NULL;
		for (TiXmlNode* loopNode = root->FirstChild(); loopNode; loopNode =
				loopNode->NextSibling())
		{
			// 遍历函数不遍历文本节点，访问文本节点请用.text()
			if (loopNode->Type() != $::XML_TEXT && pConExGp->gpMatch(loopNode))
			{
				matchCnt++;
				for (UINT i = 0; i < filterExGp.size(); i++)
				{						//遍历filter，把满足filter的都集中起来
					if (filterExGp[i].filterType == conEx::FT_AT
							&& filterExGp[i].filterValue == matchCnt
							|| filterExGp[i].filterType == conEx::FT_EVEN
									&& matchCnt % 2 == 0
							|| filterExGp[i].filterType == conEx::FT_ODD
									&& matchCnt % 2 == 1
							|| filterExGp[i].filterType == conEx::FT_FIRST
									&& matchCnt == 1)
					{
						filterExGp[i]._used = true;
						ret.push_back(loopNode);
						break;		//这个break表示或关系的filter,在匹配到一个之后可以不用再测试别的条件
					}
					else if (filterExGp[i].filterType == conEx::FT_NODETYPE
							&& filterExGp[i].testNodeType == loopNode->Type()
							|| filterExGp[i].filterType == conEx::FT_NOT_AT
									&& filterExGp[i].filterValue != matchCnt)
					{
						//在这一层判断里需要把所有match到的节点都判断一次，所以用过的filter不会被删除
						ret.push_back(loopNode);
						break;
					}
					if (filterExGp[i].filterType == conEx::FT_LAST)
					{
						// 选取last这个要另外处理
						// 在上面的if分支中已经有break跳出循环，故匹配到的节点不需要在这里重复
						// 最后只需呀判断saveTheLastNode是否存在，推入ret中就可以了
						saveTheLastNode = loopNode;
					}
				}
				if (filterDone())
				{
					if (saveTheLastNode)
					{
						ret.push_back(saveTheLastNode);
					}
					return ret;
				}
			}
		}
		for (UINT i = 0; i < filterExGp.size(); i++)
		{
			// 在最后面把filter全部生效！
			filterExGp[i]._used = false;
		}
		if (saveTheLastNode)
		{
			ret.push_back(saveTheLastNode);
		}
	}
	return ret;
}

//////////////////////////////////////////////////////////////////
// conExGp
conExGp::conExGp()
{
	_conExRelation = conEx::CONEX_DEFAULT;
	exceptThisConExGp = false;
}

bool conExGp::gpMatch(TiXmlNode* node)
{
	if (conexGp.empty())
		return true;  //关于空节点到底属于false还是true？？还应该仔细考虑
	if (conexGp.size() == 1)
	{
		switch (_conExRelation)
		{
		case conEx::CONEX_NOT:
			return !conexGp[0].match(node);
		default:
			return conexGp[0].match(node);
		}
	}

	switch (_conExRelation)
	{
	case conEx::CONEX_AND:
		for (UINT i = 0; i < conexGp.size(); i++)
		{
			if (!conexGp[i].pConExGp->gpMatch(node))
			{
				return false && !exceptThisConExGp || true && exceptThisConExGp;
			}
		}
		break;
	case conEx::CONEX_OR:
		for (UINT i = 0; i < conexGp.size(); i++)
		{
			if (conexGp[i].pConExGp->gpMatch(node))
			{
				return true && !exceptThisConExGp || false && exceptThisConExGp;
			}
		}
		return false && !exceptThisConExGp || true && exceptThisConExGp;
	default:
		return this->conexGp[0].match(node);
	}
	return true && !exceptThisConExGp || false && exceptThisConExGp;// 其余情况都为false，或关系为default
}

/////////////////////////////////////////////////////////
// filterEx
filterEx::filterEx(string exp)
{
	testNodeType = $::NT_AllType;
	filterType = conEx::FT_ALL;
	filterValue = 0;
	_used = false;
	parse(exp);
}

filterEx& filterEx::parse(string exp)
{
	// 请注意，当表达式中同时出现not(1)和at(1)时，
	// 那not(1)会被忽略，这个特征由matchTree系列函数中判断顺序决定
	if (exp.find("not(") != string::npos)
	{
		// 排除第n个
		filterType = conEx::FT_NOT_AT;
		filterValue = strToInt(exp.substr(4));
	}
	else if (exp.find("not") != string::npos)
	{
		filterType = conEx::FT_NOT_AT;
		filterValue = strToInt(exp.substr(3));
	}
	else if (exp.find("at(") != string::npos || exp.find("eq(") != string::npos)
	{
		// 保留第n个
		filterType = conEx::FT_AT;
		filterValue = strToInt(exp.substr(3));
	}
	else if (exp.find("at") != string::npos || exp.find("eq") != string::npos)
	{
		filterType = conEx::FT_AT;
		filterValue = strToInt(exp.substr(2));
	}
	else if (exp.find("@") != string::npos)
	{
		filterType = conEx::FT_AT;
		filterValue = strToInt(exp.substr(1));
	}
	else if (exp.find("first") != string::npos)
	{
		// 取第一个
		filterType = conEx::FT_AT;
		filterValue = 1;
	}
	else if (exp.find("last") != string::npos)
	{
		// 取最后一个
		filterType = conEx::FT_LAST;
	}
	else if (exp.find("even") != string::npos)
	{
		filterType = conEx::FT_EVEN;
	}
	else if (exp.find("odd") != string::npos)
	{
		filterType = conEx::FT_ODD;
	}
	else if (exp.find("element") != string::npos)
	{
		filterType = conEx::FT_NODETYPE;
		testNodeType = $::XML_ELEMENT;
	}
	else if (exp.find("comment") != string::npos)
	{
		filterType = conEx::FT_NODETYPE;
		testNodeType = $::XML_COMMENT;
	}
	return *this;
}

///////////////////////////////////////////////////////
// functions
void removeSpaceBeforeGT(string& conexp)
{
	int pos = conexp.find('>');
	int i = 0;
	while (pos != string::npos)
	{
		i = 0;
		while (pos - i - 1 >= 0 && conexp[pos - i - 1] == ' ')
		{
			i++;
		}
		conexp.replace(conexp.begin() + pos - i, conexp.begin() + pos, "");
		pos = conexp.find('>', pos - i + 1);
	}
}

char removeBraketPairFrom(string& conexp, string ls, string rs, string& lRemain,
		string& rRemain)
{
	char ret = '-';
	int lPos = conexp.find(ls);
	int rPos = conexp.find_last_of(rs);
	if (lPos == string::npos || conexp.find(rs) == string::npos)
		return '-';	// 因为find_last_of不测试完全匹配，所以要先用这个判断
	ret = lPos - 1 < 0 ? '-' : conexp[lPos - 1];
	if (rPos - rs.length() >= lPos + ls.length())
	{
		lRemain = conexp.substr(0, lPos);
		rRemain = conexp.substr(rPos + 1);
		conexp = conexp.substr(lPos + ls.length(),
				rPos - rs.length() - lPos - ls.length() + 1);
	}
	else if (rPos == lPos + 1)
	{
		//因为substr不能取出长度为0的字符串，所以当去除了两边的括号后没有字符了那就直接返回空字符串
		lRemain = conexp.substr(0, lPos);
		rRemain = conexp.substr(rPos + 1);
		conexp = "";
	}
	return ret;
}
