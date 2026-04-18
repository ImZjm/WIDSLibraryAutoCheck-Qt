#include "apirequest.h"

#include <QDebug>
#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <QUrlQuery>

namespace {
constexpr const char* kBaseHost = "https://form.qun100.com";
constexpr const char* kUserAgent =
    "Mozilla/5.0 (iPhone; CPU iPhone OS 18_2_1 like Mac OS X) "
    "AppleWebKit/605.1.15 (KHTML, like Gecko) Mobile/15E148 "
    "Html5Plus/1.0 (Immersed/20) uni-app";

QByteArray doSyncRequest(QNetworkAccessManager& manager,
                         QNetworkRequest& request,
                         const QByteArray& method,
                         const QByteArray& body,
                         int& httpStatus,
                         bool& ok) {
    QNetworkReply* reply = nullptr;

    if (method == "GET") {
        reply = manager.get(request);
    } else if (method == "PUT") {
        reply = manager.put(request, body);
    } else {
        ok = false;
        httpStatus = -1;
        return {};
    }

    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    httpStatus = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "HTTP request failed:" << reply->errorString();
        ok = false;
        reply->deleteLater();
        return {};
    }

    ok = true;
    QByteArray response = reply->readAll();
    reply->deleteLater();
    return response;
}
}

int getAuthCode(const QString& phoneNumber) {
    QNetworkAccessManager manager;

    QUrl url(QString::fromLatin1(kBaseHost) + "/v1/phone/login");
    QUrlQuery query;
    query.addQueryItem("phone", phoneNumber);
    url.setQuery(query);

    QNetworkRequest request(url);
    request.setRawHeader("User-Agent", kUserAgent);
    request.setRawHeader("Client-App-Id", "wx3f32186d2340171c");

    int status = -1;
    bool ok = false;
    QByteArray responseData = doSyncRequest(manager, request, "GET", {}, status, ok);

    if (!ok || status != 200) {
        qDebug() << "HTTP request failed with status code:" << status;
        return -1;
    }

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(responseData, &error);

    if (error.error != QJsonParseError::NoError || !doc.isObject()) {
        qDebug() << "Failed to parse JSON response.";
        return -1;
    }

    QJsonObject root = doc.object();
    int code = root.value("code").toInt();
    if (code != 0) {
        qDebug() << "API Error Code:" << code;
        return -1;
    }

    QJsonObject data = root.value("data").toObject();
    return data.value("expireIn").toInt(-1);
}

QString getAuthToken(const QString& phone, const QString& codeStr) {
    QNetworkAccessManager manager;

    QUrl url(QString::fromLatin1(kBaseHost) + "/v1/phone/login");
    QNetworkRequest request(url);
    request.setRawHeader("User-Agent", kUserAgent);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject payload;
    payload.insert("phone", phone);
    payload.insert("code", codeStr);
    QByteArray body = QJsonDocument(payload).toJson(QJsonDocument::Compact);

    int status = -1;
    bool ok = false;
    QByteArray responseData = doSyncRequest(manager, request, "PUT", body, status, ok);

    if (!ok || status != 200) {
        return QString();
    }

    QJsonParseError parseError;
    QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData, &parseError);

    if (parseError.error != QJsonParseError::NoError || !jsonResponse.isObject()) {
        return QString();
    }

    QJsonObject rootObj = jsonResponse.object();

    if (!rootObj.contains("code") || rootObj.value("code").toInt() != 0) {
        return QString();
    }

    if (!rootObj.contains("data") || !rootObj.value("data").isObject()) {
        return QString();
    }

    QJsonObject dataObj = rootObj.value("data").toObject();

    if (!dataObj.contains("token") || !dataObj.value("token").isString()) {
        return QString();
    }

    return dataObj.value("token").toString();
}
