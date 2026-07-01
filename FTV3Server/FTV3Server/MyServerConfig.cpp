#include "MyServerConfig.h"
#include "serverconfig.h"
#include <fstream>
#include <sstream>
#include <iostream>

bool MyServerConfig::loadAccountsFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "[ERROR] Khong the mo file: " << filename << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::istringstream iss(line);
        std::string username, password;
        iss >> username >> password;

        if (!username.empty() && !password.empty()) {
            Account* acc = new Account(username);
            acc->setPassword(password);
            addAccount(acc);
            std::cout << "[INFO] Loaded account: " << username << std::endl;
        } else {
            std::cout << "[WARN] Dong khong hop le: " << line << std::endl;
        }
    }

    file.close();
    return true;
}
