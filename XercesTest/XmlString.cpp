#include "pch.h"
#include "XmlString.h"


XmlString::XmlString()
	: m_stdString("")
	, m_xmlChString(nullptr)
{
}

XmlString::XmlString(std::string str)
{
	setString(str);
}

XmlString::XmlString(XMLCh* str)
{
	setString(str);
}


XmlString::~XmlString()
{
	reset();
}

void XmlString::reset()
{
	if (m_xmlChString != nullptr)
		XMLString::release(&m_xmlChString);
	
	m_stdString = "";
}


void XmlString::setString(std::string str)
{
	reset();

	m_stdString = str;
	m_xmlChString = XMLString::transcode(str.c_str());
}
void XmlString::setString(XMLCh* str)
{
	reset();

	char* transcodeStr = XMLString::transcode(str);
	m_stdString = std::string(transcodeStr);
	delete transcodeStr;

	XMLString::copyString(m_xmlChString, str);	// don't reference the string passed in
}

std::string XmlString::getStdString()
{
	return m_stdString;
}
XMLCh* XmlString::getXmlChString()
{
	return m_xmlChString;
}