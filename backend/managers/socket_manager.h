#ifndef SOCKET_MANAGER_H
#define SOCKET_MANAGER_H

#include <drogon/WebSocketConnection.h>
#include <unordered_map>
#include <unordered_set>
#include <mutex>
#include <string>

class SocketManager {
private:
    std::mutex mutex_;
    std::unordered_map<int, std::unordered_set<drogon::WebSocketConnectionPtr>> documentRooms_;
    
    SocketManager() = default;

public:
    static SocketManager& instance();

    void initialize();
    void joinRoom(int docId, const drogon::WebSocketConnectionPtr& conn);
    void leaveRoom(int docId, const drogon::WebSocketConnectionPtr& conn);
    void leaveAllRooms(const drogon::WebSocketConnectionPtr& conn);
    void broadcastToRoom(int docId, const std::string& message, const drogon::WebSocketConnectionPtr& excludeConn = nullptr);
};

#endif // SOCKET_MANAGER_H