#ifndef MYSERVERCONFIG_H
#define MYSERVERCONFIG_H

#include "serverconfig.h"
#include <string>

class MyServerConfig : public ServerConfig {
public:
    bool loadAccountsFromFile(const std::string& filename) override;
};


#endif // MYSERVERCONFIG_H
