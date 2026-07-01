#ifndef DOC_MANAGER_H
#define DOC_MANAGER_H

#include <string>
#include <functional>
#include <vector>
#include "../models/doc.h"

class DocManager {
public:
    static void addDocument(
        int userId,
        const std::string& title,
        const std::string& content,
        std::function<void(int docId)> onSuccess,
        std::function<void(const std::string&)> onError);

    static void deleteDocument(
        int docId,
        int userId,
        std::function<void()> onSuccess,
        std::function<void(const std::string&)> onError);

    static void getDocument(
        int docId,
        int userId,
        std::function<void(const Document&)> onSuccess,
        std::function<void(const std::string&)> onError);

    static void getDocumentsList(
        int userId,
        std::function<void(const std::vector<Document>&)> onSuccess,
        std::function<void(const std::string&)> onError);

    static bool authenticateRequest(int userId, const std::string& sessionUserId);
};

#endif // DOC_MANAGER_H