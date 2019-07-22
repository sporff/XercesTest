#pragma once
class XmlString
{
	std::string m_stdString;
	XMLCh* m_xmlChString;
public:
	XmlString();
	~XmlString();

	void reset();

	void setString(std::string str);
	void setString(XMLCh* str);

	std::string getStdString();
	std::string getXmlChString();
};

