#ifndef USER_H
#define USER_H

#include <QString>

class User
{
    QString stuId;
    QString name;
    QString auth;
public:
    User(QString stuId, QString name, QString auth):stuId(stuId), name(name), auth(auth){};
    QString getStuId();
    QString getName();
    QString getAuth();
};

#endif // USER_H
