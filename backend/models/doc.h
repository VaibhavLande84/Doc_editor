#ifndef USER_H
#define USER_H

#include <drogon/drogon.h>
#include <vector>
#include <string>
#include <functional>

class Documenr{
private:
    int id;
    int user_id;
    std::string title;
    std::string summary;
public:

    void getDocumentsByUserId(int doc_ID,int User_ID,
                              std::function<void(const std::vector<Document>&)> onSuccess,
                              std::function<void(const std::string&)> onError);
    
    void getUserDetails(int doc_ID,int User_ID);
}

#endif