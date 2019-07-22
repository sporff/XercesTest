// XercesTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

using namespace xercesc;

int main()
{
	XmlManager xmlManager;

	/*xmlManager.loadXsdFile("C:/xml/wacSmall.xsd");
	xmlManager.loadXmlFile("C:/xml/wacSmall.xml");*/
	xmlManager.loadXsdFile("C:/OPEX/Cortex/CortexHMI/resources/documents/Master.xsd");
	xmlManager.loadXmlFile("C:/Users/dgibson/wkspaces/CortexClientSpecific-OPEX-Workstation7/config/xml/WS07_Show_Combined_localTesting.XML");

	DOMNode* docRoot = xmlManager.getDocumentRoot();
	DOMNode* wuisNode = xmlManager.getFirstChildNamed("WUIs");

	DOMNode* aisleControllersNode = xmlManager.getFirstChildNamed("AisleControllers");

	//xmlManager.printTree();
	std::cout << "Error count: " << xmlManager.getXsdErrorCount() << "\n";

	xmlManager.executeXPathQuery(nullptr, "//WACFile/WUIs/WUI");
	xmlManager.executeXPathQuery_singleReturn(nullptr, "//WACFile/WUIs/WUI");

    std::cout << " "; 
}
