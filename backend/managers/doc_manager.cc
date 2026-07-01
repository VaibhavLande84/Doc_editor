#include "doc_manager.h"
#include "storage_manager.h"

void DocManager::addDocument(
    int userId,
    const std::string& title,
    const std::string& content,
    std::function<void(int)> onSuccess,
    std::function<void(const std::string&)> onError)
{
    StorageManager::getAdaptor()->addDocument(userId, title, content, std::move(onSuccess), std::move(onError));
}

void DocManager::deleteDocument(
    int docId,
    int userId,
    std::function<void()> onSuccess,
    std::function<void(const std::string&)> onError)
{
    StorageManager::getAdaptor()->deleteDocument(docId, userId, std::move(onSuccess), std::move(onError));
}

void DocManager::getDocument(
    int docId,
    int userId,
    std::function<void(const Document&)> onSuccess,
    std::function<void(const std::string&)> onError)
{
    StorageManager::getAdaptor()->fetchSpecificDocument(docId, userId, std::move(onSuccess), std::move(onError));
}

void DocManager::getDocumentsList(
    int userId,
    std::function<void(const std::vector<Document>&)> onSuccess,
    std::function<void(const std::string&)> onError)
{
    StorageManager::getAdaptor()->fetchDocumentsByUserId(userId, std::move(onSuccess), std::move(onError));
}

bool DocManager::authenticateRequest(int userId, const std::string& sessionUserId) {
    if (sessionUserId.empty()) return false;
    try {
        return std::stoi(sessionUserId) == userId;
    } catch (...) {
        return false;
    }
}
