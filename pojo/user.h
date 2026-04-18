#ifndef USER_H
#define USER_H

#include <QString>

class User
{
    friend class DataControl;
    QString stuId;
    QString name;
    QString auth;
public:
    User(QString stuId, QString name, QString auth):stuId(stuId), name(name), auth(auth){};
    QString getStuId() const;
    QString getName() const;
    QString getAuth() const;
};

#endif // USER_H
