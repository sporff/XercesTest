#pragma once

using namespace xercesc;

class XmlManager
{
public:
	inline static const std::string NODENAME_PARENTTEMPLATE = "ParentTemplate";
	inline static const std::string NODENAME_TEMPLATENAME = "TemplateName";

	XmlManager();
	~XmlManager();

	// XML file
	bool loadXmlFile(std::string filename);
	void closeXmlFile();

	// Document helpers
	DOMNode* getDocumentRoot();
	DOMNode* getFirstNamedChild(std::string nodeName);
	
	
	/*std::string getXPath(DOMNode* parent, DOMNode* child) {
		if (child == parent)
			return "";

		DOMNode* childParent = child->getParentNode();
		if (childParent == nullptr)
			return nullptr;

		std::string retString = getXPath(parent, childParent);
		if (retString == nullptr)
			return nullptr;
		if (retString != "")
			retString = retString + "/";

		return retString + child->getNodeName();
	}*/

	// Inheritance helpers
	DOMNode* GetDirectRoot(DOMNode* node);
	DOMNode* GetTemplateRoot(DOMNode* node);
	DOMNode* GetRoot(DOMNode* node, std::string childName);

	// Utils
	void treeAction(DOMNode* rootNode, const std::function <void(DOMNode* node, int treeLevel)>& f);
	void printTree();

	// XSD Schema
	void loadXsdFile(std::string filename);
	int getXsdErrorCount();

private:
	XercesDOMParser		*m_domParser;
	DOMDocument			*m_xmlDocument;
	XmlParserErrorHandler *m_domErrorHandler;

	void _treeAction(DOMNode* rootNode, int level, const std::function <void(DOMNode* node, int treeLevel)>& f);
};

// xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespaceSchemaLocation="wacSmall.xsd"