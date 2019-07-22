// XercesTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>

#include "XmlManager.h"

using namespace xercesc;

int main()
{
	XmlManager xmlManager;

	xmlManager.loadXsdFile("C:/xml/wacSmall.xsd");
	xmlManager.loadXmlFile("C:/xml/wacSmall.xml");

	DOMNode* docRoot = xmlManager.getDocumentRoot();
	DOMNode* wuisNode = xmlManager.getFirstNamedChild("WUIs");

	DOMNode* aisleControllersNode = xmlManager.getFirstNamedChild("AisleControllers");
	/*DOMNode* wuisNode = xmlManager.getFirstNamedChild("WUIs");

	DOMNodeList* nodeList = wuisNode->getChildNodes();
	for (int i = 0; i < nodeList->getLength(); i++)
	{
		if (nodeList->item(i)->getNodeType() == DOMNode::NodeType::ELEMENT_NODE) {
			std::cout << XMLString::transcode(nodeList->item(i)->getNodeName()) << std::endl;

			DOMNodeList* childNodeList = nodeList->item(i)->getChildNodes();
			for (int i = 0; i < childNodeList->getLength(); i++)
			{
				if (childNodeList->item(i)->getNodeType() == DOMNode::NodeType::ELEMENT_NODE)
					std::cout << "    " << XMLString::transcode(childNodeList->item(i)->getNodeName()) << std::endl;
			}
		}
	}*/

	/*nodeList = aisleControllersNode->getChildNodes();
	for (int i = 0; i < nodeList->getLength(); i++)
	{
		if (nodeList->item(i)->getNodeType() == DOMNode::NodeType::ELEMENT_NODE)
			std::cout << XMLString::transcode(nodeList->item(i)->getNodeName()) << std::endl;
	}*/

	//xmlManager.printTree();

	std::cout << "Error count: " << xmlManager.getXsdErrorCount() << "\n";

    std::cout << " "; 
}
