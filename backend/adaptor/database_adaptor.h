#ifndef DATABASE_ADAPTOR_H
#define DATABASE_ADAPTOR_H

#include <functional>
#include <string>
#include <vector>
#include "../models/doc.h"

class DatabaseAdaptor {
public:
    virtual ~DatabaseAdaptor() = default;

    virtual void fetchDocumentsByUserId(
        int targetUserId,
        std::function<void(const std::vector<Document>&)> onSuccess,
        std::function<void(const std::string&)> onError) const = 0;

    virtual void fetchUserDetails(
        int targetUserId,
        std::function<void(int, const std::string&, const std::string&)> onSuccess,
        std::function<void(const std::string&)> onError) const = 0;

    virtual void fetchSpecificDocument(
        int docId,
        int userId,
        std::function<void(const Document&)> onSuccess,
        std::function<void(const std::string&)> onError) const = 0;

    virtual void saveUser(
        const std::string& username,
        const std::string& name,
        const std::string& passwordHash,
        const std::string& salt,
        const std::string& dateOfJoining,
        std::function<void(int userId)> onSuccess,
        std::function<void(const std::string&)> onError) const = 0;

    virtual void fetchUserCredentials(
        const std::string& username,
        std::function<void(int userId, const std::string& passwordHash, const std::string& salt, const std::string& name, const std::string& dateOfJoining)> onSuccess,
        std::function<void(const std::string&)> onError) const = 0;

    virtual void addDocument(
        int userId,
        const std::string& title,
        const std::string& content,
        std::function<void(int docId)> onSuccess,
        std::function<void(const std::string&)> onError) const = 0;

    virtual void deleteDocument(
        int docId,
        int userId,
        std::function<void()> onSuccess,
        std::function<void(const std::string&)> onError) const = 0;
};

#endif // DATABASE_ADAPTOR_H
