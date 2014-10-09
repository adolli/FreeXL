//xmlSel.cpp

#include "tinyxml\tinyxml.h"
#include "xmlSel.h"
#include <string>
#include <sstream>
using namespace std;

// $ ////////////////////////////////////////////////////////////////////
//
// global settings

/* pre-defined selector */
$ xQuery_noConflict(true);
$ xQ(true);
$::$(bool _isPreDefined):isPreDefined(_isPreDefined){}

// special functions
template<typename X,typename Y> inline bool alwaysRtTrue(X x,Y y) { return true; }			//������Զ����true�ĺ���

// constant and static
string $::notFound="__000000_NOT_FOUND_ATTRIBUTE_OR_NODE__";
map<string,TiXmlDocument*> $::fileMap;
TiXmlDocument* $::recentXmlDoc=NULL;

// constructor
$::$(TiXmlNode* node):isPreDefined(false){
	xmlDoc=node->GetDocument();
	this->pushNode(node);
}
$::$(vector<TiXmlNode*> vecNode):isPreDefined(false){
	if (vecNode.empty()){
		xmlDoc=NULL;
	}else{
		xmlDoc=vecNode[0]->GetDocument();
		$_SelElement=vecNode;
	}
}
$::$(TiXmlDocument* _xmlDoc,string filter,string root):xmlDoc(_xmlDoc),isPreDefined(false){	//˽�й��죬���ڳ����ڲ�
	if (xmlDoc){														//ʹ��ǰ��ؼ��_xmlDoc�Ƿ����
		$_SelElement=select(filter,$(root,xmlDoc)).$_SelElement;
	}
}
$::$(string filter,TiXmlNode* root):isPreDefined(false){
	this->xmlDoc = root->GetDocument();
	select(filter,root);
}
$::$(string filter,$ root):isPreDefined(false){									//˽�й��죬���ڳ����ڲ�
	this->xmlDoc=root.xmlDoc;
	if (xmlDoc){
		$_SelElement=select(filter,root).$_SelElement;
	}
}
$::$(const char * xmlFileName):isPreDefined(false){
	loadXml(xmlFileName);
}
$::$(const char * xmlFileName,string filter,string rootSel):isPreDefined(false){
	loadXml(xmlFileName);
	select(filter,rootSel);
}
$::$():isPreDefined(false){ 
	xmlDoc=new TiXmlDocument(); 
}
$::~$(){ /*we should not delete the xmlDoc as it was shared through the application*/ }


$& $::loadXml(const char * xmlFileName){
	if (string(xmlFileName)!="" && fileMap.find(xmlFileName)!=fileMap.end()){
		recentXmlDoc=xmlDoc=fileMap[xmlFileName];
	}else{
		recentXmlDoc=xmlDoc=fileMap[xmlFileName]=new TiXmlDocument(xmlFileName);
		bool loadedSuccess=xmlDoc->LoadFile(xmlDoc->ValueStr()/*,TIXML_ENCODING_UTF8*/);
		if (!loadedSuccess){
			string newFileName= string(xmlFileName)=="" ? "temp.xml" : xmlFileName;
			xmlDoc->SaveFile(newFileName.c_str());				//�������ʧ�ܣ����½���ʱ�ļ��������û������ļ�����ʱ�洢
			recentXmlDoc=xmlDoc=fileMap[xmlFileName]=new TiXmlDocument(newFileName.c_str());
		}
	}
	return *this;
}
bool $::save(string filename){
	filename = filename=="" ? xmlDoc->ValueStr() : filename;  //Ĭ���ļ���
	bool success = xmlDoc->SaveFile(filename.c_str());
	xmlDoc->SetValue(filename.c_str());
	return success;
}
$& $::setPreDefinedObj(){
	this->isPreDefined=true;
	return *this;
}

// selector and filter
$& $::select(string filter,string rootStr){
	VecCombine($_SelElement,$(xmlDoc,filter,rootStr).getSelection());
	return this->clearSame();
}
$& $::select(string filter,$& root ){
	if (root.isEmpty()){
		VecCombine($_SelElement,select(filter).getSelection());
	}else{
		for (unsigned int i=0; i<root.getSelection().size(); i++){
			VecCombine(
				this->$_SelElement,
				$(filter,root[i]).getSelection()
			);
		}
	}
	return this->clearSame();
}
$& $::select(string filter,TiXmlNode* root){
	// TODO : �ڸ���root���ҳ�����filter�����нڵ�
	// ����ʹ��ǿ���conEx����������
	root = !root ? xmlDoc : root;
	if (!notBindXml()){
		conExParser par(filter);
		$_SelElement=par.match(root);
	}else{
		cout<<"xmlDoc �����ڣ����� [�����]"<<endl;
		cout<<"Ҳ�п�����ʹ�����½ṹ $(TiXmlNode*) ��TiXmlNode*�ǹ�����"<<endl;
	}
	return this->clearSame();
}/*Main Proc*/
$& $::operator()(string selector,string root){
	if ( isPreDefined ) xmlDoc=recentXmlDoc;
	if (""==root) return this->clear().select(selector);
	return this->clear().select(selector,root);
}
$& $::operator()(string selector,$& root){
	if ( root.isPreDefined ) root.xmlDoc=recentXmlDoc;
	$ protectRoot=root;//��root������������Ϊ����select��ʱ����ܻ�clear��������root�������� xQuery("Table",xQuery("Data"));
	return this->clear().select(selector,protectRoot);
}

