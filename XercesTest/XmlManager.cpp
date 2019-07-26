#include "pch.h"
#include "XmlManager.h"

const std::string XmlManager::EMPTY_STRING = "";
const std::string XmlManager::NODENAME_PARENTTEMPLATE = "ParentTemplate";
const std::string XmlManager::NODENAME_TEMPLATENAME = "TemplateName";

XmlManager::XmlManager()
{
	try
	{
		XMLPlatformUtils::Initialize();
		m_xmlDocument	= nullptr;
		m_domParser = new XercesDOMParser();
		m_xsdParser = new XSDDOMParser();
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

	delete m_xsdParser;
	m_xsdParser = nullptr;

	delete m_domErrorHandler;
	m_domErrorHandler = nullptr;

	XMLPlatformUtils::Terminate();
}

std::string XmlManager::ConvertXMLCh(const XMLCh* xmlChString)
{
	char* pTmpChar = XMLString::transcode(xmlChString);
	std::string retString(pTmpChar);

	delete pTmpChar;
	return retString;
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


DOMNode* XmlManager::getFirstChildNamed(DOMNode* parentNode, std::string nodeName)
{
	DOMNode* retNode = nullptr;

	if (parentNode != nullptr)
	{
		DOMNode* curChild		= parentNode->getFirstChild();
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

		XMLString::release(&xmlChNodeName);
	}

	return retNode;
}

//std::optional<std::string> getXPath(DOMNode* parent, DOMNode* child)
//{
//	if (parent == nullptr || child == nullptr)
//		return std::nullopt;
//
//	if (child == parent)
//		return "";
//
//	DOMNode* childsParent = child->getParentNode();
//	if (childsParent == nullptr)
//		return std::nullopt;
//
//	auto retString = getXPath(parent, childsParent);
//	if (retString == std::nullopt)
//		return std::nullopt;
//	else if (retString == XmlManager::EMPTY_STRING)
//		retString = retString.value() + "/";
//
//	return retString;
//}

void XmlManager::printTree()
{
	int count = 0;

	std::cout << "######\n";

	treeAction(m_xmlDocument, [&count](DOMNode* node, int treeLevel) {
		if (node->getNodeType() == DOMNode::NodeType::ELEMENT_NODE)
		{
			for (int i = 0; i < treeLevel; i++)
				std::cout << " ";
			//char* xmlChNodeName = XMLString::transcode(node->getNodeName());
			std::string nodeName = XmlManager::ConvertXMLCh(((const XMLCh*)node->getNodeName()));
			//std::cout << xmlChNodeName << "\n";
			//delete xmlChNodeName;

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

void XmlManager::treeAction(DOMNode* rootNode, const std::function <void(DOMNode* node, int treeLevel)>& f)
{
	_treeAction(rootNode, 0, f);
}

void XmlManager::_treeAction(DOMNode* rootNode, int level, const std::function <void(DOMNode* node, int treeLevel)>& f)
{
	int textFound = false;
	DOMNode* curNode = rootNode;
	if (curNode == nullptr)
		curNode = getDocumentRoot();

	if (curNode == nullptr)
		return;

	XMLSize_t curChildCount = curNode->getChildNodes()->getLength();

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

DOMXPathResult* XmlManager::_executeXPathQuery(DOMNode* rootNode, std::string query, DOMXPathResult::ResultType resultType)
{
	if (rootNode == nullptr && m_xmlDocument != nullptr)
		rootNode = (DOMNode*)m_xmlDocument->getDocumentElement();

	DOMXPathResult* result = m_xmlDocument->evaluate(
		XMLString::transcode(query.c_str()),
		rootNode,
		NULL,
		resultType,
		NULL);

	return result;
}

std::vector<DOMNode*> XmlManager::executeXPathQuery(DOMNode* rootNode, std::string query)
{
	DOMXPathResult* result = _executeXPathQuery(rootNode, query, DOMXPathResult::ORDERED_NODE_SNAPSHOT_TYPE);
	DOMXPathResult::ResultType rt = result->getResultType();
	std::vector<DOMNode*> resultVector;

	if (rt == DOMXPathResult::ResultType::ORDERED_NODE_SNAPSHOT_TYPE)
	{
		XMLSize_t resultLength = result->getSnapshotLength();
		for (XMLSize_t i = 0; i < resultLength; i++)
		{
			result->snapshotItem(i);
			resultVector.push_back(result->getNodeValue());
			std::cout << XMLString::transcode(result->getNodeValue()->getNodeName()) << "\n";
		}
	}

	return resultVector;
}

DOMNode* XmlManager::executeXPathQuery_singleReturn(DOMNode* rootNode, std::string query)
{
	DOMXPathResult* result = _executeXPathQuery(rootNode, query, DOMXPathResult::ResultType::FIRST_ORDERED_NODE_TYPE);
	DOMXPathResult::ResultType rt = result->getResultType();

	if (rt == DOMXPathResult::ResultType::FIRST_ORDERED_NODE_TYPE)
	{
		std::cout << XMLString::transcode(result->getNodeValue()->getNodeName()) << "\n";
		return result->getNodeValue();
	}

	return nullptr;
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

//std::optional<std::string> XmlManager::getTemplateText(DOMNode* node, std::string templateName)
//{
//	if (node != nullptr)
//	{
//		DOMNode* curChildNode = node->getFirstChild();
//
//		while (curChildNode != nullptr)
//		{
//			std::string curChildNodeName = XmlManager::ConvertXMLCh( curChildNode->getNodeName() );
//		}
//	}
//}
//private String GetTemplateText(Node node, String template) {
//	if (node != null)
//	{
//		Node curChildNode = node.getFirstChild();
//
//		while (curChildNode != null)
//		{
//			String curChildNodeName = curChildNode.getNodeName();
//			if (curChildNodeName != null && curChildNodeName.equals(template))
//			{
//				return curChildNode.getTextContent();
//			}
//
//			curChildNode = curChildNode.getNextSibling();
//		}
//	}
//
//	return null;
//}


hmi_uint64 XmlManager::getXsdErrorCount()
{
	return m_domParser->getErrorCount();
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

	m_xsdParser->parse(filename.c_str());
	m_xsdDocument = m_xsdParser->getDocument();
	DOMNode* n = m_xsdDocument->getFirstChild();
	std::cout << "Root Children: " << n->getChildNodes()->getLength() << std::endl;

	DOMNode* WACnode = this->xsd_getFirstChildNamed(n, "WAC");
	DOMNode* SiteInfonode = this->xsd_getFirstChildNamed(WACnode, "SiteInfo");
	//DOMNode* c = n->getFirstChild()->getNextSibling();
	//DOMNode* attr = c->getAttributes()->getNamedItem(XMLString::transcode("name"));

	SiteInfonode->getAttributes()->getNamedItem(XmlString("name").getXmlChString()/*XMLString::transcode("name")*/);
	std::cout << "Name: " << this->ConvertXMLCh(SiteInfonode->getNodeName()) << std::endl;

	//if (attr)
	//	std::cout << "Name: " << this->ConvertXMLCh(attr->getTextContent()) << std::endl;

	//std::cout << "test\n";

	exit(0);
}


DOMNode* XmlManager::xsd_getFirstChildNamed(DOMNode* parentXsdNode, std::string nodeName)
{
	DOMNode* retNode = nullptr;

	if (parentXsdNode != nullptr)
	{
		DOMNode* curChild = parentXsdNode->getFirstChild();
		XmlString nodeName(nodeName);
		XmlString attrName("name");

		while (curChild != nullptr)
		{
			DOMNode* attrNode = curChild->getAttributes()->getNamedItem(attrName.getXmlChString());
			if (attrNode != nullptr)
			{
				const XMLCh* attrValue = attrNode->getTextContent();
				if (attrValue != nullptr && XMLString::equals(nodeName.getXmlChString(), attrValue))
				{
					retNode = curChild;
					break;
				}
			}
			curChild = curChild->getNextSibling();
		}
	}
	return retNode;
}

std::string XmlManager::xsd_getAttribute(DOMNode* xsdNode, std::string attrName)
{
	XMLCh* xmlChAttrName = XmlString(attrName).getXmlChString();
	auto x = xsdNode->getAttributes()->getNamedItem(xmlChAttrName);
	if (xsdNode->getAttributes()->getNamedItem(xmlChAttrName) != nullptr)
	{

	}
	return "";
}