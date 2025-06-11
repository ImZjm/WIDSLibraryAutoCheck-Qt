#include "checkcore.h"

#include <QString>
#include <QCoreApplication>
#include <QDir>
#include <QProcess>

QString CheckAll(){
    // 获取当前应用程序所在目录
    QString appPath = QCoreApplication::applicationDirPath();

    // 构建目标exe的路径
    QString corePath = appPath + QDir::separator() + "core";
    QString targetExe = corePath + QDir::separator() + "CheckJ.exe";

    // 使用 QProcess 启动外部程序
    QProcess process;
    process.setWorkingDirectory(corePath);
    process.start(targetExe);

    if (!process.waitForStarted()) {
        return QString("启动失败: %1").arg(process.errorString());
    }

    // 等待程序执行完成
    process.waitForFinished(-1);  // -1 表示无限等待

    // 获取输出和错误信息
    QString output = QString::fromLocal8Bit(process.readAllStandardOutput());
    QString error = QString::fromLocal8Bit(process.readAllStandardError());

    QString result = output + "\n" + error;
    return result;
}