$& $::filter(string condition ){
	// filter���ʽ��������Ȼ����ǰ��
	// ��������ʹ��filter�﷨
	if (""==condition) return *this;						//���ɸѡ�����ǿյ��Ǿ�ֱ�ӷ���

	string except="";
	condition=removeSection(condition,".not",except);		//�����:not(x)���ǲ��֣��ȴ��������Ĺ��ˣ��ٴ����ų��Ĺ���
	vector<TiXmlNode*> result;								//���ڱ���ÿ�ι��˵Ľ����ÿһ�εĹ���֮���ǻ��ϵ
	if (condition.find(".not")!=string::npos) {				//ֱ����:not(x)������
		VecCombine(result,filter(condition).getSelection());
	}
	vector<string> conditions=split(condition,'.');
	for (unsigned int i=0; i<conditions.size(); i++){
		if (
			conditions[i].find("at(")!=string::npos && conditions[i].find(")")!=string::npos ||
			conditions[i].find("eq(")!=string::npos && conditions[i].find(")")!=string::npos
		){
			int 
				start	=3,									//ֱ�Ӿ���3�ˣ���������ʲ���at����eq
				end		=conditions[i].find(")"),
				index	=strToInt(conditions[i].substr(start,end-start))-1;
			if (index>=0 && index<$_SelElement.size()) {	
				result.push_back($_SelElement[index]);
			}
		}else if (
			conditions[i].find("first")!=string::npos		//ѡ����һ��
		){
			VecCombine(result,this->filter(".at(1)").getSelection());
		}else if (
			conditions[i].find("last")!=string::npos		//ѡ�����һ��
		){
			int lastidex=this->$_SelElement.size();
			VecCombine(result,this->filter(".at("+intToStr(lastidex)+")").getSelection());
		}else if (
			conditions[i].find("even")!=string::npos		//ѡ����ż����
		){
			for (unsigned int i=1; i<$_SelElement.size(); i+=2){
				result.push_back($_SelElement[i]);
			}
		}else if (
			conditions[i].find("odd")!=string::npos			//ѡ����������
		){
			for (unsigned int j=0; j<$_SelElement.size(); j+=2){
				result.push_back($_SelElement[j]);
			}
		}else if (
			conditions[i].find("element")!=string::npos		//ѡ�������Ԫ�ؽڵ�
		){
			for (unsigned int j=0; j<$_SelElement.size(); j++){
				if ($_SelElement[j]->Type()==$::XML_ELEMENT){
					result.push_back($_SelElement[j]);
				}
			}
		}else{
			//
		}
	}
	if (result.size()) $_SelElement=result;	//���������˽��
	if (except!="") {		//������Ҫ�ų�һЩ���
		$_SelElement=this->except(except).getSelection();
	}	
	this->clearSame();		//���ȥ�����ܻ�����ظ���
	return this->popEmpty();
}
$& $::filter(NodeType type){
	return this->getNodeWithType(type);
}
$& $::except(string condition ){
	// except���ʽ��������Ȼ����ǰ��
	// ��������ʹ��except�﷨
	if (""==condition) return *this;						//���ɸѡ�����ǿյ��Ǿ�ֱ�ӷ���
	
	//���벻������.����condition��ֱ�ӵ�������ĺ����ų�
	if (condition.find('.')==string::npos){
		return this->removeFromSelector(condition);
	}

	string filter="";
	condition=removeSection(condition,".not",filter);		//�����.not(x)���ǲ��֣�����Ͷ������filter�Ǳߴ���

	if (condition.find(".not")!=string::npos) {				//ֱ����.not(x)������
		$_SelElement=this->except(condition).getSelection();
	}
	vector<TiXmlNode*> result=$_SelElement;
	
	vector<string> conditions=split(condition,'.');
	vector<string> otherConditions;
	for (unsigned int i=0; i<conditions.size(); i++){
		if (
			conditions[i].find("at(")!=string::npos && conditions[i].find(")")!=string::npos ||
			conditions[i].find("eq(")!=string::npos && conditions[i].find(")")!=string::npos
		){
			int 
				start	=3,									//ֱ�Ӿ���3�ˣ���������ʲ���at����eq
				end		=conditions[i].find(")"),
				index	=strToInt(conditions[i].substr(start,end-start))-1;
			if (index>=0 && index<result.size()) {	
				result[index]=NULL;
			}
		}else if (
			conditions[i].find("first")!=string::npos		//ѡ����һ��
		){
			result[0]=NULL;
		}else if (
			conditions[i].find("last")!=string::npos		//ѡ�����һ��
		){
			int lastidex=this->$_SelElement.size();
			result[lastidex]=NULL;
		}else if (
			conditions[i].find("even")!=string::npos		//ѡ����ż����
		){
			for (unsigned int j=1; j<$_SelElement.size(); j+=2){
				result[j]=NULL;
			}
		}else if (
			conditions[i].find("odd")!=string::npos			//ѡ����������
		){
			for (unsigned int j=0; j<$_SelElement.size(); j+=2){
				result[j]=NULL;
			}
		}else if (
			conditions[i].find("element")!=string::npos		//�ų�Ԫ�ؽڵ�
		){
			for (unsigned int j=0; j<$_SelElement.size(); j++){
				if ($_SelElement[i]->Type()!=$::XML_ELEMENT){
					result[j]=NULL;
				}
			}
		}else{
			otherConditions.push_back(conditions[i]);
		}
	}
	int size=$_SelElement.size();
	$_SelElement.clear();
	for (unsigned int i=0; i<size; i++){
		if (result[i]) $_SelElement.push_back(result[i]);
	}
	if (filter!="") {		//�������п��ܻ�Ҫ�ӻ���һЩ����
		VecCombine($_SelElement,this->filter(filter).getSelection());//�ų���֮����Ҫ�ӻ���˫not�ڵĹ�������
	}
	this->clearSame();			//�����ܻ�����ظ���
	
	// ����������ͨ������ų�������except�Ĺ��ܣ�����ע�ⲻҪ������������ã�
	//����˵.except(tag.first) ��ʾȥ����ǰѡ�����еĵ�һ����Ȼ���ȥ��<tag>�������ǵ�һ��<tag>
	//�������firstҲ�Ǳ�ǩ���ģ�Ҫȥ��<first>����ôֻ����removeFromSelector(first);
	for (UINT i=0; i<otherConditions.size(); i++){
		this->removeFromSelector(otherConditions[i]);
	}
	
	return this->popEmpty();
}

