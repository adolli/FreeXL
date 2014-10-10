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
	说明：基于tinyxml开发的xml选择器及其对应操作函数，使用格式与jQuery相似
	使用方法：
		注意，所有$()选择器第一个参数是xml文件名路径，第二个参数是筛选条件，第三个参数是相对参考父节点，以下为了表示清晰省略第一参数
		选择器：$("div") 标签名选择器
				$("#id") id选择器
				$("/div") 孩子选择器，只选出直系孩子的div标签
				$("[attr=xxx]") 属性选择器，等于该属性
				$("[attr!=xxx]") 属性选择器，不等于该属性
				$("[attr^=xxx]") 属性选择器，以xxx开头的属性
				$("[attr$=xxx]") 属性选择器，以xxx结尾的属性
				$("[attr*=xxx]") 属性选择器，包含xxx的属性
				$("div.at(x).first") 下标选择器，在根节点内选出第x个div标签，同时也选出第一个div标签，或关系

				$("div,#id,[attr]") 多项选择器，等价于$("div")+$("#id")+$("[attr]") 
				$("div&#id") 与项选择器，等价于$("div")与$("#id")的交集
				$("div span") 层级选择器，在xml根节点下选出所有div标签，然后以选出的div标签为根节点选出span标签，最后返回的结果是span标签的集合
				$("div.at(x) span.at(y)") 层级下标选择器，根节点搜索第x个div下的第y个span标签
				$("div.at(x)>span.at(y)") 直系下标选择器，仅在孩子节点下搜索第x个div，然后第x个div的孩子节点搜索第y个span

				$("[attr=xx]","#id") 相对选择器，在#id所在节点下选出属性attr=xx的标签，等价于$("#id>[attr=xx]")
				$("[attr=xx]",$("#id")) 相对选择器等价于 $("[attr=xx]","#id")
		
		过滤器：$(".first")	选出第一个节点
				$(".last") 选出最后一个节点
				$(".odd") 选出奇数节点
				$(".even") 选出偶数节点
				$(".not(condition)") 排除满足条件condition的节点
				$(".at(x)")或者$(".eq(x)") 选出第x个节点
				$("/div.at(3)") 选出根节点下第三个直系div
				$(".not(.at(x)).not(.at(y))") 先排除符合条件y后再把结果排除符合条件x的项
				$(".not(.at(x).at(y))")	排除符合条件y或者条件x的项

		运算优先级： TAG  []  #  /  .  &  SPACE  >  ,
				先分割逗号，再分割大于号'>',……最后是标签名：例如
				$(#home>input&[name=user]>input.at(1),#home [type=password]) 表示id为home的节点下（直系）input标签且同时属性name的值为user的第一个input标签 与 id为home的节点下（所有）属性type的值为password的所有标签
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
vector<string> split(string source, char splitSymbol); //分割字符串
string joint(const vector<string>& vec, string withThis); //串联字符串
vector<TiXmlNode*> traverse(TiXmlNode* rootElement, bool (*testOk)(TiXmlNode* Node, string test), string testValue); //遍历TiXmlNode的节点，并作某个判断
vector<TiXmlNode*> traverseOnce(TiXmlNode* rootElement, bool (*testOk)(TiXmlNode* Node, string test), string testValue); //遍历TiXmlNode跟节点下的一层子节点，并作某个判断
vector<TiXmlNode*> traverseAmongNodes(vector<TiXmlNode*> amongThis, bool (*testOk)(TiXmlNode* Node, string test), string testValue); //遍历amongThis中所有节点，把满足条件的节点返回

template <typename T> 
vector<T> & VecCombine(vector<T> &, const vector<T> &); //合并两个同类型的vector

string removeSection(const string origin, string condition, string& removed); //去除包含condition(……)这样的一段并将括号内容赋给removed,e.g. removeSection("12f(abc)123","2f",str);这个函数返回"12123",str变为"abc"
void dump_to_stdout(const char* pFilename); //树形输出xml文档
int strToInt(string str);	//将string转换成int
double strToDouble(string str);
string intToStr(int num);		//将int强制转换成string
string doubleToStr(double num);
string & encode(string& encodeStr);		//给筛选条件的字符串编码，把':'等换成实体符号
string & decode(string& decodeStr);		//给筛选条件的字符串解码，把实体符号转换成符号
string & filterStrProc(string& filterStr);		//处理选择器的的选择条件字符串，避免符号冲突

ostream & operator<<(ostream & out, $& xQuery);		//ios输出函数


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
	
	$ & subtractBy(const vector<TiXmlNode*> &);	//差集运算（效率不高）
	$ & intersectionWith(const vector<TiXmlNode*> &);	//交集运算（效率不高）
	$ & intersectionWith(const $ &);	//交集运算（效率不高）
	$ & pushNode(TiXmlNode* node);						//在$_SelElement后添加节点


	/**
	 * 测试函数
	 */
	static bool testNodeType(TiXmlNode* Node, int type);		//测试节点类型是否等于type
	bool hasNode(TiXmlNode* Node);						//判断选择器中是否包含该节点

	
	/**
	 * 私有构造函数，不要把TiXml有关的暴露出去以免影响使用
	 */
	$(string filter, TiXmlNode* root);						//私有的构造函数用于该类的内部
	$(TiXmlDocument* _xmlDoc, string filter, string root);	//私有的构造函数用于该类的内部
	$(vector<TiXmlNode*> vecNode);

	/** 
	 * 插入节点的通用函数，防止带入非正常类型的模版类型导致出错，设为私有
	 */
	template<typename T>											//私有append函数
	$ & _append(T node_type,								//要插入的节点
			int method									//插入方法
			);

protected:
	
	/**
	 * 保护成员，filename不需要，xmlDoc->ValueStr()可以返回文件名
	 */
	vector<TiXmlNode*> $_SelElement;							//被筛选出来的都放在里面
	TiXmlDocument * xmlDoc;											//xml文档指针
	static TiXmlDocument * recentXmlDoc;	//上一次读取的xmlDoc保存在这里，方便$(selector)的用法
	bool isPreDefined;									//判断是否是预定义对象的标志

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
		APDM_SELFCLOSE = 0x01,										//插入自动闭合标签
		APDM_DOUBLETAG = 0x02,										//插入非自动闭合标签
		APDM_ALL = 0x04,									//对选择器所有节点执行append节点
		APOS_AFTER = 0x08,		//插入位置在节点后										
		APOS_INSIDE = 0x10,			//插入位置在节点内									
		APOS_BEFORE = 0x20											//插入位置在节点前
	};
	
	enum copyMethod
	{
		CM_With_singleNODE, /*replace with single node*/
		CM_With_whole_TREE, /*replace with whole tree*/
		CM_DeepCopy, /*the same as RM_With_singleNODE*/
		CM_ShallowCopy /*the same as RM_With_whole_TREE*/
	};

	enum printMethod
	{											//打印到屏幕的模式
		PM_TEXT = 0x1,											//按照文件本身的文本打印
		PM_TREE = 0x2												//按照树状结构打印
	};

	enum errorType
	{												
		//可能错误的类型，用于返回错误
		ET_ATTR_NOT_FOUND = 0x99999998, 
		ET_NODE_NOT_EXIST = 0x99999999
	};

	enum queryAttr
	{
		// 查询属性的时候错误或者正确的返回值
		QA_SUCCESS = 0x00000000,		//成功（attr函数不返回这个值，直接返回对应属性值，这个值只在程序内部出现）
		QA_NO_ATTRIBUTE = 0x99999998,									//找不到属性
		QA_WRONG_TYPE													//类型错误
	};

	enum OS_Validation
	{										// offspringValidation 返回是否属于选择器中的后代
		OS_ALL,														//全是选择器中的后代
		OS_PART,														//一部分是~~
		OS_NONE														//没有一个是~~
	};

	
	static string notFound;									//用于返回找不到所要的值或者节点
	static map<string, TiXmlDocument*> fileMap;	//文件名与文档节点的对应关系，相同文件在同一程序中只读取一次且xmlDoc相同

	$(TiXmlNode* node);								//直接将节点包装起来
	$(const char * xmlFileName);
	$(string filter, $ root);
	$(const char * xmlFileName, string filter, string rootSel);
	$();											//默认构造函数的成员xmlDoc为NULL
	$(bool _isPreDefined);							//用于构造预定义类型
	~$();
	$ & loadXml(const char * xmlFileName);				//将实例绑定xml文档
	bool save(string filename = "");						//保存文件
	$ & setPreDefinedObj();								//设对象为预定义对象

	/**
	 * 选择器
	 */
	$ & select(string filter, TiXmlNode* root = NULL);//筛选后放入实例的$_SelElement成员内(base)
	$ & select(string filter, string rootStr);		//筛选后放入实例的$_SelElement成员内
	$ & select(string filter, $& root);				//筛选后放入实例的$_SelElement成员内
	$ & selectAllNode(TiXmlNode* root);			//选择所有xml文档中的节点保存到$_selElement中
	$ & selectAllNode($& root);
	$ & operator()(string selector, string root = "");//与select功能相同但是将selector中原有的节点清除后再选择
	$ & operator()(string selector, $& root);

	/**
	 * 过滤器与排除器
	 */
	$ & filter(string condition);						//过滤满足条件的筛选结果
	$ & filter(NodeType type);							//过滤出满足节点类型条件的节点
	$ & except(string condition);						//排除满足条件的筛选结果(与filter相反)

	/**
	 * xml节点访问与遍历
	 */
	$ & next(string condition = "");			//返回选择器每一个节点的下一个兄弟节点,可以带有选择条件
	$ & previous(string condition = "");		//返回选择器每一个节点的上一个兄弟节点,可以带有选择条件
	$ & nextAll(string condition = "");			//返回选择器每一个节点的下方所有兄弟节点,可以带有选择条件
	$ & previousAll(string condition = "");		//返回选择器每一个节点的上方所有兄弟节点,可以带有选择条件

	$ & parent();										//返回选择器每一个节点的父节点
	$ & ancestors();									//返回选择器中每一个节点的所有祖先节点
	$ & children(string selector = "*");//返回选择器每一个节点满足条件的孩子节点（直系）,参数空则返回所有直系孩子
	$ & offspring(string selector = "*");
	bool noChildren();									//返回选择器第一个节点是否有孩子节点

	/**
	 * xml操作
	 */
	string attr(string attrname);				//返回所选节点的第一个的attrname属性值,找不到返回""
	int iAttr(string attrname);							//返回int的属性值，如果存在
	double dAttr(string attrname);//返回double的属性值，如果存在，int或double不存在返回$::QA_NO_ATTRIBUTE
	bool bAttr(string attrname);			//当选择器空或者无该属性或者值为0时返回false，其余都返回true
	vector<string> getAllAttr(string attrname);	//返回selector中所有节点的attrname属性，下标对应
	$ & attr(string attrname, string value);				//设置所有节点属性
	$ & attr(string attrname, int value);				//设置所有节点属性 (重载不同属性值类型)
	$ & attr(string attrname, double value);			//设置所有节点属性 (重载不同属性值类型)

	//DOM append child
	$ & appendDeclaration(								//文档申明
			string ver = "1.0",							//version
			string encoding = "utf-8",					//encoding
			string standalone = "yes",					//standAlone
			int method = $::APOS_BEFORE);
	$ & appendElement(
			//元素节点
			string tagName = "",
			int method = $::APDM_ALL | $::APOS_INSIDE | $::APOS_AFTER
					| $::APDM_DOUBLETAG);
	$ & appendText(
			//文本节点
			string text = "",
			int method = $::APDM_ALL | $::APOS_INSIDE | $::APOS_AFTER);
	$ & appendComment(									//注释节点
			string comment = "", int method = $::APOS_BEFORE			//默认添加方式
			);

	//Selector remove child
	$ & removeFromSelector(string selector = "*");		//从选择器中移除节点，默认移除所有
	$ & removeFromSelector(int index);					//从选择器中移除index下标的节点

	//DOM remove/replace child
	$ & remove(string selector = "");//从xml中移除节点，默认移除选择器中的节点，否则移除selector选择出的节点
	$ & removeText(string selector = "");//从xml中移除文本节点，默认移除选择器中的节点的文本节点及其孩子中的文本节点
	$ & replaceWith(string withThis, int copyMethod = $::CM_ShallowCopy);//用withThis作为选择器条件选出的节点替换当前选择器中的节点，可以一换多。
	$ & replaceWith($& withThis,				//用withThis中的节点替换selector中DOM节点
			int copyMethod = $::CM_ShallowCopy);	//将withThis代入selector中的节点
	$ & replaceWithNewElement(string tagName);			//节点换成新元素，返回包含该元素的选择器
	$ & clone($& pasteTo, int position = $::APOS_AFTER, int copyMethod =
			$::CM_ShallowCopy);		//将节点复制到pasteTo之后或者之前的位置,注意！返回到选择器中的是新插入的节点
	$ & cloneTo($& pasteTo, int position = $::APOS_AFTER, int copyMethod =
			$::CM_ShallowCopy);						//与clone相同，但是选择器中的仍然是原来的节点
	$ & wrapBy($& byThis);			//把选择器中的节点用另一个节点的拷贝来包裹在外面,不建议使用有上下层关系的节点的选择器
	$ & wrapByElement(string tagName);					//用某新元素包裹

	/**
	 * 选择器操作
	 */
	$ & clear();										//清除筛选器中的内容
	$ & clearSame();									//清除$_SelElement中相同的内容
	$ & popEmpty();										//清楚$_SelElement中为空的节点
	vector<TiXmlNode*> getSelection(int index = -1);//选出第index个筛选出的节点(0开始计，小于0则返回第一个)
	TiXmlNode * lastSelection();
	TiXmlNode * getNode(int index = -1);	//选出第index个筛选出的节点(0开始计，小于0则返回第一个)
	TiXmlNode * operator[](int index);
	unsigned int length() const;								//返回筛选出的节点的个数
	string text();//返回$_SelElement[0]的text孩子的value()，如果本身是text节点则直接返回本身的value()
	string tagName(int index = 0);						//返回元素节点的标签名，选择器中的第一个
	int nodeType();										//返回节点类型

	bool notBindXml();									//判断是否绑定了xml
	bool isEmpty();										//返回是否为空
	OS_Validation hasOffSpring(const string& selector);	//判断$_SelElement[0]是否有selector选中的后代
	bool has(NodeType type);							//返回选择器中是否含有type类型的节点
	$ & getNodeWithType(NodeType type);					//返回选择器中type类型的节点的选择器
	$ & displaySelTagName();							//显示出所有筛选结果（节点）的value()
	$ & print(printMethod method = $::PM_TREE);			//打印xml文档
	string fileName();
	$ & operator+=(const $& addThis);	//合并选出来的节点，请注意保证两个$的xmlDoc是否相同，不相同是合并不了的
	$ & operator-(const $& substractThis);				//差集，效果相当于except
	friend ostream & operator<<(ostream & out, $& xQuery);			//树状打印文档

	/**
	 * 选择器遍历函数
	 */
	template<typename Ret_type, typename Para_type>
	Ret_type each(Para_type (*function)());					//遍历所有筛选结果的函数

	/**************************************************************************************************************/
};

