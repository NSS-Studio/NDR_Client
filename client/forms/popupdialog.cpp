#include "popupdialog.h"

popUpDialog::popUpDialog(QWidget *parent) : QDialog(parent)
{
    qDebug() << "create popUpDialog" << endl;
    pageNow = 1;
    popUp = new QDialog(this, Qt::WindowStaysOnTopHint | Qt::WindowCloseButtonHint);
    popUp -> setWindowTitle(tr("通知"));
    //popUp -> setWindowFlags(Qt::WindowStaysOnTopHint);
    popUp -> setFixedSize(364, 228);
    popUp -> move((QApplication::desktop()->availableGeometry().width() - popUp->width()), QApplication::desktop()->availableGeometry().height() - popUp->height() - 40);

    qDebug() << QApplication::desktop()->availableGeometry().width() << " " << QApplication::desktop()->availableGeometry().height() << endl;
    mainLay = new QVBoxLayout();
    hbAuth = new QHBoxLayout();
    hbTitle = new QHBoxLayout();
    hbPage = new QHBoxLayout();

    page = new QLabel(tr("N/N"));
    nextPage = new QPushButton(tr(">"), popUp);
    prePage = new QPushButton(tr("<"), popUp);
    laAuth = new QLabel(tr("作者: "), popUp);
    laTitle = new QLabel(tr("标题: "), popUp);
    closeTime = new QLabel(tr(" "), popUp);
    liAuth = new QLineEdit(tr("Unknow"), popUp);
    liTitle = new QLineEdit(tr("Unknow"), popUp);
    teText = new QTextBrowser(popUp);

    liTitle -> setReadOnly(true);
    liAuth -> setReadOnly(true);
    liTitle -> setFrame(false);
    liAuth -> setFrame(false);
    QFont font;
    font.setBold(true);
    liTitle -> setFont(font);
    liAuth -> setFont(font);
    teText -> setFrameStyle(QFrame::NoFrame);
    teText -> setFont(font);
    laAuth -> setFont(font);
    laTitle -> setFont(font);

    hbAuth -> addWidget(laAuth);
    hbAuth -> addWidget(liAuth);
    hbTitle -> addWidget(laTitle);
    hbTitle -> addWidget(liTitle);
    hbPage -> addWidget(page);
    hbPage -> addWidget(closeTime);
    hbPage -> addWidget(prePage);
    hbPage -> addWidget(nextPage);

    mainLay -> addLayout(hbTitle);
    mainLay -> addLayout(hbAuth);
    mainLay -> addWidget(teText);
    mainLay -> addLayout(hbPage);

    popUp -> setLayout(mainLay);

    sslConf.setPeerVerifyMode(QSslSocket::VerifyNone);
    sslConf.setProtocol(QSsl::TlsV1_2);


    history = new QSettings(utils::appHome + "/history.ini", QSettings::IniFormat);
    historyNum = 0;
    QString message;
    do{
        message = history->value(QString::number(historyNum, 10), "none").toString();
        text.insert(message);
        historyNum++;
        qDebug() << "meaasgeHistory: " << message << endl;
    } while (message != "none");
    --historyNum;

    //popUp -> exec();
}


popUpDialog::~popUpDialog()
{
    qDebug() << "delete popUpDialog" << endl;
    delete popUp;

}

//---------------------------function---------------------------//


void popUpDialog::getXmlFromNSS(const QString &url)
{
    bool flag = true;
#ifdef Q_OS_WIN
    flag = checkWindow();
#endif
    if (flag){
        qDebug() << "getXmlFromNSS(const QString &url)" << endl;
        request.setUrl(QUrl(url));
        request.setSslConfiguration(this->sslConf);

        reply = manager.get(request);
        connect(reply, &QNetworkReply::finished, this, &popUpDialog::getXmlDone);
    }
}

void popUpDialog::getXmlDone()
{
    qDebug() << "getXmlDone()" << endl;
    if (reply -> error() != QNetworkReply::NoError){
        QString stateValue = reply -> attribute(QNetworkRequest::HttpStatusCodeAttribute).toString();

        qDebug() << "Http value: " << stateValue << '\n'
                 << reply -> errorString() << endl;
        return;
    }

    QByteArray xml;
    xml = reply -> readAll();
    qDebug() << "xml string: " << xml << endl;

    QDomDocument doc;
    if (doc.setContent(xml, true)){
        QDomElement root = doc.documentElement();
        QDomNodeList nodes = root.elementsByTagName("status");
        QString status = nodes.at(0).toElement().text();

        qDebug() << "status: " << status << endl;
        if (nodes.isEmpty()){
            qDebug() << "xml message root is empty!!" << endl;
            return;
        }

        if (status == "failed"){
            nodes = root.elementsByTagName("message");
            status = nodes.at(0).toElement().text();
            qDebug() << "xml message failed: " << status << endl;
            return;
        }

        if (status == "none"){
            qDebug() << "there is no change at xml" << endl;
            return;
        }

        if (status == "has"){
            QDomElement root = doc.documentElement();
            QDomNodeList nodes = root.elementsByTagName("message");
            //qDebug() << nodes.size() << endl;
            QDomElement node;
            QString temp[4];
            for (int i = 0; i < nodes.size(); i++){
                 int k = 0;
                 node = nodes.at(i).toElement();
                 for (QDomNode j = node.firstChild(); !j.isNull() && k < 4; j = j.nextSibling()){

                      temp[k++] = j.toElement().text();
                      //temp[k++] = j.nodeValue(); //can not use this function,unknow
                 }
                 qDebug() << temp[0] << " " << temp[1] << " " << temp[2] <<" " << temp[3] << endl;
                 if (temp[0] == "1" && !temp[3].isEmpty()){
                     if (text.find(temp[3]) == text.constEnd()){
                         text.insert(temp[3]);
                         group.push_back(message{bool(temp[0].toInt()), temp[1], temp[2], temp[3]});
                         history->setValue(QString::number(historyNum++, 10), temp[3]);
                     } else {
                         qDebug() << "message already display" << endl;
                         return;
                     }
                 }
            }
            showMessage();
        }

    }
}