$& $::next(string condition){
	if (this->isEmpty()) return *this;							//ѡ����Ϊ����ֱ�ӷ���
	vector<TiXmlNode*> result;
	// debug : adolli
	// skip non-element node. 2012-12-25
	
	// use conEx Engine!
	conEx conex(condition);
	for (unsigned int i=0; i<$_SelElement.size(); i++){
		TiXmlNode* next=$_SelElement[i]->NextSibling();
		while(next && next->Type()==$::XML_TEXT){
			next=next->NextSibling();
		}
		if (conex.match(next)){
			result.push_back(next);
		}
	}
	
	$_SelElement=result;
	return this->clearSame();
}
$& $::previous(string condition){
	if (this->isEmpty()) return *this;							//ѡ����Ϊ����ֱ�ӷ���
	vector<TiXmlNode*> result;
	// debug : adolli
	// skip non-element node. tixml does not supply previousSiblingElement method
	
	// use conEx Engine!
	conEx conex(condition);
	for (unsigned int i=0; i<$_SelElement.size(); i++){
		TiXmlNode* prev=$_SelElement[i]->PreviousSibling();
		while(prev && prev->Type()==$::XML_TEXT){
			prev=prev->PreviousSibling();
		}
		if (conex.match(prev)){
			result.push_back(prev);
		} 
	}
	$_SelElement=result;
	return this->clearSame();
}
$& $::nextAll(string condition){
	// ����nextAll��previousAll����ѡ���������͵Ľڵ㣬��ֻ��Ԫ�ؽڵ�
	if (this->isEmpty()) return *this;							//ѡ����Ϊ����ֱ�ӷ���
	vector<TiXmlNode*> result;
	conEx conex(condition);
	for (UINT i=0; i<$_SelElement.size(); i++){
		for (TiXmlNode* next=$_SelElement[i]->NextSibling(); next; next=next->NextSibling()){
			if (next->Type()!=$::XML_TEXT && conex.match(next)){
				result.push_back(next);
			}
		}
	}
	$_SelElement=result;
	return this->clearSame();
}
$& $::previousAll(string condition){
	if (this->isEmpty()) return *this;							//ѡ����Ϊ����ֱ�ӷ���
	vector<TiXmlNode*> result;
	conEx conex(condition);
	for (UINT i=0; i<$_SelElement.size(); i++){
		for (TiXmlNode* prev=$_SelElement[i]->PreviousSibling(); prev; prev=prev->PreviousSibling()){
			if (prev->Type()!=$::XML_TEXT && conex.match(prev)){
				result.push_back(prev);
			}
		}
	}
	$_SelElement=result;
	return this->clearSame();
}

$& $::parent(){
	// ��ѡ���������нڵ㶼ת�������ǵĸ��ڵ㣬Ȼ������յĺ��ظ���
	vector<TiXmlNode*> result;
	for (unsigned int i=0; i<$_SelElement.size(); i++){
		if ($_SelElement[i]->Parent()) {
			result.push_back($_SelElement[i]->Parent());
		}
	}
	$_SelElement=result;
	return this->clearSame();
}
$& $::ancestors(){
	// ��ѡ���������нڵ㶼ת�������ǵ����ȵ㣬Ȼ������յĺ��ظ���
	vector<TiXmlNode*> result;
	for (unsigned int i=0; i<$_SelElement.size(); i++){
		TiXmlNode* parent=$_SelElement[i]->Parent();
		while(parent){
			result.push_back(parent);
			parent=parent->Parent();
		}
	}
	$_SelElement=result;
	return this->popEmpty().clearSame();
}
$& $::children(string selector){
	// use conExParser Engine!
	conExParser par(selector);
	vector<TiXmlNode*> result;
	for (UINT i=0; i<$_SelElement.size(); i++){
		VecCombine(result,par.matchOnce($_SelElement[i]));
	}
	$_SelElement=result;
	return this->clearSame();
}
$& $::offspring(string selector){
	// use conExParser Engine!
	conExParser par(selector);
	vector<TiXmlNode*> result;
	for (UINT i=0; i<$_SelElement.size(); i++){
		VecCombine(result,par.match($_SelElement[i]));
	}
	$_SelElement=result;
	return this->clearSame();
}
bool $::noChildren(){
	if (this->isEmpty()) return true;
	return $_SelElement[0]->NoChildren();
}


// append functions
template<typename T>$& $::_append(T node_type,int method){
	//���ж��ĵ��ǲ��ǿյģ��յ�ֻ������������������һ���ڵ�
	if (xmlDoc && xmlDoc->NoChildren() || this->isEmpty()){
		xmlDoc->InsertEndChild(*node_type);
		return *this;
	}

	method=method & (~$::APDM_DOUBLETAG) & (~$::APDM_SELFCLOSE);	//ȥ������������Ӱ��
	switch(method){

	case $::APDM_ALL|$::APOS_AFTER :
		for (unsigned int i=0; i<$_SelElement.size(); i++){
			$_SelElement[i]->Parent()->InsertAfterChild($_SelElement[i],*node_type);
		}
		break;

	case $::APDM_ALL|$::APOS_BEFORE :
		for (unsigned int i=0; i<$_SelElement.size(); i++){
			$_SelElement[i]->Parent()->InsertBeforeChild($_SelElement[i],*node_type);
		}
		break;

	case $::APDM_ALL|$::APOS_AFTER|$::APOS_INSIDE :	
		for (unsigned int i=0; i<$_SelElement.size(); i++){
			$_SelElement[i]->InsertEndChild(*node_type);
		}
		break;

	case $::APDM_ALL|$::APOS_BEFORE|$::APOS_INSIDE :
		for (unsigned int i=0; i<$_SelElement.size(); i++){
			if ($_SelElement[i]->NoChildren()){					//���û�к����൱��ֱ�Ӳ����ڲ����
				$_SelElement[i]->InsertEndChild(*node_type);
			}else{
				$_SelElement[i]->InsertBeforeChild($_SelElement[i]->FirstChild(),*node_type);
			}
		}
		break;

	case $::APOS_AFTER :
		lastSelection()->Parent()->InsertAfterChild(lastSelection(),*node_type);
		break;

	case $::APOS_BEFORE :
		lastSelection()->Parent()->InsertBeforeChild(lastSelection(),*node_type);
		break;

	case $::APOS_AFTER|$::APOS_INSIDE :
		lastSelection()->InsertEndChild(*node_type);
		break;

	case $::APOS_BEFORE|$::APOS_INSIDE :
		if (lastSelection()->NoChildren()){					//���û�к����൱��ֱ�Ӳ����ڲ����
			lastSelection()->InsertEndChild(*node_type);
		}else{
			lastSelection()->InsertBeforeChild(lastSelection()->FirstChild(),*node_type);
		}
		break;

	default:
		for (unsigned int i=0; i<$_SelElement.size(); i++){
			$_SelElement[i]->InsertEndChild(*node_type);
		}
		break;
	}
	return *this;
}
$& $::appendDeclaration( string ver, string encoding, string standalone,int method){
	TiXmlDeclaration* decl = new TiXmlDeclaration(ver,encoding,standalone);
	if (!xmlDoc) {
		xmlDoc=new TiXmlDocument();
		xmlDoc->LinkEndChild(decl);
	}else if (!xmlDoc->FirstChild()){
		// debug : adolli 
		// if the xmlDoc has no children we directly link the decl node at the end of xmlDoc
		// so add another condition to enter this block
		xmlDoc->LinkEndChild(decl);
	}else{
		$(".first",xmlDoc).filter($::XML_DECLARATION).remove();
		// this does not work if xmlDoc->FirstChild()==NULL
		xmlDoc->InsertBeforeChild(xmlDoc->FirstChild(),*decl);
	}
	return *this;
}
$& $::appendElement(string tagName,int method){
	if (tagName!=""){
		TiXmlElement* element = new TiXmlElement(tagName.c_str());
		if (! (method & $::APDM_SELFCLOSE)){											//���Զ��պϱ�ǩ�ж�
			TiXmlText* blankText = new TiXmlText("");//����ط������˺ܾã���Ҫ��Ϊ�˸���ԭ�����Զ��պ����ԣ��ο�tixml��Դ����
			element->LinkEndChild(blankText);
			// to link child as every time we make a new pointer
		}
		_append(element,method);
		delete element;// delete when use over
	}
	return *this;
}
$& $::appendText(string text,int method){
	TiXmlText* _text = new TiXmlText(text.c_str());
	_append(_text,method);
	delete _text;
	return *this;
}
$& $::appendComment(string comment,int method){
	if (comment!=""){
		TiXmlComment* _comment = new TiXmlComment(comment.c_str());
		_append(_comment,method);
		delete _comment;
	}
	return *this;
}

