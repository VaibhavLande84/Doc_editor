#include "mongoDBadaptor.h"

#include <drogon/drogon.h>

void MongoDB_adaptor::fetchDocumentsByUserId(
    int targetUserId,
    std::function<void(const std::vector<Document>&)> onSuccess,
    std::function<void(const std::string&)> onError) const
{
    auto dbClient = drogon::app().getDbClient();

    dbClient->execSqlAsync(
        "SELECT id, user_id, title, content_summary FROM documents WHERE user_id = $1;",
        [],
        [onSuccess](const drogon::orm::Result& result) {
            std::vector<Document> userDocs;

            for (const auto& row : result) {
                Document doc;
                doc.id = row["id"].as<int>();
                doc.user_id = row["user_id"].as<int>();
                doc.title = row["title"].as<std::string>();
                doc.content_summary = row["content_summary"].as<std::string>();
                userDocs.push_back(doc);
            }

            onSuccess(userDocs);
        },
        [onError](const drogon::orm::DrogonDbException& e) {
            onError(e.base().what());
        }
    );
}

void MongoDB_adaptor::fetchUserDetails(
    int targetUserId,
    std::function<void(int, const std::string&, const std::string&)> onSuccess,
    std::function<void(const std::string&)> onError) const
{
    auto dbClient = drogon::app().getDbClient();

    dbClient->execSqlAsync(
        "SELECT id, name, date_of_joining FROM users WHERE id = $1;",
        [targetUserId],
        [onSuccess, onError, targetUserId](const drogon::orm::Result& result) {
            if (result.empty()) {
                onError("User with ID " + std::to_string(targetUserId) + " not found in database.");
                return;
            }

            const auto& row = result[0];
            onSuccess(
                row["id"].as<int>(),
                row["name"].as<std::string>(),
                row["date_of_joining"].as<std::string>());
        },
        [onError](const drogon::orm::DrogonDbException& e) {
            onError(e.base().what());
        }
    );
}

void MongoDB_adaptor::fetchSpecificDocument(
    int docId,
    int userId,
    std::function<void(const Document&)> onSuccess,
    std::function<void(const std::string&)> onError) const
{
    auto dbClient = drogon::app().getDbClient();

    dbClient->execSqlAsync(
        "SELECT id, user_id, title, content FROM documents WHERE id = $1;",
        [],
        [onSuccess, onError, userId](const drogon::orm::Result& result) {
            if (result.empty()) {
                onError("Error: Document not found.");
                return;
            }

            const auto& row = result[0];
            const int dbUserId = row["user_id"].as<int>();

            if (dbUserId != userId) {
                onError("Access Denied: You do not have permission to view this document.");
                return;
            }

            Document doc;
            doc.id = row["id"].as<int>();
            doc.user_id = dbUserId;
            doc.title = row["title"].as<std::string>();
            doc.content_summary = row["content"].as<std::string>();
            onSuccess(doc);
        },
        [onError](const drogon::orm::DrogonDbException& e) {
            onError(e.base().what());
        }
    );
}
