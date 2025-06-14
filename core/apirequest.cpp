#include "apirequest.h"
#include "httplib.h"

#include <QCoreApplication>
#include <QJsonDocument>
#include <QJsonObject>

int getAuthCode(const QString& phoneNumber){
    httplib::SSLClient cli("form.qun100.com");

    // 设置 Headers
    httplib::Headers headers = {
        {"User-Agent", "Mozilla/5.0 (iPhone; CPU iPhone OS 18_2_1 like Mac OS X) AppleWebKit/605.1.15 (KHTML, like Gecko) Mobile/15E148 Html5Plus/1.0 (Immersed/20) uni-app"},
        {"Client-App-Id", "wx3f32186d2340171c"}
    };

    // 构造 URL
    std::string urlPath = "/v1/phone/login?phone=" + phoneNumber.toStdString();

    // 发送 GET 请求
    auto res = cli.Get(urlPath.c_str(), headers);

    if (res && res->status == 200) {
        // 使用 Qt 解析 JSON
        QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson(
            QByteArray(res->body.data(), static_cast<int>(res->body.size())), &error);

        if (error.error == QJsonParseError::NoError && doc.isObject()) {
            QJsonObject root = doc.object();

            int code = root.value("code").toInt();
            if (code == 0) {
                QJsonObject data = root.value("data").toObject();
                return data.value("expireIn").toInt(); // 提取 expireIn
            } else {
                qDebug() << "API Error Code:" << code;
            }
        } else {
            qDebug() << "Failed to parse JSON response.";
        }
    } else {
        qDebug() << "HTTP request failed with status code:"
                 << (res ? QString::number(res->status) : "N/A");
    }

    return -1; // 出错返回 -1
}

QString getAuthToken(const QString& phone, const QString& codeStr) {
    httplib::SSLClient cli("form.qun100.com");

    // 设置请求头
    httplib::Headers headers = {
        {"User-Agent", "Mozilla/5.0 (iPhone; CPU iPhone OS 18_2_1 like Mac OS X) AppleWebKit/605.1.15 (KHTML, like Gecko) Mobile/15E148 Html5Plus/1.0 (Immersed/20) uni-app"},
        {"Content-Type", "application/json"}
    };

    // 构造 JSON body
    QString jsonBodyStr = QString(R"({"phone":"%1","code":"%2"})").arg(phone, codeStr);
    std::string body = jsonBodyStr.toStdString();

    // 发送 PUT 请求
    auto res = cli.Put("/v1/phone/login", headers, body, "application/json");

    if (!res || res->status != 200) {
        return QString(); // 网络错误或状态码非 200
    }

    // 将响应体转换为 QByteArray
    QByteArray responseData(res->body.c_str(), res->body.size());

    // 解析 JSON
    QJsonParseError parseError;
    QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        // JSON 解析失败
        return QString();
    }

    if (!jsonResponse.isObject()) {
        return QString(); // 根类型不是对象
    }

    QJsonObject rootObj = jsonResponse.object();

    // 检查 code 是否存在且为 0
    if (!rootObj.contains("code") || rootObj.value("code").toInt() != 0) {
        return QString();
    }

    // 检查 data 对象是否存在
    if (!rootObj.contains("data") || !rootObj.value("data").isObject()) {
        return QString();
    }

    QJsonObject dataObj = rootObj.value("data").toObject();

    // 提取 token
    if (!dataObj.contains("token") || !dataObj.value("token").isString()) {
        return QString();
    }

    QString token = dataObj.value("token").toString();
    return token;
}
