#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QLineEdit>
#include <QLabel>
#include <QSettings>
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>
#include "register.h"
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    //vec.push_back(User("admin", "admin"));


    //通过编码画界面
    this->setWindowTitle("程序");
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_2_clicked()
{
    QString text = ui->lineEdit->text();
    QString textl = ui->lineEdit_2->text();

    qDebug() << "=== 登录尝试 ===";
    qDebug() << "输入的用户名:" << text;
    qDebug() << "输入的密码:" << textl;

    QString configPath = "D:/Qt/woniuQT/Surveillance/config.ini";
    QSettings settings(configPath, QSettings::IniFormat);

    // 读取用户数量
    int userCount = settings.value("Login/user_count", 0).toInt();
    qDebug() << "配置中用户数量:" << userCount;

    // 遍历所有用户
    for (int i = 1; i <= userCount; i++) {
        QString usernameKey = QString("Login/user%1_name").arg(i);
        QString passwordKey = QString("Login/user%1_password").arg(i);

        QString configUser = settings.value(usernameKey).toString();
        QString configPass = settings.value(passwordKey).toString();

        qDebug() << QString("用户%1: %2 / %3").arg(i).arg(configUser).arg(configPass);

        if ((text == configUser) && (textl == configPass)) {
            qDebug() << "登录成功！用户ID:" << i;
            ui->label_4->setText("登录成功");
            ui->label_4->setStyleSheet("color: green;");
            emit getaa(text);
            return;
        }
    }

    // 内存验证
    for(auto &user : vec) {
        if((text == user.name) && (textl == user.password)) {
            qDebug() << "内存验证成功！";
            ui->label_4->clear();
            emit getaa(text);
            return;
        }
    }

    qDebug() << "登录失败！";
    ui->label_4->setText("用户名或密码错误");
    ui->label_4->setStyleSheet("color: red;");
}

void Widget::on_pushButton_3_clicked()
{
    // 创建注册页面
      registerPage = new Register();

       // 连接注册成功信号
       connect(registerPage, &Register::registrationSuccess,
               this, &Widget::handleRegistrationSuccess);

       // 显示注册页面
       registerPage->show();
}
// 处理注册成功的槽函数
void Widget::handleRegistrationSuccess(const QString &username, const QString &password)
{
    // 将新用户添加到内存
    vec.push_back(User(username, password));

    // 同时保存到配置文件
    saveUserToConfig(username, password);

    // 提示注册成功
    QMessageBox::information(this, "注册成功",
        QString("用户 %1 注册成功！\n现在可以使用该账号登录。").arg(username));

    // 自动填充登录表单（可选）
    ui->lineEdit->setText(username);
    ui->lineEdit_2->clear();

    qDebug() << "新用户注册成功:" << username;
}
// 保存用户到配置文件的函数
void Widget::saveUserToConfig(const QString &username, const QString &password)
{
    QString configPath = "D:/Qt/woniuQT/Surveillance/config.ini";
    QSettings settings(configPath, QSettings::IniFormat);

    // 读取现有用户数量
    int userCount = settings.value("Login/user_count", 0).toInt();

    // 检查用户名是否已存在
    for (int i = 1; i <= userCount; i++) {
        QString existingUser = settings.value(
            QString("Login/user%1_name").arg(i)).toString();
        if (existingUser == username) {
            QMessageBox::warning(this, "注册失败", "用户名已存在！");
            return;
        }
    }

    // 增加用户数量
    userCount++;
    settings.setValue("Login/user_count", userCount);

    // 保存新用户
    settings.setValue(QString("Login/user%1_name").arg(userCount), username);
    settings.setValue(QString("Login/user%1_password").arg(userCount), password);

    settings.sync();  // 确保写入磁盘

    qDebug() << "用户保存到配置文件，当前用户数:" << userCount;
}


