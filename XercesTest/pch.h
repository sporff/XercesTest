// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

#ifndef PCH_H
#define PCH_H

// TODO: add headers that you want to pre-compile here
#include <string>
#include <iostream>
#include <functional>
#include <vector>
#include <cstdint>
#include <optional>

#include <xercesc/util/PlatformUtils.hpp>
#include "xercesc/parsers/AbstractDOMParser.hpp"
#include "xercesc/dom/DOMImplementation.hpp"
#include "xercesc/dom/DOMImplementationLS.hpp"
#include "xercesc/dom/DOMImplementationRegistry.hpp"
#include "xercesc/dom/DOMLSParser.hpp"
#include "xercesc/dom/DOMException.hpp"
#include "xercesc/dom/DOMDocument.hpp"
#include "xercesc/dom/DOMNodeList.hpp"
#include "xercesc/dom/DOMError.hpp"
#include "xercesc/dom/DOMLocator.hpp"
#include "xercesc/dom/DOMNamedNodeMap.hpp"
#include "xercesc/dom/DOMAttr.hpp"
#include "xercesc/sax/ErrorHandler.hpp"
#include "xercesc/parsers/XercesDOMParser.hpp"
#include <xercesc/sax/SAXParseException.hpp>

using namespace xercesc;
typedef uint64_t hmi_uint64;
#include "XmlParserErrorHandler.h"
#include "XmlManager.h"

#endif //PCH_H
