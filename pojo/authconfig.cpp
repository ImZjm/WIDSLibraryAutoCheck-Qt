#include "authconfig.h"

AuthConfig& AuthConfig::getInstance(){
    static AuthConfig authConfig;
    return authConfig;
}


int AuthConfig::getFormVersion(){
    return formVersion;
}
QList<User>& AuthConfig::getUsers(){
    return users;
}

void AuthConfig::setFormVersion(int v){
    formVersion = v;
}
