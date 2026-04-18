#include "datacontrol.h"
#include "../pojo/user.h"

#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDir>
#include <QApplication>
#include <QJsonArray>

bool DataControl::initConfig() {
    // 获取可执行文件所在的目录
    QString appDir = QApplication::applicationDirPath();

    // 构造 auth.json 的完整路径
    QString filePath = appDir + QDir::separator() + "core" + QDir::separator() + "auth.json";

    // 打开文件
    QFile file(filePath);
    if (!file.exists()) {
        QDir dir(QFileInfo(filePath).absolutePath());
        if (!dir.exists()) {
            if (!dir.mkpath(".")) {
                qDebug() << "创建目录失败：" << dir.absolutePath();
                return 0;
            }
        }

        if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
            qDebug() << "创建文件失败：" << filePath;
            return 0;
        }

        QJsonObject defaultObj;
        defaultObj.insert("formVersion", 0);
        defaultObj.insert("user", QJsonArray());
        file.write(QJsonDocument(defaultObj).toJson(QJsonDocument::Indented));
        file.close();
    }

    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "无法打开文件：" << filePath;
        return 0;
    }

    // 读取文件内容
    QByteArray data = file.readAll();
    file.close();

    // 解析 JSON
    QJsonParseError jsonError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &jsonError);

    if (jsonError.error != QJsonParseError::NoError) {
        qDebug() << "JSON 解析错误：" << jsonError.errorString();
        return 0;
    }

    if (!doc.isObject()) {
        qDebug() << "JSON 不是对象格式";
        return 0;
    }

    QJsonObject obj = doc.object();

    if (obj.contains("formVersion")) {
        config.setFormVersion(obj.value("formVersion").toInt());
    }

    if (obj.contains("user") && obj.value("user").isArray()) {
        QJsonArray arr = obj.value("user").toArray();
        for (QJsonArray::const_iterator it = arr.constBegin(); it != arr.constEnd(); ++it) {
            QJsonObject obj = it->toObject();
            User user(obj.value("stuId").toString(), obj.value("name").toString(), obj.value("auth").toString());
            config.getUsers().append(user);
        }
    }
    return 1;
}

bool DataControl::saveConfig() {
    QString appDir = QApplication::applicationDirPath();
    QString filePath = appDir + QDir::separator() + "core" + QDir::separator() + "auth.json";

    QDir dir(QFileInfo(filePath).absolutePath());
    if (!dir.exists()) {
        if (!dir.mkpath(".")) {
            qDebug() << "创建目录失败：" << dir.absolutePath();
            return false;
        }
    }

    QJsonObject rootObj;
    rootObj.insert("formVersion", config.getFormVersion());

    QJsonArray userArray;
    QList<User> users = config.getUsers();
    for (const User &user : users) {
        QJsonObject userObj;
        userObj.insert("stuId", user.getStuId());
        userObj.insert("name", user.getName());
        userObj.insert("auth", user.getAuth());
        userArray.append(userObj);
    }
    rootObj.insert("user", userArray);

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        qDebug() << "无法写入文件：" << filePath;
        return false;
    }

    file.write(QJsonDocument(rootObj).toJson(QJsonDocument::Indented));
    file.close();
    return true;
}
