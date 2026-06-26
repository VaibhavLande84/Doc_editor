#include "user.h"
#include <iostream>
#include <utility>

#include "../managers/storage_manager.h"

User::User(int userId, string userName, string joiningDate)
    : user_ID(userId), name(std::move(userName)), date_of_joining(std::move(joiningDate))
{
}

int User::getUserId() const
{
    return user_ID;
}

const string& User::getName() const
{
    return name;
}

const string& User::getDateOfJoining() const
{
    return date_of_joining;
}

const std::vector<std::string>& User::getDocumentsList() const
{
    return documents_list;
}

// Implementation of getDocumentsByUserId
void User::getDocumentsByUserId(int targetUserId,
                                std::function<void(const std::vector<Document>&)> onSuccess,
                                std::function<void(const string&)> onError) 
{
    StorageManager::getAdaptor()->fetchDocumentsByUserId(targetUserId, std::move(onSuccess), std::move(onError));
}

// Implementation of getUserDetails
void User::getUserDetails(int targetUserId) {
    StorageManager::getAdaptor()->fetchUserDetails(
        targetUserId,
        [this](int userId, const string& userName, const string& joiningDate) {
            this->user_ID = userId;
            this->name = userName;
            this->date_of_joining = joiningDate;
            std::cout << "Successfully initialized local User instance for: " << this->name << std::endl;
        },
        [](const string& errorMessage) {
            std::cerr << "Failed to fetch user details: " << errorMessage << std::endl;
        }
    );
}