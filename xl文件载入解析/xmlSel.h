//xmlSel.h 

/*
	xQuery++ 1.0 (code by adolli)
	
	xQuery++ is a complete module that can resolve and modify xml 
	document, based on TinyXml and reassociation its functions and methods, 
	without any express or implied warranty. In no event 
	will the authors be held liable for any damages arising from 
	the use of this software.

	Permission is granted to anyone to use this software for any
	purpose, including commercial applications, and to alter it and
	redistribute it freely, subject to the following restrictions:

	1. The origin of this software must not be misrepresented; you must
	not claim that you wrote the original software. If you use this
	software in a product, an acknowledgment in the product documentation
	would be appreciated but is not required.

	2. Altered source versions must be plainly marked as such, and
	must not be misrepresented as being the original software.

	3. This notice may not be removed or altered from any source
	distribution. 

	(thanks to the support of TinyXml and all of my friend.)
 
*/
/*
	To get further help, please content me, e-mail: adol-u@qq.com .
	All notes are included in the help document inside the zip file.
*/
// classes declaired below
// xmlSelector
/*
	˵��������tinyxml������xmlѡ���������Ӧ����������ʹ�ø�ʽ��jQuery����
	ʹ�÷�����
		ע�⣬����$()ѡ������һ��������xml�ļ���·�����ڶ���������ɸѡ��������������������Բο����ڵ㣬����Ϊ�˱�ʾ����ʡ�Ե�һ����
		ѡ������$("div") ��ǩ��ѡ����
				$("#id") idѡ����
				$("/div") ����ѡ������ֻѡ��ֱϵ���ӵ�div��ǩ
				$("[attr=xxx]") ����ѡ���������ڸ�����
				$("[attr!=xxx]") ����ѡ�����������ڸ�����
				$("[attr^=xxx]") ����ѡ��������xxx��ͷ������
				$("[attr$=xxx]") ����ѡ��������xxx��β������
				$("[attr*=xxx]") ����ѡ����������xxx������
				$("div.at(x).first") �±�ѡ�������ڸ��ڵ���ѡ����x��div��ǩ��ͬʱҲѡ����һ��div��ǩ�����ϵ

				$("div,#id,[attr]") ����ѡ�������ȼ���$("div")+$("#id")+$("[attr]") 
				$("div&#id") ����ѡ�������ȼ���$("div")��$("#id")�Ľ���
				$("div span") �㼶ѡ��������xml���ڵ���ѡ������div��ǩ��Ȼ����ѡ����div��ǩΪ���ڵ�ѡ��span��ǩ����󷵻صĽ����span��ǩ�ļ���
				$("div.at(x) span.at(y)") �㼶�±�ѡ���������ڵ�������x��div�µĵ�y��span��ǩ
				$("div.at(x)>span.at(y)") ֱϵ�±�ѡ���������ں��ӽڵ���������x��div��Ȼ���x��div�ĺ��ӽڵ�������y��span

				$("[attr=xx]","#id") ���ѡ��������#id���ڽڵ���ѡ������attr=xx�ı�ǩ���ȼ���$("#id>[attr=xx]")
				$("[attr=xx]",$("#id")) ���ѡ�����ȼ��� $("[attr=xx]","#id")
		
		��������$(".first")	ѡ����һ���ڵ�
				$(".last") ѡ�����һ���ڵ�
				$(".odd") ѡ�������ڵ�
				$(".even") ѡ��ż���ڵ�
				$(".not(condition)") �ų���������condition�Ľڵ�
				$(".at(x)")����$(".eq(x)") ѡ����x���ڵ�
				$("/div.at(3)") ѡ�����ڵ��µ�����ֱϵdiv
				$(".not(.at(x)).not(.at(y))") ���ų���������y���ٰѽ���ų���������x����
				$(".not(.at(x).at(y))")	�ų���������y��������x����

		�������ȼ��� TAG  []  #  /  .  &  SPACE  >  ,
				�ȷָ�ţ��ٷָ���ں�'>',��������Ǳ�ǩ��������
				$(#home>input&[name=user]>input.at(1),#home [type=password]) ��ʾidΪhome�Ľڵ��£�ֱϵ��input��ǩ��ͬʱ����name��ֵΪuser�ĵ�һ��input��ǩ �� idΪhome�Ľڵ��£����У�����type��ֵΪpassword�����б�ǩ
*/


