#include "storage_manager.h"

std::shared_ptr<DatabaseAdaptor> StorageManager::activeAdaptor = nullptr;
std::string StorageManager::dbRdbms = "mysql"; 

void StorageManager::setAdaptor(std::shared_ptr<DatabaseAdaptor> adaptor) {
    activeAdaptor = adaptor;
}

std::shared_ptr<DatabaseAdaptor> StorageManager::getAdaptor() {
    return activeAdaptor;
}

void StorageManager::setDbRdbms(const std::string& rdbms) {
    dbRdbms = rdbms;
}

std::string StorageManager::getDbRdbms() {
    return dbRdbms;
}
