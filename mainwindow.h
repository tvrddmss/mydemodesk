#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTime>
#include <QTextEdit>  //文本框输入头文件 可以多行输入

//添加网络相关头文件
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>

//添加JSON解析相关的头文件，因为从该服务器请求的天气数据是以json的形式回复的
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QTime timeNow;
    QTextEdit *te;
    QNetworkAccessManager *manager;

    //点击事件
public slots:
    void pushButtontest_clicked();

    void replyDone(QNetworkReply *reply);
};


#endif // MAINWINDOW_H
