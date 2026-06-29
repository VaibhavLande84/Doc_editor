#include "user_manager.h"
#include "storage_manager.h"
#include "../utils/crypto_util.h"
#include <chrono>
#include <iomanip>
#include <sstream>
#include <ctime>

void UserManager::registerUser(
    const std::string& username,
    const std::string& name,
    const std::string& password,
    std::function<void(int)> onSuccess,
    std::function<void(const std::string&)> onError)
{
    std::string salt = CryptoUtil::generateSalt();
    std::string hash = CryptoUtil::hashPassword(password, salt);
    
    auto now = std::chrono::system_clock::now();
    std::time_t in_time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    
#ifdef _MSC_VER
    struct tm buf;
    if (localtime_s(&buf, &in_time_t) == 0) {
        ss << std::put_time(&buf, "%Y-%m-%d %H:%M:%S");
    } else {
        ss << "2026-06-23 00:00:00"; 
    }
#else
    struct tm* buf = std::localtime(&in_time_t);
    if (buf) {
        ss << std::put_time(buf, "%Y-%m-%d %H:%M:%S");
    } else {
        ss << "2026-06-23 00:00:00";
    }
#endif

    std::string dateOfJoining = ss.str();

    StorageManager::getAdaptor()->saveUser(
        username, name, hash, salt, dateOfJoining,
        std::move(onSuccess), std::move(onError)
    );
}

void UserManager::authenticateUser(
    const std::string& username,
    const std::string& password,
    std::function<void(int, const std::string&, const std::string&)> onSuccess,
    std::function<void(const std::string&)> onError)
{
    StorageManager::getAdaptor()->fetchUserCredentials(
        username,
        [password, onSuccess, onError](int userId, const std::string& hash, const std::string& salt, const std::string& name, const std::string& dateOfJoining) {
            if (CryptoUtil::verifyPassword(password, salt, hash)) {
                onSuccess(userId, name, dateOfJoining);
            } else {
                onError("Invalid credentials. Incorrect password.");
            }
        },
        std::move(onError)
    );
}
