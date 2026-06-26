#include "doc.h"
#include <iostream>
#include <utility>

#include "../managers/storage_manager.h"

void getSpecificDocument(int doc_ID, int User_ID,
                         std::function<void(const Document&)> onSuccess,
                         std::function<void(const std::string&)> onError)
{
    StorageManager::getAdaptor()->fetchSpecificDocument(doc_ID, User_ID, std::move(onSuccess), std::move(onError));
}