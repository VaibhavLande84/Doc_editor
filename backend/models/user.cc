#include "user.h"
#include <iostream>
#include <utility>

#include "../adaptor/mongoDBadaptor.h"

// Implementation of getDocumentsByUserId
void User::getDocumentsByUserId(int targetUserId,
                                std::function<void(const std::vector<Document>&)> onSuccess,
                                std::function<void(const std::string&)> onError) 
{
    MongoDB_adaptor adaptor;
    adaptor.fetchDocumentsByUserId(targetUserId, std::move(onSuccess), std::move(onError));
}

// Implementation of getUserDetails
void User::getUserDetails(int targetUserId) {
    MongoDB_adaptor adaptor;
    adaptor.fetchUserDetails(
        targetUserId,
        [this](int userId, const std::string& userName, const std::string& joiningDate) {
            this->user_ID = userId;
            this->name = userName;
            this->date_of_joining = joiningDate;
            std::cout << "Successfully initialized local User instance for: " << this->name << std::endl;
        },
        [](const std::string& errorMessage) {
            std::cerr << "Failed to fetch user details: " << errorMessage << std::endl;
        }
    );
}