$& $::removeFromSelector(string selector){
	this->subtractBy($(selector,xmlDoc).$_SelElement);
	return *this;
}
$& $::removeFromSelector(int index){
	index=index-1;//��1��ʼ����
	if ( index>=0 && index<$_SelElement.size() ){
		$_SelElement[index]=NULL;
	}
	return this->popEmpty();
}


$& $::remove(string selector){
	vector<TiXmlNode*> deOffspring;
	if (""==selector) {
		// we should only remove the highest parent
		// or we'll meet the same exception as in replaceWith();
		// ���ڵ㱻�Ƴ���֮���ӽڵ�Ͳ������ˣ�����ֻ��Ҫ�Ƴ������ڵĽڵ�
		for (UINT i=0; i<$_SelElement.size(); i++){
			// ��ȡ�����ķ�������߱��ֵĽڵ�һ�������$_SelElement�нڵ����ص�
			if ($($_SelElement[i]).ancestors().intersectionWith($_SelElement).isEmpty()){
				deOffspring.push_back($_SelElement[i]);
			}
		}
		this->clear();
	}else{
		// first remove node from selector
		removeFromSelector(selector);

		vector<TiXmlNode*> selectNode=$(selector,xmlDoc).getSelection();
		for (UINT i=0; i<$_SelElement.size(); i++){
			// ��ȡ�����ķ�������߱��ֵĽڵ�һ�������$_SelElement�нڵ����ص�
			if ($(selectNode).ancestors().intersectionWith(selectNode[i]).isEmpty()){
				deOffspring.push_back(selectNode[i]);
			}
		}		
	}
	// then remove from DOM. if we remove from DOM first, some nodes(NULL) will be left in selector
	for (unsigned int i=0; i<deOffspring.size(); i++){
		deOffspring[i]->Parent()->RemoveChild(deOffspring[i]);
	}
	return *this;
}
$& $::removeText(string selector){
	if (""==selector){
		//�Ȱ�ѡ�����е��ı��ڵ��xml���Ƴ�
		vector<TiXmlNode*> result;
		for (UINT i=0; i<$_SelElement.size(); i++){
			if ($_SelElement[i]->Type()==$::XML_TEXT){
				$_SelElement[i]->Parent()->RemoveChild($_SelElement[i]);
				//$_SelElement[i]=NULL;
			}else{
				result.push_back($_SelElement[i]);
			}
		}
		$_SelElement=result;

		//����ѡ�����еĽڵ�ĺ��ӽڵ�
		for (UINT i=0; i<$_SelElement.size(); i++){
			TiXmlNode* child=$_SelElement[i]->FirstChild();
			TiXmlNode* toBeRemoved=NULL;
			while(child){
				if (child->Type()==$::XML_TEXT){
					toBeRemoved=child;
					child=child->NextSibling();
					toBeRemoved->Parent()->RemoveChild(toBeRemoved);
					// debug : adolli
					// every time we after removing node we should set NULL to the pointer
					toBeRemoved=NULL;
				}else{
					child=child->NextSibling();
				}
			}
		}
	}else{
		$(selector,xmlDoc).removeText();
	}
	return this->popEmpty();
}
$& $::replaceWith(string withThis,int copyMethod){
	return this->replaceWith($(*this)(withThis),copyMethod);
}
$& $::replaceWith($& withThis,int copyMethod){
	// TODO : ��withThis�еĽڵ�clone���滻����ǰselector�еĽڵ�
	// CM_With_singleNODE	: ǳ����
	// CM_With_whole_TREE	: ���
	// CM_DeepCopy			: ���
	// CM_ShallowCopy		: ǳ����
	
	if (withThis.isEmpty()) return *this;

	vector<TiXmlNode*> replaceWithTheNode;
	TiXmlNode* tempNode;
	
	switch(copyMethod){
	case CM_With_whole_TREE:
	case CM_DeepCopy:
		// copy������Ľڵ�
		for (UINT i=0; i<withThis.length(); i++){
			replaceWithTheNode.push_back(withThis[i]->Clone());
		}
		break;

	case CM_With_singleNODE:
	case CM_ShallowCopy:
	default:
		// copy��ǳ�����Ľڵ�
		for (UINT i=0; i<withThis.length(); i++){
			tempNode=withThis[i]->Clone();
			tempNode->Clear();
			replaceWithTheNode.push_back(tempNode);
		}
	}

	// �滻�ڵ��е����⣬$_SelElement�����и��ӹ�ϵ�õ��ڵ㣬
	// ���ڵ㱻�滻��֮���ӽڵ�Ͳ������ˣ�����ֻ���滻�����ڵĽڵ�
	vector<TiXmlNode*> deOffspring;
	for (UINT i=0; i<$_SelElement.size(); i++){
		// ��ȡ�����ķ�������߱��ֵĽڵ�һ�������$_SelElement�нڵ����ص�
		if ($($_SelElement[i]).ancestors().intersectionWith($_SelElement).isEmpty()){
			deOffspring.push_back($_SelElement[i]);
		}
	}

	vector<TiXmlNode*> result;//���ڱ����»���ȥ�Ľڵ�
	for (UINT i=0; i<deOffspring.size(); i++){
		
		// �Ȱ�Ҫ���ɵĽڵ����һ������ӵ������Ľڵ��
		for (UINT j=1; j<replaceWithTheNode.size(); j++){
			result.push_back(deOffspring[i]->Parent()->InsertAfterChild(deOffspring[i],*replaceWithTheNode[j]));
		}
		// Ȼ���Ҫ���Ľڵ㻻��toPlaceWithThis[0]
		result.push_back(deOffspring[i]->Parent()->ReplaceChild(deOffspring[i],*replaceWithTheNode[0]));
	}
	this->$_SelElement=result;
	return this->popEmpty().clearSame();
}
$& $::replaceWithNewElement(string tagName){
	TiXmlNode* newElement=new TiXmlElement(tagName);
	replaceWith($(newElement));
	return *this;
}

