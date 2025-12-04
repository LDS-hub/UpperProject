#ifndef REGISTER_H
#define REGISTER_H

#include <QWidget>

namespace Ui {
class Register;
}

class Register : public QWidget
{
    Q_OBJECT

public:
    explicit Register(QWidget *parent = nullptr);
    ~Register();
signals:
    // 注册成功信号，传递用户名和密码
    void registrationSuccess(const QString &username, const QString &password);
private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::Register *ui;
    // 验证密码是否有效
    bool validatePassword(const QString &password);

    // 验证用户名是否有效
    bool validateUsername(const QString &username);
};

#endif // REGISTER_H
