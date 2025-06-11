#ifndef AUTHCONFIG_H
#define AUTHCONFIG_H

#include "user.h"

#include <QList>

class AuthConfig
{
    int formVersion;
    QList<User> users;
    AuthConfig(){}
public:
    static AuthConfig& getInstance();

    int getFormVersion();
    QList<User>& getUsers();

    void setFormVersion(int);
};

#endif // AUTHCONFIG_H
