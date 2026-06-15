#include "user.h"
#include <iostream>

// Implementation of getDocumentsByUserId
void User::getDocumentsByUserId(int targetUserId,
                                std::function<void(const std::vector<Document>&)> onSuccess,
                                std::function<void(const std::string&)> onError) 
{
    // 1. Grab the database client managed by Drogon
    auto dbClient = drogon::app().getDbClient();

    // 2. Query the containerized MySQL database safely using a prepared statement ($1)
    dbClient->execSqlAsync(
        "SELECT id, user_id, title, content_summary FROM documents WHERE user_id = $1;",
        [targetUserId], 
        [onSuccess](const drogon::orm::Result &result) {
            std::vector<Document> userDocs;

            // Loop through all database rows returned by Docker
            for (const auto &row : result) {
                Document doc;
                doc.id = row["id"].as<int>();
                doc.user_id = row["user_id"].as<int>();
                doc.title = row["title"].as<std::string>();
                doc.content_summary = row["content_summary"].as<std::string>();
                
                userDocs.push_back(doc);
            }
            // Trigger the success sticky note with our populated vector
            onSuccess(userDocs);
        },
        [onError](const drogon::orm::DrogonDbException &e) {
            // Something broke (e.g. lost connection to the Docker container)
            onError(e.base().what());
        }
    );
}

// Implementation of getUserDetails
void User::getUserDetails(int targetUserId) {
    auto dbClient = drogon::app().getDbClient();

    dbClient->execSqlAsync(
        "SELECT id, name, date_of_joining FROM users WHERE id = $1;",
        [targetUserId],
        [this](const drogon::orm::Result &result) {
            if (!result.empty()) {
                const auto &row = result[0];
                
                // Since this runs asynchronously inside a lambda, we capture 'this' 
                // to safely assign database values directly back to our class private fields.
                this->user_ID = row["id"].as<int>();
                this->name = row["name"].as<std::string>();
                this->date_of_joining = row["date_of_joining"].as<std::string>();
                
                std::cout << "Successfully initialized local User instance for: " << this->name << std::endl;
            } else {
                std::cout << "User with ID " << targetUserId << " not found in database." << std::endl;
            }
        },
        [](const drogon::orm::DrogonDbException &e) {
            std::cerr << "Failed to fetch user details: " << e.base().what() << std::endl;
        }
    );
}