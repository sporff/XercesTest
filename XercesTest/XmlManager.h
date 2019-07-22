#pragma once

using namespace xercesc;

class XmlManager
{
public:
	XmlManager();
	~XmlManager();

	bool loadXmlFile(std::string filename);
	void closeXmlFile();

	DOMNode* getDocumentRoot();
	DOMNode* getFirstNamedChild(std::string nodeName);

	void treeAction(DOMNode* rootNode, const std::function <void(DOMNode* node, int treeLevel)>& f);
	void printTree();

	void loadXsdFile(std::string filename);
	int getXsdErrorCount();

private:
	XercesDOMParser		*m_domParser;
	DOMDocument			*m_xmlDocument;
	XmlParserErrorHandler *m_domErrorHandler;

	void _treeAction(DOMNode* rootNode, int level, const std::function <void(DOMNode* node, int treeLevel)>& f);
};

// xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespaceSchemaLocation="wacSmall.xsd"