#ifndef _XMLSEL_H_
#define	_XMLSEL_H_

#include "tinyxml\tinyxml.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <queue>
using namespace std;

typedef unsigned int UINT;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// class declairation
class $;
class conEx;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// function
vector<string> split(string source, char splitSymbol); //�ָ��ַ���
string joint(const vector<string>& vec, string withThis); //�����ַ���
vector<TiXmlNode*> traverse(TiXmlNode* rootElement, bool (*testOk)(TiXmlNode* Node, string test), string testValue); //����TiXmlNode�Ľڵ㣬����ĳ���ж�
vector<TiXmlNode*> traverseOnce(TiXmlNode* rootElement, bool (*testOk)(TiXmlNode* Node, string test), string testValue); //����TiXmlNode���ڵ��µ�һ���ӽڵ㣬����ĳ���ж�
vector<TiXmlNode*> traverseAmongNodes(vector<TiXmlNode*> amongThis, bool (*testOk)(TiXmlNode* Node, string test), string testValue); //����amongThis�����нڵ㣬�����������Ľڵ㷵��

template <typename T> 
vector<T> & VecCombine(vector<T> &, const vector<T> &); //�ϲ�����ͬ���͵�vector

string removeSection(const string origin, string condition, string& removed); //ȥ������condition(����)������һ�β����������ݸ���removed,e.g. removeSection("12f(abc)123","2f",str);�����������"12123",str��Ϊ"abc"
void dump_to_stdout(const char* pFilename); //�������xml�ĵ�
int strToInt(string str);	//��stringת����int
double strToDouble(string str);
string intToStr(int num);		//��intǿ��ת����string
string doubleToStr(double num);
string & encode(string& encodeStr);		//��ɸѡ�������ַ������룬��':'�Ȼ���ʵ�����
string & decode(string& decodeStr);		//��ɸѡ�������ַ������룬��ʵ�����ת���ɷ���
string & filterStrProc(string& filterStr);		//����ѡ�����ĵ�ѡ�������ַ�����������ų�ͻ

ostream & operator<<(ostream & out, $& xQuery);		//ios�������


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// interface
/* pre-defined selector */
extern $ xQuery;
extern $ xQ;


// selector and manipulator
class $
{
	// friend classes decl
	friend class $_Excel;
	friend class $_Sheets;
	friend class $_Cells;
	friend class $_Rows;
	friend class $_Columns;
	friend class $_Range;
	friend class conExParser;

private:
	
	$ & subtractBy(const vector<TiXmlNode*> &);	//����㣨Ч�ʲ��ߣ�
	$ & intersectionWith(const vector<TiXmlNode*> &);	//�������㣨Ч�ʲ��ߣ�
	$ & intersectionWith(const $ &);	//�������㣨Ч�ʲ��ߣ�
	$ & pushNode(TiXmlNode* node);						//��$_SelElement����ӽڵ�


	/**
	 * ���Ժ���
	 */
	static bool testNodeType(TiXmlNode* Node, int type);		//���Խڵ������Ƿ����type
	bool hasNode(TiXmlNode* Node);						//�ж�ѡ�������Ƿ�����ýڵ�

	
	/**
	 * ˽�й��캯������Ҫ��TiXml�йصı�¶��ȥ����Ӱ��ʹ��
	 */
	$(string filter, TiXmlNode* root);						//˽�еĹ��캯�����ڸ�����ڲ�
	$(TiXmlDocument* _xmlDoc, string filter, string root);	//˽�еĹ��캯�����ڸ�����ڲ�
	$(vector<TiXmlNode*> vecNode);

	/** 
	 * ����ڵ��ͨ�ú�������ֹ������������͵�ģ�����͵��³�����Ϊ˽��
	 */
	template<typename T>											//˽��append����
	$ & _append(T node_type,								//Ҫ����Ľڵ�
			int method									//���뷽��
			);

protected:
	
