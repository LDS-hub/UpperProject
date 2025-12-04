#include "register.h"
#include "ui_register.h"
#include <QMessageBox>
#include <QDebug>
#include <QRegularExpression>
#include <QRegularExpressionValidator>

Register::Register(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Register)
{
    ui->setupUi(this);
    this->setWindowTitle("用户注册");

    // 设置密码显示为圆点
    ui->lineEdit_2->setEchoMode(QLineEdit::Password);
    ui->lineEdit_3->setEchoMode(QLineEdit::Password);

    // 可选：设置输入限制
    // 用户名只允许字母、数字、下划线
    QRegularExpression regExp("[a-zA-Z0-9_]+");
    ui->lineEdit->setValidator(new QRegularExpressionValidator(regExp, this));
}

Register::~Register()
{
    delete ui;
}

void Register::on_pushButton_2_clicked()
{
    this->close();  // 关闭注册页面
}

void Register::on_pushButton_clicked()
{
    QString username = ui->lineEdit->text().trimmed();
        QString password = ui->lineEdit_2->text();
        QString confirmPassword = ui->lineEdit_3->text();

        // 1. 验证输入是否为空
        if (username.isEmpty() || password.isEmpty() || confirmPassword.isEmpty()) {
            QMessageBox::warning(this, "注册失败", "所有字段都必须填写！");
            return;
        }

        // 2. 验证用户名
        if (!validateUsername(username)) {
            QMessageBox::warning(this, "注册失败",
                "用户名无效！\n只能包含字母、数字和下划线，长度3-20位。");
            return;
        }

        // 3. 验证密码
        if (!validatePassword(password)) {
            QMessageBox::warning(this, "注册失败",
                "密码无效！\n长度至少6位，最多20位。");
            return;
        }

        // 4. 验证两次密码是否一致
        if (password != confirmPassword) {
            QMessageBox::warning(this, "注册失败", "两次输入的密码不一致！");
            ui->lineEdit_2->clear();
            ui->lineEdit_3->clear();
            ui->lineEdit_2->setFocus();
            return;
        }

        // 5. 所有验证通过，发送注册成功信号
        emit registrationSuccess(username, password);

        // 6. 提示用户
        QMessageBox::information(this, "注册成功",
            QString("用户 %1 注册成功！").arg(username));

        // 7. 关闭注册窗口
        this->close();
    }

bool Register::validatePassword(const QString &password)
{
    // 密码长度验证：6-20位
    if (password.length() < 6 || password.length() > 20) {
        return false;
    }
    return true;
}
bool Register::validateUsername(const QString &username)
{
    // 用户名长度验证：3-20位
    if (username.length() < 3 || username.length() > 20) {
        return false;
    }

    // 用户名只能包含字母、数字、下划线
    QRegularExpression regExp("^[a-zA-Z0-9_]+$");
    return regExp.match(username).hasMatch();
}