//class head
class filterEx;

//functions
void removeSpaceBeforeGT(string& conexp);					//删除conEx的>字符前所有空格
char removeBraketPairFrom(string& conexp,			//在匹配符号中间的表达式，即去除括号后中间的内容
		string ls,				//左括号
		string rs,				//右括号
		string& lRemain,		//括号左边的字符串
		string& rRemain			//括号右边的字符串
		);//在conexp中移除最外层匹配的括号，左括号为ls，右括号为rs

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
	string testTagName;									//标签匹配名
	string testAttrName;									//属性匹配名
	string testAttrValue;									//属性匹配值
	string testAttrMatchType;								//属性匹配方式
	conExGp * pConExGp;										//表达式群组
	bool exceptThisConEx;								//是否匹配结果取反
	vector<filterEx> filterExGp;								//过滤器群组（全都或关系）

public:
	enum Relation
	{
		CONEX_DEFAULT,			//只做第一个元素的判断
		CONEX_AND,				//和条件
		CONEX_OR,				//或条件
		CONEX_NOT				//非条件
	};
	
	enum FilterType
	{
		FT_ALL,					//匹配全部
		FT_FIRST,				//匹配到第一个就停止
		FT_LAST,				//取匹配结果的最后一个
		FT_AT,					//取第n个匹配结果
		FT_NOT_AT,				//不取匹配到的第n个
		FT_ODD,					//取第2n+1个匹配结果
		FT_EVEN,				//取第2n个匹配结果
		FT_NODETYPE				//取节点为type的匹配结果
	};
	
	static string blank;											//表示空表达式的常量
	conExGp & conExSplit(string exp, char spSymbol);	//分解后返回自身conExGp(群组)的引用

	conEx(string exp = "*", bool except = false);
	conEx & parse(string exp);								//把表达式的信息解析到数据成员中
	bool match(TiXmlNode* node);							//测试是否匹配该节点
	vector<TiXmlNode*> matchTree(TiXmlNode* root);					//遍历树返回满足的节点
	vector<TiXmlNode*> matchOnce(TiXmlNode* root);					//遍历树返回满足的节点
	//vector<TiXmlNode*>	matchVector(TiXmlNode* root,int& matchCount);	//遍历vector中的节点返回满足的

	bool filterDone();								//当filterType全为FT_ALL时返回true

};

