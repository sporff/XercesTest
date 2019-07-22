#pragma once
#include "xercesc/sax/ErrorHandler.hpp"

class XmlParserErrorHandler : public ErrorHandler
{
public:
	XmlParserErrorHandler();
	~XmlParserErrorHandler();

private:
private:
	void reportParseException(const SAXParseException& ex)
	{
		char* msg = XMLString::transcode(ex.getMessage());

		//fprintf(stderr, "at line %llu column %llu, %s\n", ex.getLineNumber(), ex.getColumnNumber(), msg);
		std::cout << "at line " << ex.getLineNumber() << " column " << ex.getColumnNumber() << ", " << msg << "\n";
		
		XMLString::release(&msg);
	}

public:
	void warning(const SAXParseException& ex)
	{
		reportParseException(ex);
	}

	void error(const SAXParseException& ex)
	{
		reportParseException(ex);
	}

	void fatalError(const SAXParseException& ex)
	{
		reportParseException(ex);
	}

	void resetErrors()
	{
		std::cout << "Reset\n";
	}
};