$& $::clone($& pasteTo,int position,int copyMethod){
	//���ж�pasteTo�Ƿ�Ϊ��
	if (pasteTo.isEmpty()) return *this;

	vector<TiXmlNode*> cloneNode;
	TiXmlNode* tempNode;
	
	// ����ģʽ�Ŀ���
	switch(copyMethod){
	case CM_With_whole_TREE:
	case CM_DeepCopy:
		// copy������Ľڵ�
		for (UINT i=0; i<$_SelElement.size(); i++){
			cloneNode.push_back($_SelElement[i]->Clone());
		}
		break;

	case CM_With_singleNODE:
	case CM_ShallowCopy:
	default:
		// copy��ǳ�����Ľڵ�
		for (UINT i=0; i<$_SelElement.size(); i++){
			tempNode=$_SelElement[i]->Clone();
			tempNode->Clear();
			cloneNode.push_back(tempNode);
		}
	}
	
	// result���������²���Ľڵ㣬��󷵻ظ�ѡ����
	vector<TiXmlNode*> result;
	// ����λ�õĿ���
	switch (position){
	case $::APOS_AFTER :
		for (UINT i=0; i<cloneNode.size(); i++) {
			for (UINT j=0; j<pasteTo.length(); j++){
				result.push_back(pasteTo[j]->Parent()->InsertAfterChild(pasteTo[j],*cloneNode[i]));
			}
		}
		break;
	case $::APOS_BEFORE :
		for (UINT i=0; i<cloneNode.size(); i++) {
			for (UINT j=0; j<pasteTo.length(); j++){
				result.push_back(pasteTo[j]->Parent()->InsertBeforeChild(pasteTo[j],*cloneNode[i]));
			}
		}
		break;
	case $::APOS_INSIDE|$::APOS_AFTER :
		for (UINT i=0; i<cloneNode.size(); i++) {
			for (UINT j=0; j<pasteTo.length(); j++){
				result.push_back(pasteTo[j]->InsertEndChild(*cloneNode[i]));
			}
		}
		break;
	case $::APOS_INSIDE|$::APOS_BEFORE :
		for (UINT i=0; i<cloneNode.size(); i++) {
			for (UINT j=0; j<pasteTo.length(); j++){
				result.push_back(pasteTo[j]->InsertBeforeChild(pasteTo[j]->FirstChild(),*cloneNode[i]));
			}
		}
		break;
	default ://Ĭ���ǲ��뵽pasteTo�Ľڵ��
		for (UINT i=0; i<cloneNode.size(); i++) {
			for (UINT j=0; j<pasteTo.length(); j++){
				result.push_back(pasteTo[j]->Parent()->InsertAfterChild(pasteTo[j],*cloneNode[i]));
			}
		}
		break;
	}
	$_SelElement=result;
	return *this;
}
$& $::cloneTo($& pasteTo,int position,int copyMethod){
	//���ж�pasteTo�Ƿ�Ϊ��
	if (pasteTo.isEmpty()) return *this;

	vector<TiXmlNode*> cloneNode;
	TiXmlNode* tempNode;
	
	// ����ģʽ�Ŀ���
	switch(copyMethod){
	case CM_With_whole_TREE:
	case CM_DeepCopy:
		// copy������Ľڵ�
		for (UINT i=0; i<$_SelElement.size(); i++){
			cloneNode.push_back($_SelElement[i]->Clone());
		}
		break;

	case CM_With_singleNODE:
	case CM_ShallowCopy:
	default:
		// copy��ǳ�����Ľڵ�
		for (UINT i=0; i<$_SelElement.size(); i++){
			tempNode=$_SelElement[i]->Clone();
			tempNode->Clear();
			cloneNode.push_back(tempNode);
		}
	}
	
	// ����λ�õĿ���
	switch (position){
	case $::APOS_AFTER :
		for (UINT i=0; i<cloneNode.size(); i++) {
			for (UINT j=0; j<pasteTo.length(); j++){
				pasteTo[j]->Parent()->InsertAfterChild(pasteTo[j],*cloneNode[i]);
			}
		}
		break;
	case $::APOS_BEFORE :
		for (UINT i=0; i<cloneNode.size(); i++) {
			for (UINT j=0; j<pasteTo.length(); j++){
				pasteTo[j]->Parent()->InsertBeforeChild(pasteTo[j],*cloneNode[i]);
			}
		}
		break;
	case $::APOS_INSIDE|$::APOS_AFTER :
		for (UINT i=0; i<cloneNode.size(); i++) {
			for (UINT j=0; j<pasteTo.length(); j++){
				pasteTo[j]->InsertEndChild(*cloneNode[i]);
			}
		}
		break;
	case $::APOS_INSIDE|$::APOS_BEFORE :
		for (UINT i=0; i<cloneNode.size(); i++) {
			for (UINT j=0; j<pasteTo.length(); j++){
				pasteTo[j]->InsertBeforeChild(pasteTo[j]->FirstChild(),*cloneNode[i]);
			}
		}
		break;
	default ://Ĭ���ǲ��뵽pasteTo�Ľڵ��
		for (UINT i=0; i<cloneNode.size(); i++) {
			for (UINT j=0; j<pasteTo.length(); j++){
				pasteTo[j]->Parent()->InsertAfterChild(pasteTo[j],*cloneNode[i]);
			}
		}
		break;
	}
	return *this;
}
$& $::wrapBy($& byThis){
	if (byThis.isEmpty()) return *this;

	vector<TiXmlNode*> result;
	while(!isEmpty()){		
		for (UINT i=0; i<$_SelElement.size(); i++){
			// �Ȱ�������Ľڵ�����ˣ����������ѡ�������������ڵ�
			// Ҫһ��һ�����ϰ�����ֱ��ѡ������û�нڵ�Ҫ����Ϊֹ
			if ($($_SelElement[i]).offspring().intersectionWith($_SelElement).isEmpty()){
				TiXmlNode* wrapper=$_SelElement[i]->Parent()->InsertAfterChild($_SelElement[i],*byThis[0]);
				wrapper->Clear();
				result.push_back(wrapper->InsertEndChild(*$_SelElement[i]));
				$_SelElement[i]->Parent()->RemoveChild($_SelElement[i]);
				$_SelElement[i]=NULL;
			}
		}
		this->popEmpty();
	}

	// ����ѡ�����з����°����õĽڵ�
	$_SelElement=result;
	return this->popEmpty().clearSame();
}
$& $::wrapByElement(string tagName){
	TiXmlNode* element=new TiXmlElement(tagName);
	return this->wrapBy($(element));
}

