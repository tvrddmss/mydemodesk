#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //绑定事件
    connect(ui->pushButtonctest, SIGNAL(clicked()), this, SLOT(pushButtontest_clicked()));
    //定义控件
    te = ui->textEdit;

    manager = new QNetworkAccessManager(this);  //新建QNetworkAccessManager对象
    //当收到http请求回复的数据，便会调用replyDone槽函数。
    //关联信号和槽
    connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(replyDone(QNetworkReply*)));
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::pushButtontest_clicked()
{
    QT_TRY {
        timeNow = QTime::currentTime();
        qDebug("pushButtonConnect");
        te->setText(timeNow.toString()+"点击了按钮");

        //获得需要查询天气的城市名称

       QString local_city = QString(ui->comboBox->currentText().trimmed());
        char quest_array[256]="http://wthrcdn.etouch.cn/weather_mini?city=";
        QNetworkRequest quest;
        sprintf(quest_array,"%s%s",quest_array,local_city.toUtf8().data());
        quest.setUrl(QUrl(quest_array));
        quest.setHeader(QNetworkRequest::UserAgentHeader,"RT-Thread ART");
        //发送get网络请求
        manager->get(quest);
    } QT_CATCH(...) {
        qDebug("test");
    }

}


void MainWindow::replyDone(QNetworkReply *reply)
{

    QString weather_type;
    QString temperature;
    QString wind_power;

    QString all = reply->readAll();
    //        ui->textEdit->setText(all); //将接收到的数据显示出来
    qDebug()<<"recv weather data!!"<<all;
    QJsonParseError err;
    QJsonDocument json_recv = QJsonDocument::fromJson(all.toUtf8(),&err);//解析json对象
    qDebug() << err.error;
    if(!json_recv.isNull())
    {
        QJsonObject object = json_recv.object();

        if(object.contains("data"))
        {
            QJsonValue value = object.value("data");  // 获取指定 key 对应的 value
            if(value.isObject())
            {
                QJsonObject object_data = value.toObject();
                if(object_data.contains("forecast"))
                {
                    QJsonValue value = object_data.value("forecast");
                    if(value.isArray())
                    {
                        QJsonObject today_weather = value.toArray().at(0).toObject();
                        weather_type = today_weather.value("type").toString();

                        QString low = today_weather.value("low").toString();
                        QString high = today_weather.value("high").toString();
                        //                        QString ganmao = today_weather.value("ganmao").toString();//感冒提示
                        temperature= low.mid(low.length()-3,4) +"~"+ high.mid(high.length()-3,4);
                        QString strength = today_weather.value("fengli").toString();
                        strength.remove(0,8);
                        strength.remove(strength.length()-2,2);
                        wind_power= today_weather.value("fengxiang").toString() + strength;
                        ui->lab_type->setText(weather_type); //显示天气类型
                        ui->lab_temperature->setText(temperature);   //显示温度
                        ui->lab_wind_power->setText(wind_power); //显示风力
                        //                        this->lab_ganmao->setText(ganmao);
                    }
                }
            }
        }

    }
    else
    {
        qDebug()<<"请求数据失败";
    }
    reply->deleteLater(); //销毁请求对象


}