	/**
	 * ������Ա��filename����Ҫ��xmlDoc->ValueStr()���Է����ļ���
	 */
	vector<TiXmlNode*> $_SelElement;							//��ɸѡ�����Ķ���������
	TiXmlDocument * xmlDoc;											//xml�ĵ�ָ��
	static TiXmlDocument * recentXmlDoc;	//��һ�ζ�ȡ��xmlDoc�������������$(selector)���÷�
	bool isPreDefined;									//�ж��Ƿ���Ԥ�������ı�־

public:

	
	enum NodeType									// xmlNodeType
	{
		XML_DOCUMENT,
		XML_ELEMENT,
		XML_COMMENT,
		XML_UNKNOWN,
		XML_TEXT,
		XML_DECLARATION,
		NT_AllType
	};

	enum appendMethod
	{									// append method
		APDM_SELFCLOSE = 0x01,										//�����Զ��պϱ�ǩ
		APDM_DOUBLETAG = 0x02,										//������Զ��պϱ�ǩ
		APDM_ALL = 0x04,									//��ѡ�������нڵ�ִ��append�ڵ�
		APOS_AFTER = 0x08,		//����λ���ڽڵ��										
		APOS_INSIDE = 0x10,			//����λ���ڽڵ���									
		APOS_BEFORE = 0x20											//����λ���ڽڵ�ǰ
	};
	
	enum copyMethod
	{
		CM_With_singleNODE, /*replace with single node*/
		CM_With_whole_TREE, /*replace with whole tree*/
		CM_DeepCopy, /*the same as RM_With_singleNODE*/
		CM_ShallowCopy /*the same as RM_With_whole_TREE*/
	};

	enum printMethod
	{											//��ӡ����Ļ��ģʽ
		PM_TEXT = 0x1,											//�����ļ�������ı���ӡ
		PM_TREE = 0x2												//������״�ṹ��ӡ
	};

	enum errorType
	{												
		//���ܴ�������ͣ����ڷ��ش���
		ET_ATTR_NOT_FOUND = 0x99999998, 
		ET_NODE_NOT_EXIST = 0x99999999
	};

	enum queryAttr
	{
		// ��ѯ���Ե�ʱ����������ȷ�ķ���ֵ
		QA_SUCCESS = 0x00000000,		//�ɹ���attr�������������ֵ��ֱ�ӷ��ض�Ӧ����ֵ�����ֵֻ�ڳ����ڲ����֣�
		QA_NO_ATTRIBUTE = 0x99999998,									//�Ҳ�������
		QA_WRONG_TYPE													//���ʹ���
	};

	enum OS_Validation
	{										// offspringValidation �����Ƿ�����ѡ�����еĺ��
		OS_ALL,														//ȫ��ѡ�����еĺ��
		OS_PART,														//һ������~~
		OS_NONE														//û��һ����~~
	};

	
	static string notFound;									//���ڷ����Ҳ�����Ҫ��ֵ���߽ڵ�
	static map<string, TiXmlDocument*> fileMap;	//�ļ������ĵ��ڵ�Ķ�Ӧ��ϵ����ͬ�ļ���ͬһ������ֻ��ȡһ����xmlDoc��ͬ

	$(TiXmlNode* node);								//ֱ�ӽ��ڵ��װ����
	$(const char * xmlFileName);
	$(string filter, $ root);
	$(const char * xmlFileName, string filter, string rootSel);
	$();											//Ĭ�Ϲ��캯���ĳ�ԱxmlDocΪNULL
	$(bool _isPreDefined);							//���ڹ���Ԥ��������
	~$();
	$ & loadXml(const char * xmlFileName);				//��ʵ����xml�ĵ�
	bool save(string filename = "");						//�����ļ�
	$ & setPreDefinedObj();								//�����ΪԤ�������

	/**
	 * ѡ����
	 */
	$ & select(string filter, TiXmlNode* root = NULL);//ɸѡ�����ʵ����$_SelElement��Ա��(base)
	$ & select(string filter, string rootStr);		//ɸѡ�����ʵ����$_SelElement��Ա��
	$ & select(string filter, $& root);				//ɸѡ�����ʵ����$_SelElement��Ա��
	$ & selectAllNode(TiXmlNode* root);			//ѡ������xml�ĵ��еĽڵ㱣�浽$_selElement��
	$ & selectAllNode($& root);
	$ & operator()(string selector, string root = "");//��select������ͬ���ǽ�selector��ԭ�еĽڵ��������ѡ��
	$ & operator()(string selector, $& root);

