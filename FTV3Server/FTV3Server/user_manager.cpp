#include "user_manager.h"
#include <fstream>
#include <iostream>
#include <sstream>

#ifdef _WIN32
#include <windows.h>
#endif

bool UserManager::loadUsers(const std::string &filename)
{
    // In ra thư mục hiện tại (debug)
#ifdef _WIN32
    char buffer[260];
    GetCurrentDirectoryA(260, buffer);
    std::cout << "[DEBUG] Working directory: " << buffer << std::endl;
#endif

    // Kiểm tra file có tồn tại không
    std::ifstream test(filename);
    if (!test.good()) {
        std::cout << "[ERROR] File khong ton tai trong working directory: " << filename << std::endl;
    }
    test.close();

    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cout << "[ERROR] Khong the mo file user: " << filename << std::endl;
        return false;
    }

    std::cout << "[INFO] Da mo file user: " << filename << std::endl;

    std::string line;
    while (std::getline(file, line))
    {
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string username, password;

        ss >> username >> password;
        if (!username.empty() && !password.empty())
        {
            users[username] = password;
            std::cout << "[INFO] Loaded user: " << username << std::endl;
        }
        else
        {
            std::cout << "[WARN] Dong bi bo qua (khong hop le): " << line << std::endl;
        }
    }

    file.close();
    return true;
}

bool UserManager::checkPassword(const std::string &username, const std::string &password)
{
    auto it = users.find(username);
    if (it == users.end())
        return false;

    return it->second == password;
}

void UserManager::printUsers()
{
    std::cout << "Danh sach user trong he thong:\n";
    for (auto &p : users)
    {
        std::cout << " - " << p.first << "\n";
    }
}