$& $::selectAllNode(TiXmlNode* root){
	conExParser par;
	$_SelElement=par.match(root);
//	$_SelElement=traverse(root, alwaysRtTrue ,"");
	return *this;
}
$& $::selectAllNode($& root){
	if (!root.isEmpty()){
		$_SelElement=traverse(root[0], alwaysRtTrue ,"");
	}
	return *this;
}

$& $::clear(){
	$_SelElement.clear();
	return *this;
}
$& $::clearSame(){
	vector<TiXmlNode*> result;						//��ʱ��Ž��
	unsigned int i=0,j=0;
	for (i=0; i<$_SelElement.size(); i++){
		bool hasSame=false;							//ÿ��ѭ����ʼ���趼û����ͬ��
		for (j=i+1; j<$_SelElement.size(); j++){	//������Ԫ�����Ƚ�
			if ($_SelElement[i]==$_SelElement[j]) {
				hasSame=true;						//�������ͬ�ľ�ֱ����������ѭ��
				break;
			}
		}
		if (!hasSame) result.push_back($_SelElement[i]);//���û����ͬ�ľͷ�������
	}
	$_SelElement=result;							//��󱣴���
	return *this;
}
$& $::popEmpty(){
	vector<TiXmlNode*> result;
	for (unsigned int i=0; i<$_SelElement.size(); i++){
		if ($_SelElement[i]) result.push_back($_SelElement[i]);
	}
	$_SelElement=result;
	return *this;
}
$& $::pushNode(TiXmlNode* node){
	if (node->GetDocument()==xmlDoc){		//Ҫ�������ڵ��Ƿ����ڵ�ǰ�ĵ�
		this->$_SelElement.push_back(node);
	}
	return *this;
}

$& $::subtractBy(const vector<TiXmlNode*> & withThis){
	vector<TiXmlNode*> result;
	unsigned int a=0,b=0;	
	for(a=0; a<withThis.size(); a++){				//ɨ��Ҫȥ���Ľڵ�
		result.clear();								//��գ�׼����һ���ջ�ʣ�µĽڵ�
		for(b=0; b<$_SelElement.size(); b++){
			if (withThis[a]!=$_SelElement[b]){		//��ͬ�ľʹ�����
				result.push_back($_SelElement[b]);
			}
		}
		$_SelElement=result;						//ÿ�ζ���ʣ�µĽڵ㱣���element
	}
	return this->clearSame();
}
$& $::intersectionWith(const vector<TiXmlNode*> & compare){
	vector<TiXmlNode*> result;
	unsigned int a=0,b=0;	
	for(a=0;a<$_SelElement.size();a++){
		for(b=0;b<compare.size();b++){
			if (compare[b]==$_SelElement[a]){
				result.push_back($_SelElement[a]);
			}
		}
	}
	$_SelElement=result;
	return *this;
}
$& $::intersectionWith(const $ & compare){
	this->intersectionWith(compare.$_SelElement);
	return *this;
}
$& $::displaySelTagName(){
	for (unsigned int i=0;i<$_SelElement.size();i++){
		cout<<i<<" : "<<$_SelElement[i]->ValueStr()<<endl;
	}
	return *this;
}
$& $::print(printMethod method){
	if (xmlDoc){
		if (method & $::PM_TEXT){
			xmlDoc->Print();
		}else{
			dump_to_stdout(xmlDoc->Value());
		}
	}else{
		cout<<" File loaded unsuccessful! Please check the target of your xml document. "<<endl;
	}
	return *this;
}
string $::fileName(){
	if (!xmlDoc) return "";
	return xmlDoc->ValueStr();
}

$& $::operator+=(const $& addThis){
	if (this->xmlDoc==addThis.xmlDoc){
		VecCombine(this->$_SelElement,addThis.$_SelElement);
	}
	return this->clearSame();
}
$& $::operator- (const $& substractThis){
	if (this->xmlDoc==substractThis.xmlDoc){
		this->subtractBy(substractThis.$_SelElement);
	}
	return *this;
}
ostream& operator<<(ostream & out,$& xQuery){
	xQuery.print();
	return out;
}

