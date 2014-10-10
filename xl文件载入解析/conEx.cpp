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
			// father �� son��matchTree
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
			vector<TiXmlNode*> fatherResult = father.matchOnce(root); //��Ϊ���ʽҪ��matchOnce����������Ҳ��matchOnce
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
			VecCombine(ret, result.matchOnce(root)); //����ҲҪ��ӦmatchOnce(���������Ľ�βA>B)
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
		// ���ִ˼Ǻű�ʾ�������������ƥ��ɹ�
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
		pConExGp->_conExRelation = conEx::CONEX_AND; //������������������չ̫�๦��
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
			int pos = 0, vpos = 0;	//pos�������attrName��ĩλ�ã�vpos�������attrValue�Ŀ�ʼλ��
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
		pConExGp->conexGp.push_back(*this);				//���conexGp��ֻ��һ��Ԫ��conEx
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
					|| //��һ��Ҫ����һ��
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

	return false && !exceptThisConEx || true && exceptThisConEx; // ֱ�����һ��������û���жϵģ���ôֻ��false�ˡ�
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
			// ֻҪ��һ��filterû��ʹ�ã��ǾͿ϶���ûfilte��
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
			// ���������������ı��ڵ㣬�����ı��ڵ�����.text()
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
		// �����÷ǵݹ��㷨�����汣��(���Թ����ݹ��㷨����Ч�ʸ�%1)
		for (UINT i = 0; i < filterExGp.size(); i++)
		{
			// ����ǰ��Ҳ��filterȫ����Ч��
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
				// match���ڵ�Ĵ�����
				matchCnt++;
				for (UINT i = 0; i < filterExGp.size(); i++)
				{  //����filter��������filter�Ķ���������
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
						break;  //���break��ʾ���ϵ��filter,��ƥ�䵽һ��֮����Բ����ٲ��Ա������
					}
					else if (filterExGp[i].filterType == conEx::FT_NODETYPE
							&& filterExGp[i].testNodeType == tempNode->Type()
							|| filterExGp[i].filterType == conEx::FT_NOT_AT
									&& filterExGp[i].filterValue != matchCnt)
					{
						//����һ���ж�����Ҫ������match���Ľڵ㶼�ж�һ�Σ������ù���filter���ᱻɾ��
						ret.push_back(tempNode);
						break;
					}
					if (filterExGp[i].filterType == conEx::FT_LAST)
					{
						// ѡȡlast���Ҫ���⴦��
						// �������if��֧���Ѿ���break����ѭ������ƥ�䵽�Ľڵ㲻��Ҫ�������ظ�
						// ���ֻ��ѽ�ж�saveTheLastNode�Ƿ���ڣ�����ret�оͿ�����
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
			// �Ѹýڵ���ӽڵ�ȫ����ջ��˳�������Ӻ���ǰ��ջ
			for (TiXmlNode* child = tempNode->LastChild(); child;
					child = child->PreviousSibling())
			{
				stack.push_back(child);
			}
		}
		for (UINT i = 0; i < filterExGp.size(); i++)
		{
			// ��������filterȫ����Ч��
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
	// ���Ժ��ӽڵ���������Զ�����Ҫ�ݹ�
	vector<TiXmlNode*> ret;
	if (pConExGp->conexGp[0].filterExGp.empty())
	{
		for (TiXmlNode* loopNode = root->FirstChild(); loopNode; loopNode =
				loopNode->NextSibling())
		{
			// ���������������ı��ڵ㣬�����ı��ڵ�����.text()
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
			// ����ǰ��Ҳ��filterȫ����Ч��
			filterExGp[i]._used = false;
		}
		int matchCnt = 0;
		TiXmlNode* saveTheLastNode = NULL;
		for (TiXmlNode* loopNode = root->FirstChild(); loopNode; loopNode =
				loopNode->NextSibling())
		{
			// ���������������ı��ڵ㣬�����ı��ڵ�����.text()
			if (loopNode->Type() != $::XML_TEXT && pConExGp->gpMatch(loopNode))
			{
				matchCnt++;
				for (UINT i = 0; i < filterExGp.size(); i++)
				{						//����filter��������filter�Ķ���������
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
						break;		//���break��ʾ���ϵ��filter,��ƥ�䵽һ��֮����Բ����ٲ��Ա������
					}
					else if (filterExGp[i].filterType == conEx::FT_NODETYPE
							&& filterExGp[i].testNodeType == loopNode->Type()
							|| filterExGp[i].filterType == conEx::FT_NOT_AT
									&& filterExGp[i].filterValue != matchCnt)
					{
						//����һ���ж�����Ҫ������match���Ľڵ㶼�ж�һ�Σ������ù���filter���ᱻɾ��
						ret.push_back(loopNode);
						break;
					}
					if (filterExGp[i].filterType == conEx::FT_LAST)
					{
						// ѡȡlast���Ҫ���⴦��
						// �������if��֧���Ѿ���break����ѭ������ƥ�䵽�Ľڵ㲻��Ҫ�������ظ�
						// ���ֻ��ѽ�ж�saveTheLastNode�Ƿ���ڣ�����ret�оͿ�����
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
			// ��������filterȫ����Ч��
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
		return true;  //���ڿսڵ㵽������false����true������Ӧ����ϸ����
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
	return true && !exceptThisConExGp || false && exceptThisConExGp;// ���������Ϊfalse�����ϵΪdefault
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
	// ��ע�⣬�����ʽ��ͬʱ����not(1)��at(1)ʱ��
	// ��not(1)�ᱻ���ԣ����������matchTreeϵ�к������ж�˳�����
	if (exp.find("not(") != string::npos)
	{
		// �ų���n��
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
		// ������n��
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
		// ȡ��һ��
		filterType = conEx::FT_AT;
		filterValue = 1;
	}
	else if (exp.find("last") != string::npos)
	{
		// ȡ���һ��
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
		return '-';	// ��Ϊfind_last_of��������ȫƥ�䣬����Ҫ��������ж�
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
		//��Ϊsubstr����ȡ������Ϊ0���ַ��������Ե�ȥ�������ߵ����ź�û���ַ����Ǿ�ֱ�ӷ��ؿ��ַ���
		lRemain = conexp.substr(0, lPos);
		rRemain = conexp.substr(rPos + 1);
		conexp = "";
	}
	return ret;
}
