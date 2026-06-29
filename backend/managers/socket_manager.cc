#include "socket_manager.h"
#include <iostream>

SocketManager& SocketManager::instance() {
    static SocketManager inst;
    return inst;
}

void SocketManager::initialize() {
    std::cout << "SocketManager initialized successfully." << std::endl;
}

void SocketManager::joinRoom(int docId, const drogon::WebSocketConnectionPtr& conn) {
    std::lock_guard<std::mutex> lock(mutex_);
    documentRooms_[docId].insert(conn);
    std::cout << "Connection joined room for document " << docId << std::endl;
}

void SocketManager::leaveRoom(int docId, const drogon::WebSocketConnectionPtr& conn) {
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = documentRooms_.find(docId);
    if (it != documentRooms_.end()) {
        it->second.erase(conn);
        std::cout << "Connection left room for document " << docId << std::endl;
        if (it->second.empty()) {
            documentRooms_.erase(it);
        }
    }
}

void SocketManager::leaveAllRooms(const drogon::WebSocketConnectionPtr& conn) {
    std::lock_guard<std::mutex> lock(mutex_);
    for (auto it = documentRooms_.begin(); it != documentRooms_.end();) {
        it->second.erase(conn);
        if (it->second.empty()) {
            it = documentRooms_.erase(it);
        } else {
            ++it;
        }
    }
    std::cout << "Connection left all rooms." << std::endl;
}

void SocketManager::broadcastToRoom(int docId, const std::string& message, const drogon::WebSocketConnectionPtr& excludeConn) {
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = documentRooms_.find(docId);
    if (it != documentRooms_.end()) {
        for (const auto& conn : it->second) {
            if (conn != excludeConn && conn->connected()) {
                conn->send(message);
            }
        }
    }
}
