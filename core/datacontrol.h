#ifndef DATACONTROL_H
#define DATACONTROL_H

#include "../pojo/authconfig.h"

#include <QJsonObject>
#include <QList>

class DataControl
{
    AuthConfig & config = AuthConfig::getInstance();
public:
    DataControl(){}
    bool initConfig();
};

#endif // DATACONTROL_H