string				$::text(){
	if (this->isEmpty()) return "";
	if ($_SelElement[0]->Type()==$::XML_TEXT) return $_SelElement[0]->ValueStr();
	
	vector<TiXmlNode*> textNode;
	for (TiXmlNode* child=$_SelElement[0]->FirstChild(); child; child=child->NextSibling()){
		if (child->Type()==$::XML_TEXT){
			textNode.push_back(child);
		}
	}

	string textStr="";
	for (unsigned int i=0; i<textNode.size(); i++){
		textStr += textNode[i]->ValueStr();
	}
	return textStr;
}
string				$::tagName(int index){
	if (index>=0 && index<$_SelElement.size()){
		if ($_SelElement[index]->Type()==$::XML_ELEMENT){
			return $_SelElement[index]->ValueStr();
		}else{
			return "_THIS_IS_NOT_ELEMENT_NODE_";
		}
	}else{
		return "_OUT_OF_RANGE_WHEN_QUERYING_TAGNAME_";
	}
}
int					$::nodeType(){
	if (!isEmpty()){
		return this->$_SelElement[0]->Type();
	}else{
		return -1;
	}
}
vector<TiXmlNode*>	$::getSelection(int index){
	if ( index>=0 ){
		vector<TiXmlNode*> ret;
		ret.push_back($_SelElement[index]);
		return ret;
	}else{
		return $_SelElement;
	}
}
TiXmlNode*			$::lastSelection(){
	if (!isEmpty()){
		return $_SelElement[$_SelElement.size()-1];
	}else{
		return NULL;
	}
}
TiXmlNode*			$::getNode(int index){
	return this->getSelection(index)[0];
}
TiXmlNode*			$::operator[](int index){
	if (index>=0){
		return $_SelElement[index];
	}else{
		return NULL;
	}
}
unsigned int		$::length()const{
	return $_SelElement.size();
}
template<typename Ret_type,typename Para_type> Ret_type $::each(Para_type (*function)()){

}

// node test

bool $::notBindXml(){
	return NULL==xmlDoc;
}
bool $::isEmpty(){
	return 0==$_SelElement.size();
}
bool $::hasNode(TiXmlNode* Node){
	for (unsigned int i=0; i<$_SelElement.size(); i++){
		if ($_SelElement[i]==Node) return true;
	}
	return false;
}
bool $::testNodeType(TiXmlNode* Node,int type){
	return Node->Type()==type;
}

$::OS_Validation $::hasOffSpring(const string& selector){
	if (this->isEmpty() )	return $::OS_NONE;
	
	vector<TiXmlNode*> offspring=$(selector,xmlDoc).getSelection();
	bool gotParent=false;
	vector<bool> getParentStatus;
	
	for (unsigned int i=0; i<offspring.size(); i++){
		gotParent=false;
		TiXmlNode * parent=offspring[i]->Parent();
		while (parent){								//׷�ݵ����ڵ�Ѱ�ҿ��Ƿ��ҵ����ڵ�
			if (parent==$_SelElement[0]){
				gotParent=true;
				break;
			}
			parent=parent->Parent();
		}
		getParentStatus.push_back(gotParent);
	}

	bool findTrue=false,findFalse=false;
	for (unsigned int i=0; i<getParentStatus.size(); i++){//�жϽ�����
		if (getParentStatus[i]) {
			findTrue=true;
		}else{
			findFalse=true;
		}
	}
	//�����Ƿ����ҵ��Ĳ�ͬ���ز�ͬ�����Ϣ
	if (findTrue && findFalse){
		return $::OS_PART;
	}else if (findTrue && !findFalse){
		return $::OS_ALL;
	}else{
		return $::OS_NONE;
	}
}
bool $::has(NodeType type){
	for (unsigned int i=0; i<$_SelElement.size(); i++){
		if ($_SelElement[i]->Type()==type){
			return true;
		}
	}
	return false;
}
$& $::getNodeWithType(NodeType type){
	for (unsigned int i=0; i<$_SelElement.size(); i++){
		if ($_SelElement[i]->Type()!=type){
			$_SelElement[i]=NULL;
		}
	}
	return this->popEmpty();
}

// attributes
string	$::attr (string attrname){
	if (
		!isEmpty() &&
		$_SelElement[0]->ToElement() &&
		$_SelElement[0]->ToElement()->Attribute(attrname.c_str())
	){
		return $_SelElement[0]->ToElement()->Attribute(attrname.c_str());
	}else{
		return "";
	}
}
int		$::iAttr(string attrname){
	if (this->isEmpty()) return $::QA_NO_ATTRIBUTE;
	
	int	ret;
	if ( TIXML_SUCCESS==$_SelElement[0]->ToElement()->QueryValueAttribute(attrname.c_str(),&ret) ){
		return ret;
	}else{
		return $::QA_NO_ATTRIBUTE;
	}
}
double	$::dAttr(string attrname){
	if (this->isEmpty()) return $::QA_NO_ATTRIBUTE;
	double	ret=0;
	if ( TIXML_SUCCESS==$_SelElement[0]->ToElement()->QueryValueAttribute(attrname.c_str(),&ret) ){
		return ret;
	}else if ( TIXML_WRONG_TYPE==$_SelElement[0]->ToElement()->QueryValueAttribute(attrname.c_str(),&ret) ){
		return (double)iAttr(attrname);
	}else{
		return $::QA_NO_ATTRIBUTE;
	}
}
bool	$::bAttr(string attrname){
	if (this->isEmpty()) return false;
	bool ret=false;
	$_SelElement[0]->ToElement()->QueryBoolAttribute(attrname.c_str(),&ret);
	return ret;
}
vector<string> $::getAllAttr(string attrname){
	vector<string> result;
	for (unsigned int i=0; i<$_SelElement.size(); i++){
		if (
			$_SelElement[i]->ToElement() &&
			$_SelElement[i]->ToElement()->Attribute(attrname.c_str())
		){
			result.push_back($_SelElement[i]->ToElement()->Attribute(attrname.c_str()));
		}else{
			result.push_back("$::ET_ATTR_NOT_FOUND");
		}
	}
	return result;
}
$& $::attr(string attrname,string value){
	if (""==value){
		for (unsigned int i=0; i<$_SelElement.size(); i++){
			$_SelElement[i]->ToElement()->RemoveAttribute(attrname);
		}
	}else{
		for (unsigned int i=0; i<$_SelElement.size(); i++){
			$_SelElement[i]->ToElement()->SetAttribute(attrname.c_str(),value.c_str());
		}
	}
	return *this;
}
$& $::attr(string attrname,int value){
	for (unsigned int i=0; i<$_SelElement.size(); i++){
		$_SelElement[i]->ToElement()->SetAttribute(attrname.c_str(),value);
	}
	return *this;
}
$& $::attr(string attrname,double value){
	for (unsigned int i=0; i<$_SelElement.size(); i++){
		$_SelElement[i]->ToElement()->SetDoubleAttribute(attrname.c_str(),value);
	}
	return *this;
}



