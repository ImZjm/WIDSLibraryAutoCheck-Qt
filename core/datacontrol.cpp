#include "datacontrol.h"
#include "../pojo/user.h"

#include <QFile>
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
        qDebug() << "文件不存在：" << filePath;
        return 0;
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
        config.setFormVersion(obj.value("formVersion").toInt());\
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
