

#ifndef MONGODBADAPTOR_H
#define MONGODBADAPTOR_H

#include <functional>
#include <string>
#include <vector>

#include "../models/doc.h"

class MongoDB_adaptor {
public:
    void fetchDocumentsByUserId(
        int targetUserId,
        std::function<void(const std::vector<Document>&)> onSuccess,
        std::function<void(const std::string&)> onError) const;

    void fetchUserDetails(
        int targetUserId,
        std::function<void(int, const std::string&, const std::string&)> onSuccess,
        std::function<void(const std::string&)> onError) const;

    void fetchSpecificDocument(
        int docId,
        int userId,
        std::function<void(const Document&)> onSuccess,
        std::function<void(const std::string&)> onError) const;
};

#endif // MONGODBADAPTOR_H