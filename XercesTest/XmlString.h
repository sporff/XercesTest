#pragma once
class XmlString
{
	std::string m_stdString;
	XMLCh* m_xmlChString;
public:
	XmlString();
	XmlString(std::string str);
	XmlString(XMLCh* str);
	~XmlString();

	void reset();

	void setString(std::string str);
	void setString(XMLCh* str);

	std::string getStdString();
	XMLCh* getXmlChString();
};

