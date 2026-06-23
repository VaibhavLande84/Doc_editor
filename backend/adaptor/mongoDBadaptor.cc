#include "mongoDBadaptor.h"
#include <drogon/drogon.h>
#include <iostream>
#include <regex>
#include "../managers/storage_manager.h"

static std::string formatQuery(const std::string& query) {
    std::string rdbms = StorageManager::getDbRdbms();
    if (rdbms == "mysql") {
        std::regex e("\\$[0-9]+");
        return std::regex_replace(query, e, "?");
    }
    return query;
}

void MongoDB_adaptor::fetchDocumentsByUserId(
    int targetUserId,
    std::function<void(const std::vector<Document>&)> onSuccess,
    std::function<void(const std::string&)> onError) const
{
    auto dbClient = drogon::app().getDbClient();
    std::string sql = formatQuery("SELECT id, user_id, title, content_summary FROM documents WHERE user_id = $1;");

    dbClient->execSqlAsync(
        sql,
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
        },
        targetUserId
    );
}

void MongoDB_adaptor::fetchUserDetails(
    int targetUserId,
    std::function<void(int, const std::string&, const std::string&)> onSuccess,
    std::function<void(const std::string&)> onError) const
{
    auto dbClient = drogon::app().getDbClient();
    std::string sql = formatQuery("SELECT id, name, date_of_joining FROM users WHERE id = $1;");

    dbClient->execSqlAsync(
        sql,
        [onSuccess, onError, targetUserId](const drogon::orm::Result& result) {
            if (result.empty()) {
                onError("User with ID " + std::to_string(targetUserId) + " not found.");
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
        },
        targetUserId
    );
}

void MongoDB_adaptor::fetchSpecificDocument(
    int docId,
    int userId,
    std::function<void(const Document&)> onSuccess,
    std::function<void(const std::string&)> onError) const
{
    auto dbClient = drogon::app().getDbClient();
    std::string sql = formatQuery("SELECT id, user_id, title, content FROM documents WHERE id = $1;");

    dbClient->execSqlAsync(
        sql,
        [onSuccess, onError, userId](const drogon::orm::Result& result) {
            if (result.empty()) {
                onError("Error: Document not found.");
                return;
            }
            const auto& row = result[0];
            const int dbUserId = row["user_id"].as<int>();
            Document doc;
            doc.id = row["id"].as<int>();
            doc.user_id = dbUserId;
            doc.title = row["title"].as<std::string>();
            doc.content_summary = row["content"].as<std::string>();
            onSuccess(doc);
        },
        [onError](const drogon::orm::DrogonDbException& e) {
            onError(e.base().what());
        },
        docId
    );
}

void MongoDB_adaptor::saveUser(
    const std::string& username,
    const std::string& name,
    const std::string& passwordHash,
    const std::string& salt,
    const std::string& dateOfJoining,
    std::function<void(int userId)> onSuccess,
    std::function<void(const std::string&)> onError) const
{
    auto dbClient = drogon::app().getDbClient();
    std::string sql = formatQuery("INSERT INTO users (username, name, password_hash, salt, date_of_joining) VALUES ($1, $2, $3, $4, $5);");

    dbClient->execSqlAsync(
        sql,
        [onSuccess](const drogon::orm::Result& result) {
            onSuccess(static_cast<int>(result.insertId()));
        },
        [onError](const drogon::orm::DrogonDbException& e) {
            onError(e.base().what());
        },
        username, name, passwordHash, salt, dateOfJoining
    );
}

void MongoDB_adaptor::fetchUserCredentials(
    const std::string& username,
    std::function<void(int userId, const std::string& passwordHash, const std::string& salt, const std::string& name, const std::string& dateOfJoining)> onSuccess,
    std::function<void(const std::string&)> onError) const
{
    auto dbClient = drogon::app().getDbClient();
    std::string sql = formatQuery("SELECT id, password_hash, salt, name, date_of_joining FROM users WHERE username = $1;");

    dbClient->execSqlAsync(
        sql,
        [onSuccess, onError, username](const drogon::orm::Result& result) {
            if (result.empty()) {
                onError("User " + username + " not found.");
                return;
            }
            const auto& row = result[0];
            onSuccess(
                row["id"].as<int>(),
                row["password_hash"].as<std::string>(),
                row["salt"].as<std::string>(),
                row["name"].as<std::string>(),
                row["date_of_joining"].as<std::string>()
            );
        },
        [onError](const drogon::orm::DrogonDbException& e) {
            onError(e.base().what());
        },
        username
    );
}

void MongoDB_adaptor::addDocument(
    int userId,
    const std::string& title,
    const std::string& content,
    std::function<void(int docId)> onSuccess,
    std::function<void(const std::string&)> onError) const
{
    auto dbClient = drogon::app().getDbClient();
    std::string summary = content.substr(0, 100); 
    std::string sql = formatQuery("INSERT INTO documents (user_id, title, content, content_summary) VALUES ($1, $2, $3, $4);");

    dbClient->execSqlAsync(
        sql,
        [onSuccess](const drogon::orm::Result& result) {
            onSuccess(static_cast<int>(result.insertId()));
        },
        [onError](const drogon::orm::DrogonDbException& e) {
            onError(e.base().what());
        },
        userId, title, content, summary
    );
}

void MongoDB_adaptor::deleteDocument(
    int docId,
    int userId,
    std::function<void()> onSuccess,
    std::function<void(const std::string&)> onError) const
{
    auto dbClient = drogon::app().getDbClient();
    std::string sql = formatQuery("DELETE FROM documents WHERE id = $1 AND user_id = $2;");

    dbClient->execSqlAsync(
        sql,
        [onSuccess, onError](const drogon::orm::Result& result) {
            if (result.affectedRows() == 0) {
                onError("Document not found or access denied.");
                return;
            }
            onSuccess();
        },
        [onError](const drogon::orm::DrogonDbException& e) {
            onError(e.base().what());
        },
        docId, userId
    );
}
