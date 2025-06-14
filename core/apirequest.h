#ifndef APIREQUEST_H
#define APIREQUEST_H

#include <QString>
int getAuthCode(const QString& phoneNumber);
QString getAuthToken(const QString& phone, const QString& codeStr);

#endif // APIREQUEST_H