	/**
	 * ���������ų���
	 */
	$ & filter(string condition);						//��������������ɸѡ���
	$ & filter(NodeType type);							//���˳�����ڵ����������Ľڵ�
	$ & except(string condition);						//�ų�����������ɸѡ���(��filter�෴)

	/**
	 * xml�ڵ���������
	 */
	$ & next(string condition = "");			//����ѡ����ÿһ���ڵ����һ���ֵܽڵ�,���Դ���ѡ������
	$ & previous(string condition = "");		//����ѡ����ÿһ���ڵ����һ���ֵܽڵ�,���Դ���ѡ������
	$ & nextAll(string condition = "");			//����ѡ����ÿһ���ڵ���·������ֵܽڵ�,���Դ���ѡ������
	$ & previousAll(string condition = "");		//����ѡ����ÿһ���ڵ���Ϸ������ֵܽڵ�,���Դ���ѡ������

	$ & parent();										//����ѡ����ÿһ���ڵ�ĸ��ڵ�
	$ & ancestors();									//����ѡ������ÿһ���ڵ���������Ƚڵ�
	$ & children(string selector = "*");//����ѡ����ÿһ���ڵ����������ĺ��ӽڵ㣨ֱϵ��,�������򷵻�����ֱϵ����
	$ & offspring(string selector = "*");
	bool noChildren();									//����ѡ������һ���ڵ��Ƿ��к��ӽڵ�

	/**
	 * xml����
	 */
	string attr(string attrname);				//������ѡ�ڵ�ĵ�һ����attrname����ֵ,�Ҳ�������""
	int iAttr(string attrname);							//����int������ֵ���������
	double dAttr(string attrname);//����double������ֵ��������ڣ�int��double�����ڷ���$::QA_NO_ATTRIBUTE
	bool bAttr(string attrname);			//��ѡ�����ջ����޸����Ի���ֵΪ0ʱ����false�����඼����true
	vector<string> getAllAttr(string attrname);	//����selector�����нڵ��attrname���ԣ��±��Ӧ
	$ & attr(string attrname, string value);				//�������нڵ�����
	$ & attr(string attrname, int value);				//�������нڵ����� (���ز�ͬ����ֵ����)
	$ & attr(string attrname, double value);			//�������нڵ����� (���ز�ͬ����ֵ����)

	//DOM append child
	$ & appendDeclaration(								//�ĵ�����
			string ver = "1.0",							//version
			string encoding = "utf-8",					//encoding
			string standalone = "yes",					//standAlone
			int method = $::APOS_BEFORE);
	$ & appendElement(
			//Ԫ�ؽڵ�
			string tagName = "",
			int method = $::APDM_ALL | $::APOS_INSIDE | $::APOS_AFTER
					| $::APDM_DOUBLETAG);
	$ & appendText(
			//�ı��ڵ�
			string text = "",
			int method = $::APDM_ALL | $::APOS_INSIDE | $::APOS_AFTER);
	$ & appendComment(									//ע�ͽڵ�
			string comment = "", int method = $::APOS_BEFORE			//Ĭ����ӷ�ʽ
			);

	//Selector remove child
	$ & removeFromSelector(string selector = "*");		//��ѡ�������Ƴ��ڵ㣬Ĭ���Ƴ�����
	$ & removeFromSelector(int index);					//��ѡ�������Ƴ�index�±�Ľڵ�

	//DOM remove/replace child
	$ & remove(string selector = "");//��xml���Ƴ��ڵ㣬Ĭ���Ƴ�ѡ�����еĽڵ㣬�����Ƴ�selectorѡ����Ľڵ�
	$ & removeText(string selector = "");//��xml���Ƴ��ı��ڵ㣬Ĭ���Ƴ�ѡ�����еĽڵ���ı��ڵ㼰�亢���е��ı��ڵ�
	$ & replaceWith(string withThis, int copyMethod = $::CM_ShallowCopy);//��withThis��Ϊѡ��������ѡ���Ľڵ��滻��ǰѡ�����еĽڵ㣬����һ���ࡣ
	$ & replaceWith($& withThis,				//��withThis�еĽڵ��滻selector��DOM�ڵ�
			int copyMethod = $::CM_ShallowCopy);	//��withThis����selector�еĽڵ�
	$ & replaceWithNewElement(string tagName);			//�ڵ㻻����Ԫ�أ����ذ�����Ԫ�ص�ѡ����
	$ & clone($& pasteTo, int position = $::APOS_AFTER, int copyMethod =
			$::CM_ShallowCopy);		//���ڵ㸴�Ƶ�pasteTo֮�����֮ǰ��λ��,ע�⣡���ص�ѡ�����е����²���Ľڵ�
	$ & cloneTo($& pasteTo, int position = $::APOS_AFTER, int copyMethod =
			$::CM_ShallowCopy);						//��clone��ͬ������ѡ�����е���Ȼ��ԭ���Ľڵ�
	$ & wrapBy($& byThis);			//��ѡ�����еĽڵ�����һ���ڵ�Ŀ���������������,������ʹ�������²��ϵ�Ľڵ��ѡ����
	$ & wrapByElement(string tagName);					//��ĳ��Ԫ�ذ���

