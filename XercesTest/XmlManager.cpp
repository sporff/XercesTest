#include "pch.h"
#include "XmlManager.h"

using namespace xercesc;

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
	
	/*m_domConfig->setParameter(XMLUni::fgDOMElementContentWhitespace, false);
	m_domConfig->setParameter(XMLUni::fgXercesSchema, true);
	m_domConfig->setParameter(XMLUni::fgXercesSchemaFullChecking, true);
	m_domConfig->setParameter(XMLUni::fgDOMNamespaces, true);
	m_domConfig->setParameter(XMLUni::fgDOMDisallowDoctype, false);
	m_domConfig->setParameter(XMLUni::fgXercesHandleMultipleImports, true);*/
	//m_domParser->setValidationScheme(XercesDOMParser::Val_Auto);
	//m_domParser->setDoNamespaces(true);
	//m_domParser->setDoSchema(true);
	////m_domParser->setLoadExternalDTD(false);
	//m_domParser->setIncludeIgnorableWhitespace(true);
	//m_domParser->setValidationConstraintFatal(true);

	//m_domParser.set1.setDoNamespaces(tr ue);
	//m_domParser.setDoSchema(true);
	//m_domParser.setValidationSchemaFullChecking(true);
	//m_domConfig->setParameter(XMLUni::fgDOMWRTWhitespaceInElementContent, false);
}


XmlManager::~XmlManager()
{
	delete m_domParser;
	m_domParser = nullptr;

	delete m_domErrorHandler;
	m_domErrorHandler = nullptr;

	XMLPlatformUtils::Terminate();
}


void XmlManager::loadXsdFile(std::string filename)
{
	//m_domParser->setErrorHandler(&errorHandler);
	//DOMErrorHandler* errHandler = new DOMErrorHandler();
	//m_domParser->setErrorHandler(errHandler);
	
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


bool XmlManager::loadXmlFile(std::string filename)
{
	//m_xmlDocument = m_domParser->parseURI( filename.c_str() );
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

	if (docRoot != nullptr)
	{
		DOMNode* curChild		= docRoot->getFirstChild();
		XMLCh* xmlchNodeName	= XMLString::transcode(nodeName.c_str());	// convert input string to XMLCh

		while (curChild != nullptr)
		{
			if (XMLString::equals(xmlchNodeName, curChild->getNodeName()))
			{
				return curChild;
			}
			curChild = curChild->getNextSibling();
		}
	}

	return nullptr;
}


void XmlManager::treeAction(DOMNode* rootNode, const std::function <void(DOMNode* node, int treeLevel)>& f)
{
	int level = 0;

	_treeAction(rootNode, level, f);
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
			std::cout << XMLString::transcode(node->getNodeName()) << "\n";
			count++;
		}
		else if (node->getNodeType() == DOMNode::NodeType::TEXT_NODE)
		{
			for (int i = 0; i < treeLevel; i++)
				std::cout << " ";
			std::cout << "[" << XMLString::transcode(node->getNodeValue()) << "]" << std::endl;
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

	int curChildCount = rootNode->getChildNodes()->getLength();

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

int XmlManager::getXsdErrorCount()
{
	int errorCount = m_domParser->getErrorCount();
	auto eh = m_domParser->getErrorHandler();
	return errorCount;
}