#ifndef SQL_ADAPTOR_H
#define SQL_ADAPTOR_H

#include <functional>
#include <string>
#include <vector>

#include "database_adaptor.h"
#include "../models/doc.h"

class SQL_adaptor : public DatabaseAdaptor {
public:
    SQL_adaptor() = default;
    ~SQL_adaptor() override = default;

    void fetchDocumentsByUserId(
        int targetUserId,
        std::function<void(const std::vector<Document>&)> onSuccess,
        std::function<void(const std::string&)> onError) const override;

    void fetchUserDetails(
        int targetUserId,
        std::function<void(int, const std::string&, const std::string&)> onSuccess,
        std::function<void(const std::string&)> onError) const override;

    void fetchSpecificDocument(
        int docId,
        int userId,
        std::function<void(const Document&)> onSuccess,
        std::function<void(const std::string&)> onError) const override;

    void saveUser(
        const std::string& username,
        const std::string& name,
        const std::string& passwordHash,
        const std::string& salt,
        const std::string& dateOfJoining,
        std::function<void(int userId)> onSuccess,
        std::function<void(const std::string&)> onError) const override;

    void fetchUserCredentials(
        const std::string& username,
        std::function<void(int userId, const std::string& passwordHash, const std::string& salt, const std::string& name, const std::string& dateOfJoining)> onSuccess,
        std::function<void(const std::string&)> onError) const override;

    void addDocument(
        int userId,
        const std::string& title,
        const std::string& content,
        std::function<void(int docId)> onSuccess,
        std::function<void(const std::string&)> onError) const override;

    void deleteDocument(
        int docId,
        int userId,
        std::function<void()> onSuccess,
        std::function<void(const std::string&)> onError) const override;
};

#endif // SQL_ADAPTOR_H