	/**
	 * ѡ��������
	 */
	$ & clear();										//���ɸѡ���е�����
	$ & clearSame();									//���$_SelElement����ͬ������
	$ & popEmpty();										//���$_SelElement��Ϊ�յĽڵ�
	vector<TiXmlNode*> getSelection(int index = -1);//ѡ����index��ɸѡ���Ľڵ�(0��ʼ�ƣ�С��0�򷵻ص�һ��)
	TiXmlNode * lastSelection();
	TiXmlNode * getNode(int index = -1);	//ѡ����index��ɸѡ���Ľڵ�(0��ʼ�ƣ�С��0�򷵻ص�һ��)
	TiXmlNode * operator[](int index);
	unsigned int length() const;								//����ɸѡ���Ľڵ�ĸ���
	string text();//����$_SelElement[0]��text���ӵ�value()�����������text�ڵ���ֱ�ӷ��ر����value()
	string tagName(int index = 0);						//����Ԫ�ؽڵ�ı�ǩ����ѡ�����еĵ�һ��
	int nodeType();										//���ؽڵ�����

	bool notBindXml();									//�ж��Ƿ����xml
	bool isEmpty();										//�����Ƿ�Ϊ��
	OS_Validation hasOffSpring(const string& selector);	//�ж�$_SelElement[0]�Ƿ���selectorѡ�еĺ��
	bool has(NodeType type);							//����ѡ�������Ƿ���type���͵Ľڵ�
	$ & getNodeWithType(NodeType type);					//����ѡ������type���͵Ľڵ��ѡ����
	$ & displaySelTagName();							//��ʾ������ɸѡ������ڵ㣩��value()
	$ & print(printMethod method = $::PM_TREE);			//��ӡxml�ĵ�
	string fileName();
	$ & operator+=(const $& addThis);	//�ϲ�ѡ�����Ľڵ㣬��ע�Ᵽ֤����$��xmlDoc�Ƿ���ͬ������ͬ�Ǻϲ����˵�
	$ & operator-(const $& substractThis);				//���Ч���൱��except
	friend ostream & operator<<(ostream & out, $& xQuery);			//��״��ӡ�ĵ�

	/**
	 * ѡ������������
	 */
	template<typename Ret_type, typename Para_type>
	Ret_type each(Para_type (*function)());					//��������ɸѡ����ĺ���

	/**************************************************************************************************************/
};

//class head
class filterEx;

//functions
void removeSpaceBeforeGT(string& conexp);					//ɾ��conEx��>�ַ�ǰ���пո�
char removeBraketPairFrom(string& conexp,			//��ƥ������м�ı��ʽ����ȥ�����ź��м������
		string ls,				//������
		string rs,				//������
		string& lRemain,		//������ߵ��ַ���
		string& rRemain			//�����ұߵ��ַ���
		);//��conexp���Ƴ������ƥ������ţ�������Ϊls��������Ϊrs

// conEx parser
class conExParser
{

public:
	string originExp;
	vector<string> vecExp;
	conExParser(string exp = "");
	conExParser & parse(string exp);
	vector<TiXmlNode*> match(TiXmlNode* root);
	vector<TiXmlNode*> matchOnce(TiXmlNode* root);

};

