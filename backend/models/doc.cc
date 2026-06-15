#include "doc.h"
#include <iostream>

void getSpecificDocument(int doc_ID, int User_ID,
                         std::function<void(const Document&)> onSuccess,
                         std::function<void(const std::string&)> onError)
{
    auto dbClient = drogon::app().getDbClient();
    
    // Querying by the unique document ID ($1)
    dbClient->execSqlAsync(
        "SELECT id, user_id, title, content FROM documents WHERE id = $1;",
        [doc_ID],
        [onSuccess, onError, User_ID](const drogon::orm::Result &result) {
            
            // 1. Check if the document even exists
            if (result.empty()) {
                onError("Error: Document not found.");
                return;
            }

            const auto &row = result[0];
            int db_user_id = row["user_id"].as<int>();

            // 2. Security Check: Does the owner of this document match the logged-in User_ID?
            if (db_user_id != User_ID) {
                onError("Access Denied: You do not have permission to view this document.");
                return; // Stop execution immediately!
            }

            // 3. If authorized, rebuild the single Document object
            Document doc;
            doc.id = row["id"].as<int>();
            doc.user_id = db_user_id;
            doc.title = row["title"].as<std::string>();
            doc.content_summary = row["content"].as<std::string>();

            // Pass the single document object back to the success callback
            onSuccess(doc);
        },
        [onError](const drogon::orm::DrogonDbException &e) {
            onError(e.base().what());
        }
    );
}