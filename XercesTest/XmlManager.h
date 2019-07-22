#pragma once

using namespace xercesc;

class XmlManager
{
public:
	inline static const std::string EMPTY_STRING = "";
	inline static const std::string NODENAME_PARENTTEMPLATE = "ParentTemplate";
	inline static const std::string NODENAME_TEMPLATENAME = "TemplateName";

	XmlManager();
	~XmlManager();

	// General utils
	static std::string ConvertXMLCh(const XMLCh* xmlChString);

	// XML file operations
	bool loadXmlFile(std::string filename);
	//bool saveXmlFile();
	void closeXmlFile();

	// XML Document
	DOMNode* getDocumentRoot();
	DOMNode* getFirstChildNamed(std::string nodeName);
	std::string getXPath(DOMNode* parent, DOMNode* child);
	std::optional<std::string> getTemplateText(DOMNode* node, std::string templateName);

	// XML XPath
	std::vector<DOMNode*> executeXPathQuery(DOMNode* rootNode, std::string query);
	DOMNode* executeXPathQuery_singleReturn(DOMNode* rootNode, std::string query);
	
	// XML Inheritance helpers
	DOMNode* GetDirectRoot(DOMNode* node);
	DOMNode* GetTemplateRoot(DOMNode* node);
	DOMNode* GetRoot(DOMNode* node, std::string childName);

	// XML Debug
	void treeAction(DOMNode* rootNode, const std::function <void(DOMNode* node, int treeLevel)>& f);
	void printTree();

	// XSD Schema
	void loadXsdFile(std::string filename);
	hmi_uint64 getXsdErrorCount();

private:
	XercesDOMParser		*m_domParser;
	DOMDocument			*m_xmlDocument;
	XmlParserErrorHandler *m_domErrorHandler;

	void _treeAction(DOMNode* rootNode, int level, const std::function <void(DOMNode* node, int treeLevel)>& f);
	DOMXPathResult* _executeXPathQuery(DOMNode* rootNode, std::string query, DOMXPathResult::ResultType resultType);
};

// xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespaceSchemaLocation="wacSmall.xsd"