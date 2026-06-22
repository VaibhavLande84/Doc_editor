#ifndef DOC_H
#define DOC_H

#include <drogon/drogon.h>
#include <functional>
#include <string>

struct Document {
    int id;
    int user_id;
    std::string title;
    std::string content_summary;
};

void getSpecificDocument(int doc_ID,
                         int user_ID,
                         std::function<void(const Document &)> onSuccess,
                         std::function<void(const std::string &)> onError);

#endif