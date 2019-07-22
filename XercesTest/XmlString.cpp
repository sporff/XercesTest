#include "pch.h"
#include "XmlString.h"


XmlString::XmlString()
	: m_stdString("")
	, m_xmlChString(nullptr)
{
}


XmlString::~XmlString()
{
	reset();
}

void XmlString::reset()
{
	if (m_xmlChString != nullptr)
		delete m_xmlChString;
	m_stdString = "";
}


void XmlString::setString(std::string str)
{
	m_stdString = str;
	m_xmlChString = XMLString::transcode(str.c_str());
}
void XmlString::setString(XMLCh* str)
{
	char* transcodeStr = XMLString::transcode(str);
	m_stdString = std::string(transcodeStr);

	delete transcodeStr;
}

std::string XmlString::getStdString()
{
	return "";
}
std::string XmlString::getXmlChString()
{
	return "";
}