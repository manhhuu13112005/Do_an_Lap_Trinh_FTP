#ifndef USER_MANAGER_H
#define USER_MANAGER_H

#include <string>
#include <unordered_map>

class UserManager {
private:
    std::unordered_map<std::string, std::string> users;

public:
    // Load user từ file users.txt
    bool loadUsers(const std::string &filename);

    // Kiểm tra đăng nhập
    bool checkPassword(const std::string &username, const std::string &password);

    // Debug: in danh sách user
    void printUsers();
};

#endif