// conditionExpression conEx
class conEx
{
	friend class conExGp;
	friend class filterExGp;
	friend class conExParser;

protected:
	string testTagName;									//��ǩƥ����
	string testAttrName;									//����ƥ����
	string testAttrValue;									//����ƥ��ֵ
	string testAttrMatchType;								//����ƥ�䷽ʽ
	conExGp * pConExGp;										//���ʽȺ��
	bool exceptThisConEx;								//�Ƿ�ƥ����ȡ��
	vector<filterEx> filterExGp;								//������Ⱥ�飨ȫ�����ϵ��

public:
	enum Relation
	{
		CONEX_DEFAULT,			//ֻ����һ��Ԫ�ص��ж�
		CONEX_AND,				//������
		CONEX_OR,				//������
		CONEX_NOT				//������
	};
	
	enum FilterType
	{
		FT_ALL,					//ƥ��ȫ��
		FT_FIRST,				//ƥ�䵽��һ����ֹͣ
		FT_LAST,				//ȡƥ���������һ��
		FT_AT,					//ȡ��n��ƥ����
		FT_NOT_AT,				//��ȡƥ�䵽�ĵ�n��
		FT_ODD,					//ȡ��2n+1��ƥ����
		FT_EVEN,				//ȡ��2n��ƥ����
		FT_NODETYPE				//ȡ�ڵ�Ϊtype��ƥ����
	};
	
	static string blank;											//��ʾ�ձ��ʽ�ĳ���
	conExGp & conExSplit(string exp, char spSymbol);	//�ֽ�󷵻�����conExGp(Ⱥ��)������

	conEx(string exp = "*", bool except = false);
	conEx & parse(string exp);								//�ѱ��ʽ����Ϣ���������ݳ�Ա��
	bool match(TiXmlNode* node);							//�����Ƿ�ƥ��ýڵ�
	vector<TiXmlNode*> matchTree(TiXmlNode* root);					//��������������Ľڵ�
	vector<TiXmlNode*> matchOnce(TiXmlNode* root);					//��������������Ľڵ�
	//vector<TiXmlNode*>	matchVector(TiXmlNode* root,int& matchCount);	//����vector�еĽڵ㷵�������

	bool filterDone();								//��filterTypeȫΪFT_ALLʱ����true

};

class conExGp
{
	
	friend class conEx;
	friend class conExParser;

protected:
	vector<conEx> conexGp;										//�������ʽȺ��
	conEx::Relation _conExRelation;
	bool exceptThisConExGp;								//������жϽ���Ƿ�ȡ��

public:
	conExGp();
	bool gpMatch(TiXmlNode* node);						//����ƥ��ڵ�

};

class filterEx
{
	
	friend class conEx;
	friend class conExParser;

protected:
	int filterType;										//filter����
	int testNodeType;
	int filterValue;									//ͨ����ʾat���ĸ�λ�õ�ֵ
	bool _used;											//���ù���һ�κ����Ϊtrue

public:
	filterEx(string exp = "");
	filterEx & parse(string exp);

};


#endif		/*_XMLSEL_H_*/

/*
	����conEx��һЩ���ʽ˵������д������
	conEx ex("Worksheet&[ss:Name$=4] >[ss:Width=2.25]");
	ע�⵽�Ǹ�>ǰ�Ŀո񣬸ñ��ʽ����Ϊ��
		��"[ss:Width=2.25]"�ĸ��ڵ�F��"Worksheet&[ss:Name$=4] "ʱƥ��Ϊtrue��
			F����Ϊ"Worksheet&[ss:Name$=4] "
				��"Worksheet&[ss:Name$=4]"Ϊ""�����ַ�����ע��ĩβ�Ǹ��ո����Ƚڵ�ʱF��ƥ��Ϊtrue
					���ݶ��壬""��ƥ������ԶΪtrue
			����Fƥ��Ϊtrue
		����"[ss:Width=2.25]"�ĸ��ڵ�F��ƥ��ɹ�������xml�����ǲ��Ǹ��ӹ�ϵ��
		Ϊ���������ִ���>ǰ�����пո�Ҫɾ��
			ɾ��>ǰ�Ŀո�ʹ�ú���removeSpaceBeforeGT(string& exp);


	conEx �﷨, new features
	Father>Child
	GrandFather>>Child
	GrandGrandFather>>>Child
	GrandFather    Child (no matter how many space you type)
		(this's also valid in GrandFather > > Child
	�����ĸ�֮ǰ��$()���
*/
