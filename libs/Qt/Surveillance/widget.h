#ifndef WIDGETDEMO_H
#define WIDGETDEMO_H

#include <QWidget>
#include <QLineEdit>
#include <vector>
#include <iostream>
#include "register.h"
namespace Ui {
class Widget;
}
class Form;
class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void handleRegistrationSuccess(const QString &username, const QString &password);  // 处理注册成功
    void saveUserToConfig(const QString &username, const QString &password);//保存用户信息到配置文件
signals :
    void getaa(QString name);
private:
    Ui::Widget *ui;

    Register *registerPage;

struct User{
        QString name;
        QString password;
        User(const QString &name,const QString &password)
            :name(name),password(password)
        {
        }
    };
QVector<User> vec;

};

#endif // WIDGETDEMO_H