class conExGp
{
	
	friend class conEx;
	friend class conExParser;

protected:
	vector<conEx> conexGp;										//条件表达式群组
	conEx::Relation _conExRelation;
	bool exceptThisConExGp;								//该组的判断结果是否取反

public:
	conExGp();
	bool gpMatch(TiXmlNode* node);						//总体匹配节点

};

class filterEx
{
	
	friend class conEx;
	friend class conExParser;

protected:
	int filterType;										//filter类型
	int testNodeType;
	int filterValue;									//通常表示at到哪个位置的值
	bool _used;											//被用过了一次后会标记为true

public:
	filterEx(string exp = "");
	filterEx & parse(string exp);

};


#endif		/*_XMLSEL_H_*/

/*
	关于conEx的一些表达式说明文字写在这里
	conEx ex("Worksheet&[ss:Name$=4] >[ss:Width=2.25]");
	注意到那个>前的空格，该表达式解析为，
		当"[ss:Width=2.25]"的父节点F是"Worksheet&[ss:Name$=4] "时匹配为true，
			F解析为"Worksheet&[ss:Name$=4] "
				当"Worksheet&[ss:Name$=4]"为""（空字符串，注意末尾那个空格）祖先节点时F的匹配为true
					根据定义，""在匹配中永远为true
			所以F匹配为true
		所以"[ss:Width=2.25]"的父节点F被匹配成功，但在xml中他们不是父子关系，
		为了消除这种错误，>前的所有空格要删除
			删除>前的空格使用函数removeSpaceBeforeGT(string& exp);


	conEx 语法, new features
	Father>Child
	GrandFather>>Child
	GrandGrandFather>>>Child
	GrandFather    Child (no matter how many space you type)
		(this's also valid in GrandFather > > Child
	其他的跟之前的$()差不多
*/
