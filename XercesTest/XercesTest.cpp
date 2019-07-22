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

	//xmlManager.printTree();
	std::cout << "Error count: " << xmlManager.getXsdErrorCount() << "\n";

    std::cout << " "; 
}
