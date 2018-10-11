#ifndef BASEDSL_HPP
#define BASEDSL_HPP
#include <QObject>
class BaseDsl : public QObject {
    Q_OBJECT
public:
    explicit BaseDsl(QObject *parent = nullptr);
    virtual ~BaseDsl();

    /**
     * @brief dial 开始PPPOE拨号
     * @param username 用户名
     * @param password 密码
     * @param device_name 网卡设备名(Windows 上会忽略此参数)
     * @param errorMessage 拨号失败错误消息
     * @return true返回成功
     */
    virtual bool dial(QString const &username,
                      QString const &password,
                      QString const &device_name,
                      QString &errorMessage) = 0;

    /**
     * @brief hangUp 挂断
     */
    virtual void hangUp() = 0;

    /**
     * @brief isDisconnected 返回连接状态
     * @return true 为已连接 false为断开
     */
    virtual bool isDisconnected() = 0;
    /**
     * @brief get_available_interfaces 获取可用网卡列表
     * @return 网卡名字符串列表
     */
    virtual QStringList getAvailableInterfaces() = 0;
protected:
    QString name;
};

#endif // BASEDSL_HPP
