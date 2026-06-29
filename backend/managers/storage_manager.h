#ifndef STORAGE_MANAGER_H
#define STORAGE_MANAGER_H

#include <memory>
#include <string>
#include "../adaptor/database_adaptor.h"

class StorageManager {
private:
    static std::shared_ptr<DatabaseAdaptor> activeAdaptor;
    static std::string dbRdbms;
public:
    static void setAdaptor(std::shared_ptr<DatabaseAdaptor> adaptor);
    static std::shared_ptr<DatabaseAdaptor> getAdaptor();
    static void setDbRdbms(const std::string& rdbms);
    static std::string getDbRdbms();
};

#endif // STORAGE_MANAGER_H