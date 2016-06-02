#include "updateservice.h"

UpdateService::UpdateService(const QString &serverAddr, const QString &serverAddrBack, const QString &tempDirectory, QObject *parent) :
    QObject(parent)
{
    this->tempDir = tempDirectory;
    this->ipAddress = serverAddr;
    this->ipAddress_2_back = serverAddrBack;
    this->running = false;
    qDebug() << "tempDir" << tempDirectory;
    qDebug() << "ipAddress" << serverAddr;
}

void UpdateService::checkUpdate()
{
    if(running)
        return;
    QString url="http://" + ipAddress + "/update/origin.xml";
    qDebug() << "url" << url;
    reply = nam.get(QNetworkRequest( QUrl(url)));
    connect(reply,SIGNAL(finished()),this,SLOT(checkOriginGet()));
    this->running=true;
}

/*服务器检查，状况良好则跳过*/
void UpdateService::checkOriginGet()
{
    if(reply->error() != QNetworkReply::NoError)
    {
        qDebug() << "url_1 failed";
        QString url="http://" + ipAddress_2_back + "/update/origin.xml";
        qDebug() << "url_2_back" << url;
        reply = nam.get(QNetworkRequest( QUrl(url)));
        connect(reply,SIGNAL(finished()),this,SLOT(originGetFinished_back()));
        this->isConnectUpdateServerFail = false;
    }
    else{
        connect(reply,SIGNAL(finished()),this,SLOT(originGetFinished()));
        this->isConnectUpdateServerFail = true;
    }
}

/*
bool UpdateService::tryInstall()
{

}*/

#define ERROR_UNKNOW_FORMAT "格式错误"
void UpdateService::originGetFinished()
{
    
    qDebug() << "getFinished() enter";
    QString errorMessage;
    if(reply->error() != QNetworkReply::NoError)
    {
        qDebug() << "reply->errorString()" << reply->errorString();
        running = false;
        emit checkFinished(true,0,0,tr("连接远程更新服务器失败，%1").arg(reply->errorString()));
        return ;
    }
    QString stateValue = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toString();
    
    qDebug() << "Http state value" << stateValue;
    if(stateValue!="200")
    {
        running = false;
        emit checkFinished(true,0,0,tr("远程更新服务器返回错误状态值 %1").arg(stateValue));
        return;
    }
	QByteArray xml;

	xml=reply->readAll();
    qDebug() << "xml string:\n" << xml;
    
    QDomDocument doc;
    if(doc.setContent(xml,true))
	{

		QDomElement root = doc.documentElement();
		QDomNodeList nodes;
        nodes = root.elementsByTagName("package");
        //nodes = root.elementsByTagName()
		if(nodes.isEmpty()){
			errorMessage = tr("在推送的更新配置文件中找不到包");
            //errorMessage = tr(ERROR_UNKNOW_FORMAT);
        	}
        QDomElement packageElement;
        qDebug() << "OS" << APP_OS_STR;
        qDebug() << "arch" << APP_ARCH_STR;
        for(int i=0;i<nodes.count();i++)
        {
            QDomElement node=nodes.at(i).toElement();
            if(node.attribute("OS") == APP_OS_STR
                    && node.attribute("arch") == APP_ARCH_STR)
            {
                packageElement = node;
                break;
            }
        }
        if(!packageElement.isNull())
        {
            qDebug() << "Update package config found";
            if(packageElement.attribute("enable")=="true")
            {
                int major,minor;
                major = packageElement.attribute("major").toInt();
                minor = packageElement.attribute("minor").toInt();
                packageFilename = packageElement.attribute("fname","ndr-setup");
                this->packageUrl = packageElement.text();
                if ( true == this->isConnectUpdateServerFail )
                {
                    this->packageUrl = "http://172.24.5.13/" + this->packageUrl;
                }else
                {
                    this->packageUrl = "http://172.24.10.13/" + this->packageUrl;
                }
                qDebug() << "Package url" << this->packageUrl;
                emit checkFinished(false,major,minor,"");
            }else{
                running = false;
                emit checkFinished(false,0,0,"");
            }
        }else
        {
            qDebug() << "Update package config not found";
            running = false;
            emit checkFinished(true,0,0,"找不到远程更新推送配置文件");
        }
            
    }else{
        qDebug() << "XML format error!";
        running = false;
        emit checkFinished(true,0,0,"返回的配置数据格不是标准格式，客户端无法解析.");
    }
}


