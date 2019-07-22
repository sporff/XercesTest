#include "pch.h"
#include "XmlManager.h"

// test commit

XmlManager::XmlManager()
{
	try
	{
		XMLPlatformUtils::Initialize();
		m_xmlDocument	= nullptr;
		m_domParser = new XercesDOMParser();
		m_domErrorHandler = new XmlParserErrorHandler();
	}
	catch (...)
	{
		std::cout << "Error initializing XML library.";
	}
}


XmlManager::~XmlManager()
{
	delete m_domParser;
	m_domParser = nullptr;

	delete m_domErrorHandler;
	m_domErrorHandler = nullptr;

	XMLPlatformUtils::Terminate();
}


bool XmlManager::loadXmlFile(std::string filename)
{
	m_domParser->parse(filename.c_str());
	m_xmlDocument = m_domParser->getDocument();
	return false;
}


void XmlManager::closeXmlFile()
{
	m_domParser->resetDocumentPool();
}

DOMNode* XmlManager::getDocumentRoot()
{
	if (m_xmlDocument != nullptr)
		return m_xmlDocument->getFirstChild();

	return nullptr;
}


DOMNode* XmlManager::getFirstNamedChild(std::string nodeName)
{
	DOMNode* docRoot = getDocumentRoot();
	DOMNode* retNode = nullptr;

	if (docRoot != nullptr)
	{
		DOMNode* curChild		= docRoot->getFirstChild();
		XMLCh* xmlChNodeName	= XMLString::transcode(nodeName.c_str());	// convert input string to XMLCh

		while (curChild != nullptr)
		{
			if (XMLString::equals(xmlChNodeName, curChild->getNodeName()))
			{
				retNode = curChild;
				break;
			}
			curChild = curChild->getNextSibling();
		}

		delete xmlChNodeName;
	}

	return retNode;
}


void XmlManager::treeAction(DOMNode* rootNode, const std::function <void(DOMNode* node, int treeLevel)>& f)
{
	_treeAction(rootNode, 0, f);
}


void XmlManager::printTree()
{
	int count = 0;

	std::cout << "######\n";

	treeAction(m_xmlDocument, [&count](DOMNode* node, int treeLevel) {
		if (node->getNodeType() == DOMNode::NodeType::ELEMENT_NODE)
		{
			for (int i = 0; i < treeLevel; i++)
				std::cout << " ";
			char* xmlChNodeName = XMLString::transcode(node->getNodeName());
			std::cout << xmlChNodeName << "\n";
			delete xmlChNodeName;

			count++;
		}
		else if (node->getNodeType() == DOMNode::NodeType::TEXT_NODE)
		{
			for (int i = 0; i < treeLevel; i++)
				std::cout << " ";
			char* xmlChNodeName = XMLString::transcode(node->getNodeName());
			std::cout << "[" << xmlChNodeName << "]" << std::endl;
			delete xmlChNodeName;

			count++;
		}
	});

	std::cout << "###### Node count: " << count << " ## \n";
}


void XmlManager::_treeAction(DOMNode* rootNode, int level, const std::function <void(DOMNode* node, int treeLevel)>& f)
{
	int textFound = false;
	DOMNode* curNode = rootNode;
	if (curNode == nullptr)
		curNode = getDocumentRoot();

	if (curNode == nullptr)
		return;

	int curChildCount = curNode->getChildNodes()->getLength();

	f(curNode, level);

	DOMNode* curChild = curNode->getFirstChild();
	level++;
	while (curChild != nullptr)
	{
		if (curChild->getNodeType() == DOMNode::NodeType::ELEMENT_NODE)
			_treeAction(curChild, level, f);

		if (curChild->getNodeType() == DOMNode::NodeType::TEXT_NODE && curChildCount == 1)
		{
			textFound = true;
			_treeAction(curChild, level, f);
		}
		//else
			//std::cout << "ignored whitespace\n";
		curChild = curChild->getNextSibling();
	}
}


DOMNode* XmlManager::GetDirectRoot(DOMNode* node)
{
	return GetRoot(node, NODENAME_PARENTTEMPLATE);
}
DOMNode* XmlManager::GetTemplateRoot(DOMNode* node)
{
	return GetRoot(node, NODENAME_TEMPLATENAME);
}

DOMNode* XmlManager::GetRoot(DOMNode* node, std::string childName) {
	if (node != nullptr) {
		DOMNode* currentNode = node;

		while (currentNode != nullptr) {
			DOMNode* currentChild = currentNode->getFirstChild();

			while (currentChild != nullptr)
			{
				char* xmlChNodeName = XMLString::transcode(currentChild->getNodeName());
				std::string currentChildName = xmlChNodeName;
				delete xmlChNodeName;
				if (currentChildName == childName) {
					return currentNode;
				}
				currentChild = currentChild->getNextSibling();
			}
			currentNode = currentNode->getParentNode();
		}
	}
	return nullptr;
}

int XmlManager::getXsdErrorCount()
{
	int errorCount = m_domParser->getErrorCount();
	auto eh = m_domParser->getErrorHandler();
	return errorCount;
}

void XmlManager::loadXsdFile(std::string filename)
{
	if (m_domParser->loadGrammar(filename.c_str(), Grammar::SchemaGrammarType) == nullptr)
		std::cout << "Couldn't load schema" << std::endl;
	else {
		std::cout << "Schema loaded!\n";
	}
	m_domParser->setErrorHandler(m_domErrorHandler);
	m_domParser->setValidationScheme(XercesDOMParser::Val_Auto);
	m_domParser->setDoNamespaces(true);
	m_domParser->setDoSchema(true);
	m_domParser->setIncludeIgnorableWhitespace(false);
	m_domParser->setValidationConstraintFatal(false);
	m_domParser->setHandleMultipleImports(true);
	m_domParser->setValidationSchemaFullChecking(true);
	m_domParser->setExternalNoNamespaceSchemaLocation(filename.c_str());
}