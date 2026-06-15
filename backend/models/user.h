#ifndef USER_H
#define USER_H

#include <drogon/drogon.h>
#include <vector>
#include <string>
#include <functional>

struct Document {
    int id;
    int user_id;
    std::string title;
    std::string content_summary;
};

class User {
private:
    int user_ID;
    std::string name;
    std::string date_of_joining;
public:
    void getDocumentsByUserId(int targetUserId,
                              std::function<void(const std::vector<Document>&)> onSuccess,
                              std::function<void(const std::string&)> onError);
    void getUserDetails(int targetUserId);
};

#endif // USER_H