void UpdateService::downloadLatestPackage()
{
    qDebug() << "UpdateService::downloadLatestPackage() enter";
    if(packageUrl.isEmpty())
    {
        qDebug() << "packageUrl is empty";
        running = false;
        emit downloadFinished(true,tr("找不到安装包资源"));
        return;
    }
    
    QString packageLocalFile = this->tempDir + "/" + packageFilename;
    qDebug() << "Local setup package file name" << packageLocalFile;
    QString errorMessage;
    if(downloadToFile(packageUrl,packageLocalFile,errorMessage))//
    {
        qDebug() << "Download setup package succeed" ;
        emit downloadFinished(false,"");
    }
    else
    {
        qDebug() << "Download setup package failed" ;
        running = false;
	emit downloadFinished(true, tr("文件下载失败") + "\n" + errorMessage);
    }
    //emit downloadFinished(true,"");
}

bool UpdateService::downloadToFile(QString urlStr, QString filename, QString &errorMsg)
{
	qDebug("function: downloadToFile()");

#ifdef Q_OS_LINUX
	QSettings package_config("/etc/ndr-package.cfg", QSettings::IniFormat);
	package_config.setIniCodec("UTF-8");
	QString current_package = package_config.value("PackageType").toString();
	if(current_package == "debian") {
		urlStr.replace("-gnu-linux-", "-");
		urlStr.replace(".tar.bz2", ".deb");
		filename.replace("-gnu-linux-", "-");
		filename.replace(".tar.bz2", ".deb");
	} else if(current_package == "redhat") {
		urlStr.replace("-gnu-linux-", "-");
		urlStr.replace(".tar.bz2", ".rpm");
		filename.replace("-gnu-linux-", "-");
		filename.replace(".tar.bz2", ".rpm");
	}
	qDebug() << "url" << urlStr;
	qDebug() << "filename" << filename;
#endif

    HttpGetEventLoop hgeloop;
    QEventLoop loop;

    QFile file(filename);

    if(file.open(QIODevice::WriteOnly|QIODevice::Truncate))
    {
        out = new QDataStream(&file);
        out->setVersion(QDataStream::Qt_4_7); 

        reply = nam.get(QNetworkRequest(QUrl(urlStr)));

        QObject::connect(reply,SIGNAL(readyRead()), this, SLOT(readyRead()), Qt::DirectConnection);
        QObject::connect(reply,SIGNAL(finished()), &loop, SLOT(quit()), Qt::DirectConnection);
        loop.exec();
        delete out;
        file.close();
	QString http_status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toString();
	if(http_status_code != "200")
        {
		errorMsg = tr("HTTP 服务器返回了意外的状态码 %1").arg(http_status_code);
		qDebug() << "StatusCode" << http_status_code;
		return false;
        }
        return true;
    }else
    {
		errorMsg = file.errorString();
		qDebug() << "Open file error:" << errorMsg;
		return false;
    }

    /*
    QUrl url(urlStr);
    http.setHost(url.host(),url.port());
    QFile file(filename);
    if(!file.open((QIODevice::WriteOnly|QIODevice::Truncate)))
        return false;
    qDebug() << "url.path()" << url.path();
    http.get(url.path(),&file);
    
    HttpGetEventLoop loop;
    bool ret;
    qDebug() << "wait for getting...";
    ret = !loop.waitForGetting(http);
    if(ret)
        qDebug() << "get error" << http.errorString();
    qDebug() << "wait over";
    //QObject::connect(http,SIGNAL(done(bool)), &loop, SLOT(quit()), Qt::DirectConnection);
    http.close();
    file.close();
    return ret;
    */
}

void UpdateService::readyRead()
{
    char buffer[1024*4];
    int realRead;
    while(reply->bytesAvailable())
    {
        realRead=reply->read(buffer,sizeof(buffer));
        out->writeRawData(buffer,realRead);
    }
}

#ifdef Q_OS_MAC
//bool UpdateService::checkInstallDirectory() {
bool UpdateService::getCurrentInstallDirectory(char *buffer) {
	// Should be "[/Applications/]NDR Client.app/Contents/MacOS"
	QDir app_dir(QApplication::applicationDirPath());
	app_dir.cdUp();		// Contents
	app_dir.cdUp();		// NDR Client.app
	//return app_dir.dirName() == "NDR Client.app";
	if(app_dir.dirName() != "NDR Client.app") return false;
	app_dir.cdUp();
	QByteArray install_dir_path = app_dir.absolutePath().toUtf8();
	size_t len = install_dir_path.length();
	if(len > PATH_MAX) return false;
	memcpy(buffer, install_dir_path.data(), len + 1);
	return true;
}
#endif