// travers Function
vector<TiXmlNode*> traverse(TiXmlNode* rootElement,bool (*testOk)(TiXmlNode* Node,string test),string testValue){
	vector<TiXmlNode*> result;
	TiXmlNode* loopNode = rootElement->FirstChild();
	while(loopNode){
		// ���������������ı��ڵ㣬�����ı��ڵ�����.text()
		if(loopNode->Type()!=$::XML_TEXT && (*testOk)(loopNode,testValue)){
			result.push_back(loopNode);
		}
		VecCombine(result,traverse(loopNode,testOk,testValue));
		loopNode=loopNode->NextSibling();
	}
	return result;
}
vector<TiXmlNode*> traverseOnce(TiXmlNode* rootElement,bool (*testOk)(TiXmlNode* Node,string test),string testValue){
	vector<TiXmlNode*> result;
	TiXmlNode* loopNode = rootElement->FirstChild();
	while(loopNode){
		// ���������������ı��ڵ㣬�����ı��ڵ�����.text()
		if(loopNode->Type()!=$::XML_TEXT && (*testOk)(loopNode,testValue)){
			result.push_back(loopNode);
		}
		loopNode=loopNode->NextSibling();
	}
	return result;
}
vector<TiXmlNode*> traverseAmongNodes(vector<TiXmlNode*> amongThis,bool (*testOk)(TiXmlNode* Node,string test),string testValue){
	vector<TiXmlNode*> result;
	for (UINT i=0; i<amongThis.size(); i++){
		// ���������������ı��ڵ㣬�����ı��ڵ�����.text()
		if((*testOk)(amongThis[i],testValue) && amongThis[i]->Type()!=$::XML_TEXT){
			result.push_back(amongThis[i]);
		}
	}
	return result;
}

vector<string> split(string source,char splitSymbol){
	vector<string> result;
	while(1){
		int pos=source.find(splitSymbol);
		if(pos==0){
			result.push_back(string(""));//ǰ���շָ�����ʾǰ����һ������Ϊ0��string
			source=source.substr(1);
			continue;
		}else if(pos<0){
			result.push_back(source);
			break;
		}
		result.push_back(source.substr(0,pos));
		source=source.substr(pos+1);
	}
	return result;
}
string joint(const vector<string>& vec,string withThis){
	string ret="";
	if (!vec.empty()) {
		ret+=vec[0];
		for (UINT i=1;i<vec.size();i++){
			ret+=(withThis+vec[i]);
		}
	}
	return ret;
}

template<typename T> vector<T> & VecCombine(vector<T> & result,const vector<T> & add){
	if (result==add) return result;					//�������vec��ͬ�Ͳ������ظ��ӽ�ȥ��
	for (unsigned int i=0;i<add.size();i++){		//������
		result.push_back(add[i]);
	}
	return result;									//���ص�һ������������
}

int strToInt(string str){
	return atoi(str.c_str());
}
string intToStr(int num){
	char *str=new char[15];		//һ��int������15λ
	itoa(num,str,10);
	return string(str);
}
string doubleToStr(double num){
	string str;
    stringstream ss;
    ss<<num;
    ss>>str;
    return str;
}
double strToDouble(string str){
	return atof(str.c_str());
}

string removeSection(const string origin,string condition,string& removed){
	int pos=origin.find(condition);
	if (pos!=string::npos){
		bool matching=false;
		int
			lBracket=origin.find("(",pos),
			rBracket=origin.find(")",pos),
			BracketCount=0;
		if (lBracket==string::npos || rBracket==string::npos) return origin;
		for (unsigned int i=lBracket; i<origin.length(); i++ ){
			if ('('==origin[i]){
				BracketCount++;
			}else if (')'==origin[i]){
				BracketCount--;
			}
			if (0==BracketCount && lBracket<i) {
				rBracket=i;
				matching=true;
				break;
			}
		}
		if (matching){
			removed=origin.substr(lBracket+1,rBracket-lBracket-1);
			return origin.substr(0,pos)+origin.substr(rBracket+1);
		}
	}
	return origin;
}

// codecs
string& encode(string& encodeStr){
	//����ð�� ':'
	int pos=encodeStr.find(':');
	while (pos!=string::npos){
		encodeStr.replace(encodeStr.begin()+pos,encodeStr.begin()+pos+1,"<colon;");
		pos=encodeStr.find(':');
	}
	// ��������ӱ�ı������
	return encodeStr;
}
string& decode(string& decodeStr){
	//����ð�� ':'
	int pos=decodeStr.find("<colon;");
	while (pos!=string::npos){
		decodeStr.replace(decodeStr.begin()+pos,decodeStr.begin()+pos+7,":");
		pos=decodeStr.find("<colon;");
	}
	// ��������ӱ�Ľ������
	return decodeStr;
}
string& filterStrProc(string& filterStr){
	int 
		lPos=0,				//������λ��
		rPos=0;				//������λ��
	lPos=filterStr.find('[');
	rPos=filterStr.find(']');
	string procAttrPart;
	while (
		lPos!=string::npos &&
		rPos!=string::npos
	){
		procAttrPart=filterStr.substr(lPos,rPos-lPos+1);
		encode(procAttrPart);
		filterStr.replace(filterStr.begin()+lPos,filterStr.begin()+rPos+1,procAttrPart);
		lPos=filterStr.find('[',lPos+1);
		rPos=filterStr.find(']',lPos);
	}
	return filterStr;
}



