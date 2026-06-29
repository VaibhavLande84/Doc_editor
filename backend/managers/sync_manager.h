#ifndef SYNC_MANAGER_H
#define SYNC_MANAGER_H

#include <string>
#include <mutex>
#include <unordered_map>

class SyncManager {
private:
    std::mutex mutex_;
    std::unordered_map<int, std::string> dirtyDocuments_;
    
    SyncManager() = default;

public:
    static SyncManager& instance();

    void initialize();
    void updateDocumentContent(int docId, const std::string& newContent);
    void commitToStorage(int docId, int userId);
    std::string getCachedContent(int docId);
};

#endif // SYNC_MANAGER_H