bool UpdateService::openPackage(QString &errMsg)
{
#if defined(Q_OS_WIN)
#define _WIN32_WINNT 0x0500
    QString program = packagePath();
    WCHAR szProgram[MAX_PATH];
    HINSTANCE hInstance;
    program.replace("/","\\");
    int length=program.toWCharArray(szProgram);
    szProgram[length]=0;

    hInstance = ShellExecuteW(NULL,L"open",szProgram,NULL,NULL,SW_SHOWNORMAL);

    if((DWORD)hInstance < 32)
    {
        switch((DWORD)hInstance)
        {
        case ERROR_FILE_NOT_FOUND:
			errMsg=tr("艾玛，我刚刚下载完的安装包，竟然不见了！！")+"\n"+
				tr("请尝试到e.neusoft.edu.cn下载最新版安装包");
            break;
        case SE_ERR_ACCESSDENIED:
            errMsg=tr("用户未授予客户端权限，执行进程操作被系统拒绝");
            break;
        case ERROR_BAD_FORMAT:
            errMsg=tr("下载的安装包格式错误，导致无法运行，请尝试重新拨号并下载");
            break;
        case SE_ERR_SHARE:
            errMsg=tr("打开文件时发生共享错误");
            break;
        case SE_ERR_ASSOCINCOMPLETE:
            errMsg=tr("不合法的文件路径");
            break;
        case SE_ERR_DDETIMEOUT:
            errMsg=tr("启动应用程序超时");
            break;
        case SE_ERR_NOASSOC:
            errMsg=tr("没有关联的应用程序，启动失败");
        default:
            errMsg=tr("打开文件时发生未知错误");
        }
        qDebug() << "Shell error " << (DWORD)hInstance;
        qDebug() << "Last error" << GetLastError();
        running = false;
        //QMessageBox::information(NULL,tr("错误"),tr("下载失败，")+errMsg);
        return false;
    }
    running = false;
    return true;
#elif defined(Q_OS_LINUX)
	QByteArray command_ba = QString("xdg-open %1").arg(tempDir).toUtf8();
	const char *command = command_ba.data();
	int status = system(command);
	if(status) {
        if(status == -1) {
			//perror(command);
			int e = errno;
			const char *err_msg = strerror(e);
			errMsg = QString::fromUtf8(err_msg);
			qWarning(err_msg);
            return false;
		}
		errMsg = tr("命令 %1 以状态 %2 退出").arg(QString::fromUtf8(command_ba)).arg(WEXITSTATUS(status));
		//QMessageBox::information(NULL, tr("命令 %1 失败").arg(QString::fromUtf8(command_ba)), tr("软件包已经保存到 %1，请手动安装。").arg(program));
		return false;
	}
	return true;
#elif defined(Q_OS_MAC)
	char install_dir_path[PATH_MAX + 1];
	if(!getCurrentInstallDirectory(install_dir_path)) {
		errMsg = tr("Unrecognized installation");
		return false;
	}
	QByteArray package_path = packagePath().toUtf8();
	const char *package_path_ptr = package_path.data();
	if(access(package_path_ptr, R_OK) < 0) {
		int e = errno;
		const char *err_msg = strerror(e);
		errMsg = QString::fromUtf8(err_msg);
		return false;
	}
	pid_t pid = fork();
	if(pid < 0) {
		int e = errno;
		const char *err_msg = strerror(e);
		errMsg = QString::fromUtf8(err_msg);
		//qWarning(err_msg);
		return false;
	}
	if(pid == 0) {
		chdir(install_dir_path);
		//qDebug("Unpacking \"%s\"", package_path_ptr);
		execlp("tar", "tar", "-xf", package_path_ptr, NULL);
	}
	int status;
	while(waitpid(pid, &status, 0) < 0) {
		int e = errno;
		if(e == EINTR) continue;
		const char *err_msg = strerror(e);
		errMsg = QString::fromUtf8(err_msg);
		return false;
	}
	if(WIFSIGNALED(status)) {
		errMsg = tr("解包程序因收到信号 %1 而中止").arg(WTERMSIG(status));
		return false;
	}
	if(status) {
		errMsg = tr("解包程序以状态 %1 退出").arg(WEXITSTATUS(status));
		return false;
	}
	return true;
#else
#   error OS is not supported
#endif
}

bool UpdateService::isFinished()
{
    return !running;
}

QString UpdateService::packagePath() {
	return tempDir + "/" + packageFilename;
}
