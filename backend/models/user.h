#ifndef USER_H
#define USER_H

#include <functional>
#include <string>
#include <vector>

#include "doc.h"

using string = std::string;

class User {
private:
    int user_ID;
    string name;
    string date_of_joining;
    std::vector<string> documents_list{};
public:
    User() = default;
    User(int userId, string userName, string joiningDate);

    int getUserId() const;
    const string& getName() const;
    const string& getDateOfJoining() const;
    const std::vector<std::string>& getDocumentsList() const;

    void getDocumentsByUserId(int targetUserId,
                              std::function<void(const std::vector<Document>&)> onSuccess,
                              std::function<void(const string&)> onError);
    void getUserDetails(int targetUserId);
};

#endif // USER_H