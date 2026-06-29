#ifndef DOC_FACTORY_H
#define DOC_FACTORY_H

#include <string>

#include "../models/doc.h"

class DocFactory {
public:
	static Document createDocument(int id,
								   int userId,
								   const std::string& title,
								   const std::string& content);
};

inline Document DocFactory::createDocument(int id,
										   int userId,
										   const std::string& title,
										   const std::string& content)
{
	Document document{};
	document.id = id;
	document.user_id = userId;
	document.title = title;
	document.content_summary = content;
	return document;
}

#endif // DOC_FACTORY_H