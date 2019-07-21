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

	void tempSetup();

private:
	DOMImplementation	*m_domImpl;
	XercesDOMParser			*m_domParser;
	DOMConfiguration	*m_domConfig;
	DOMDocument			*m_xmlDocument;

	void _treeAction(DOMNode* rootNode, int level, const std::function <void(DOMNode* node, int treeLevel)>& f);
};