void popUpDialog::showMessage()
{
    qDebug() << "showMessage()" << endl;
    if (group.isEmpty()){
        qDebug() << "message group is empty!!" << endl;
        return;
    }

    liTitle -> setText(group[0].title);
    liAuth -> setText(group[0].auth);
    teText -> setText(group[0].text);
    page -> setText(tr("1/%0").arg(group.size()));

    prePage -> setEnabled(false);
    if (pageNow == group.size())
        nextPage -> setEnabled(false);

    connect(nextPage, &QPushButton::clicked, this, &popUpDialog::goNextPage);
    connect(prePage, &QPushButton::clicked, this, &popUpDialog::goPrePage);

    if (utils::settings->autoClose){
        time = new QTimer(this);
        connect(time, SIGNAL(timeout()), this, SLOT(timeCount()));
        timePass = 0;
        time->start(1000);
    }
    popUp -> show();
}

void popUpDialog::timeCount()
{
    if (timePass != 10){
        timePass++;
        closeTime->setText(tr("%0秒后自动关闭").arg(10 - timePass));
    } else {
        disconnect(time);
        delete time;
        popUp->close();
    }
}

void popUpDialog::resetTime()
{
    timePass = 0;
}

void popUpDialog::goNextPage()
{
    qDebug() << "goNextPage()" << endl;
    if (pageNow < group.size())
        pageNow++;

    if (pageNow > 1)
        prePage -> setEnabled(true);

    if (pageNow == group.size())
        nextPage -> setEnabled(false);

    int realPage = pageNow - 1;
    liTitle -> setText(group[realPage].title);
    liAuth -> setText(group[realPage].auth);
    teText -> setText(group[realPage].text);
    page -> setText(tr("%0/%1").arg(pageNow)
                               .arg(group.size()));

    resetTime();
}

void popUpDialog::goPrePage()
{
    qDebug() << "goPrePage()" << endl;
    if (pageNow > 1)
        pageNow--;

    if (pageNow == 1)
        prePage -> setEnabled(false);

    if (pageNow < group.size())
        nextPage -> setEnabled(true);

    int realPage = pageNow - 1;
    liTitle -> setText(group[realPage].title);
    liAuth -> setText(group[realPage].auth);
    teText -> setText(group[realPage].text);
    page -> setText(tr("%0/%1").arg(pageNow)
                               .arg(group.size()));

    resetTime();
}

#ifdef Q_OS_WIN
bool popUpDialog::checkWindow()
{
    qDebug() << "checkWindow()" << endl;

    //enum process
    //DWORD aProcesses[1024], cbNeeded, cProcesses;
    //if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded)){
    //    qDebug() << "Enum process failed" << endl;
    //    return false;
    //}
    //
    //cProcesses = cbNeeded/sizeof(DWORD);
    //
    //for (unsigned i = 0; i < cProcesses; i++){
    //    if (aProcess[i] != 0)
    //        PrinterPocessNameAndID(aProcesses[i]);
    //
    //}

    //checkScreen
    HWND handle = GetForegroundWindow();
    if (handle == nullptr){
        qDebug() << "Get active handle failed" << endl;
        //qDebug() << "Error code: " << GetLastError() << endl;
        return false;
    }

    WINDOWINFO info;
    GetWindowInfo(handle, &info);
    if (info.dwWindowStatus == WS_ACTIVECAPTION)
        qDebug() << "this window is active" << endl;
    else
        qDebug() << "this window is not active" << endl;
    //bool isTop = (info.dwExStyle == WS_EX_TOPMOST);

    bool isFull = false;
    RECT winSize;
    GetWindowRect(handle, &winSize);
    isFull =  (winSize.right - winSize.left == GetSystemMetrics(SM_CXSCREEN)) &&
              (winSize.bottom - winSize.top == GetSystemMetrics(SM_CYSCREEN));


    qDebug() << winSize.right - winSize.left << " " << winSize.bottom - winSize.top << endl;
    qDebug() << "window check: " << isFull << endl;
    //CloseHandle(handle);
    return !isFull;
}
#endif
