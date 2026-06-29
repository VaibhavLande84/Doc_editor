#include "sync_manager.h"
#include "doc_manager.h"
#include "storage_manager.h"
#include <drogon/drogon.h>
#include <iostream>

SyncManager& SyncManager::instance() {
    static SyncManager inst;
    return inst;
}

void SyncManager::initialize() {
    std::cout << "SyncManager initialized successfully." << std::endl;
}

void SyncManager::updateDocumentContent(int docId, const std::string& newContent) {
    std::lock_guard<std::mutex> lock(mutex_);
    dirtyDocuments_[docId] = newContent;
}

void SyncManager::commitToStorage(int docId, int userId) {
    std::string content;
    {
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = dirtyDocuments_.find(docId);
        if (it != dirtyDocuments_.end()) {
            content = it->second;
            dirtyDocuments_.erase(it);
        } else {
            return; 
        }
    }

    std::cout << "SyncManager: Committing document " << docId << " for user " << userId 
              << " to storage. Content length: " << content.length() << " bytes." << std::endl;

    auto dbClient = drogon::app().getDbClient();
    std::string summary = content.substr(0, 100);
    std::string sql = "UPDATE documents SET content = $1, content_summary = $2 WHERE id = $3 AND user_id = $4;";
    if (StorageManager::getDbRdbms() == "mysql") {
        sql = "UPDATE documents SET content = ?, content_summary = ? WHERE id = ? AND user_id = ?;";
    }

    dbClient->execSqlAsync(
        sql,
        [docId](const drogon::orm::Result& result) {
            std::cout << "Successfully saved document " << docId << " to SQL database via SyncManager." << std::endl;
        },
        [docId](const drogon::orm::DrogonDbException& e) {
            std::cerr << "Failed to save document " << docId << " to database: " << e.base().what() << std::endl;
        },
        content, summary, docId, userId
    );
}

std::string SyncManager::getCachedContent(int docId) {
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = dirtyDocuments_.find(docId);
    if (it != dirtyDocuments_.end()) {
        return it->second;
    }
    return "";
}
