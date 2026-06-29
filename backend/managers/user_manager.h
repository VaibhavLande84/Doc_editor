#ifndef USER_MANAGER_H
#define USER_MANAGER_H

#include <string>
#include <functional>
#include "../models/user.h"

class UserManager {
public:
    static void registerUser(
        const std::string& username,
        const std::string& name,
        const std::string& password,
        std::function<void(int userId)> onSuccess,
        std::function<void(const std::string&)> onError);

    static void authenticateUser(
        const std::string& username,
        const std::string& password,
        std::function<void(int userId, const std::string& name, const std::string& dateOfJoining)> onSuccess,
        std::function<void(const std::string&)> onError);
};

#endif // USER_MANAGER_H