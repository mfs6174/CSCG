#include "stdafx.h"
#include "XMLDoc.h"

#include <assert.h>
#include  <boost/algorithm/string.hpp>

using std::string;
using std::vector;
using namespace boost;

#include "tinyxml.h"

namespace tixml{

	static void convertFromTiXML(XMLNode&node,TiXmlElement* elem){
		if(elem==NULL)
			return;
		
		node.setName(elem->Value());		
		if(elem->GetText()){
			node.setText(string(elem->GetText()));
		} else 
			node.setText(string(""));

		TiXmlAttribute* tiAttrib = elem->FirstAttribute();
		while(tiAttrib){
			node.attribs()[tiAttrib->Name()] = tiAttrib->Value();
			tiAttrib = tiAttrib->Next();
		}

		TiXmlNode* tiChild = elem->FirstChild();
		while(tiChild){

			TiXmlElement* tiChildElem = dynamic_cast<TiXmlElement*> (tiChild);
			if(  tiChildElem ){
				XMLNode* child= new XMLNode();
				convertFromTiXML(*child,tiChildElem);
				node.children().push_back(child);
			}

			tiChild = tiChild->NextSibling();
		}
	}

	static void convertToTiXML(XMLNode* pNode,TiXmlElement* elem){
		if(pNode==NULL)
			return;

		elem->SetValue(pNode->name());
		for(std::map<std::string,std::string>::iterator it = pNode->attribs().begin(); it!=pNode->attribs().end();it++){
			elem->SetAttribute(it->first,it->second);
		}

		string trimmedText = trim_copy(pNode->text<string>());
		if(trimmedText.length()>0) {
			TiXmlText * tiText = new TiXmlText(trimmedText);
			elem->LinkEndChild(tiText);
		}

		for(unsigned int i=0; i<pNode->children().size();i++){
			TiXmlElement* childElem = new TiXmlElement("");
			convertToTiXML(pNode->children()[i],childElem);
			elem->LinkEndChild(childElem);
		}

	}

	XMLDoc::XMLDoc(void)
	{
		m_pRoot = NULL;
	}


	XMLDoc::~XMLDoc(void)
	{
		if(m_pRoot)
			delete m_pRoot;
	}

	void XMLDoc::Create(const std::string & rootName,const std::map<std::string,std::string> & rootAttribs) {
		if(m_pRoot)
			delete m_pRoot;
		m_pRoot = new XMLNode(rootName,"",rootAttribs);
	}
	void XMLDoc::Create(const std::string & rootName){
		std::map<std::string,std::string> rootAttribs;
		Create(rootName,rootAttribs);
	}

	bool XMLDoc::Load(const char* fn) {
		
		TiXmlDocument doc;
		if(!doc.LoadFile(fn))
			return false;
		if(m_pRoot)
			delete m_pRoot;
		m_pRoot = new XMLNode();
		TiXmlElement* rootElem = doc.RootElement();
		convertFromTiXML(*m_pRoot,rootElem);

		return true;
	}
	bool XMLDoc::Save(const char* fn) {

		TiXmlDocument doc;
		TiXmlDeclaration * tiDecl = new TiXmlDeclaration("1.0","","");
		TiXmlElement * tiRoot = new TiXmlElement("");
		convertToTiXML(getRoot(),tiRoot);

		doc.LinkEndChild(tiDecl);
		doc.LinkEndChild(tiRoot);

		return doc.SaveFile(fn);

		return false;
	}
	
	unsigned int XMLNodeList::length() const {
		return m_list.size();
	}



	XMLNodeList XMLNodeList::getChildNodeList(const std::string& name) const{
		vector<XMLNode* > childList;
		for(unsigned int i=0;i<length();i++){
			std::vector<XMLNode*>& list = m_list[i]->children();
			for(unsigned int j=0;j<list.size();j++){
				if(list[j]->name() == name){
					childList.push_back(list[j]);
				}
			}			
		}
		return XMLNodeList(childList);
	}

	XMLNodeList XMLNodeList::get(const std::string& key) const {
		
		vector<string> keys;
		split(keys,trim_copy(key),is_any_of("."));
		
		XMLNodeList res(*this);
		for(unsigned int i=0;i<keys.size();i++) {
			res = res.getChildNodeList(keys[i]);
		}		
		return res;
	}
}

//string XMLNodeList::attrib(const std::string& name,unsigned int i) const{
//	assert(i>=0 && i < length());
//	return m_list[i]->attribs[name];
//}

//string& XMLNodeList::attrib(const std::string& name,unsigned int i){
//	assert(i>=0 && i < length());
//	return m_list[i]->attribs[name];
//}

//std::string XMLNodeList::text(unsigned int i) const {
//	assert(i>=0 && i < length());
//	return m_list[i]->text;
//}
//std::string& XMLNodeList::text(unsigned int i){
//	assert(i>=0 && i < length());
//	return m_list[i]->text;
//}

//std::vector<std::string> XMLNodeList::attribs(const std::string& name) const{
//	std::vector<std::string> res(length());
//	for(unsigned int i=0;i<length();i++){
//		res[i] = m_list[i]->attribs[name];
//	}
//	return res;
//}