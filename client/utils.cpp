#include "utils.hpp"

namespace utils {

QString appHome; //家目录
QString tempDir; //临时文件夹
/** 设置项*/
QSharedPointer<SettingsSet> settings;
QTranslator ndr_tr, qt_tr;
ResourceManager resourceManager;
QSharedPointer<QAESEncryption> encryption;
QMap<QString, QString> const &getDrModelId() {
  static QMap<QString, QString> const drModelId = {
      {QString{QObject::tr("考试专用")}, QString{""}},
      {QString{QObject::tr("教育网2M")}, QString{"@edu"}},
      {QString{QObject::tr("联通合约20M")}, QString{"@cnc"}},
      {QString{QObject::tr("联通合约30M")}, QString{"@cnc30a"}},
      {QString{QObject::tr("联通合约50M")}, QString{"@cnc50a"}},
      {QString{QObject::tr("电信10M")}, QString{"@ct10"}},
      {QString{QObject::tr("电信合约10M")}, QString{"@ct10a"}}};
  return drModelId;
}

QStringList getDrModelPostfixTable() { return getDrModelId().keys(); }

QString getDrModel(QString const &postfix) { return getDrModelId()[postfix]; }

QVariant getDrModelCaption(QString const &postfix) {
  qDebug() << "postfix:" << postfix;
  auto drModelId = getDrModelId();
  if (drModelId.find(postfix) == drModelId.end()) {
    return QVariant{};
  }
  return QVariant{postfix};
}

QString getLangDir() {
  qDebug();
  QString r = ":/translate/" LANGUAGE_DIR_NAME;
  return QFile::exists(r) ? r : QString{};
}

QString getVersionString() {
  Q_STATIC_ASSERT(NDR_MINOR_VERSION < 100);
  return QString{"%1.%2"}.arg(NDR_MAJOR_VERSION).arg(NDR_MINOR_VERSION);
}

QStringList getLangFileNameTable() {
  static QString lang_dir = getLangDir();
  qDebug() << "lang_dir" << lang_dir;
  if (lang_dir.isEmpty())
    return QStringList();
  QDir dir(lang_dir);
  QStringList filter("ndr.*.qm");
  QStringList filelist =
      dir.entryList(filter, QDir::Files | QDir::CaseSensitive);
  qDebug() << "filelist" << filelist;
  if (filelist.isEmpty())
    return filelist;
  filelist.replaceInStrings("ndr.", QString());
  filelist.replaceInStrings(".qm", QString());
  return filelist;
}

QVariant getLanguageName(QString const &fileName) {
  qDebug();
  static QMap<QString, QString> table{{"en_US", "English"},
                                      {"ja_JP", "日本語"},
                                      {"ko_KR", "한국의"},
                                      {"zh_CN", "简体中文"}};

  if (table.find(fileName) == table.end()) {
    return QVariant{};
  }
  return QVariant{table[fileName]};
}

void initAppHome() {
  QDir home = QDir::home();
  appHome = home.path() + "/.nssdr";
  if (!QFile::exists(appHome)) {
    home.mkdir(".nssdr");
  }
  qDebug() << appHome;
}

void initTempDir() {
  QDir temp = QDir::temp();
  tempDir = temp.path() + "/ndr";
  if (!QFile::exists(tempDir)) {
    temp.mkdir("ndr");
  }
  qDebug() << tempDir;
}

void initSettingsSet() {
  settings.reset(new SettingsSet{appHome + "/config.ini"});
}

void initLanguage() {
  QString lang_dir = utils::getLangDir();

  qDebug() << "lang_dir" << lang_dir;
  qDebug() << "language" << utils::settings->language;
  QString current_locale = utils::settings->language;
  if (current_locale.isEmpty())
    current_locale = QLocale::system().name();
  qDebug() << "current_locale" << current_locale;
  if (!ndr_tr.load("ndr." + current_locale + ".qm", lang_dir)) {
    qDebug() << "Load Qm file error";
  }

  qt_tr.load("qt_" + current_locale + ".qm",
             QLibraryInfo::location(QLibraryInfo::TranslationsPath));
  qApp->installTranslator(&ndr_tr);
  qApp->installTranslator(&qt_tr);
}

void initFont(QString const &fontFile) {
  int fontId = QFontDatabase::addApplicationFont(fontFile);
  qDebug() << QString("font id:%1\n").arg(fontId);
  if (fontId >= 0) {
    QString msyh = QFontDatabase::applicationFontFamilies(fontId).at(0);
    QFont font(msyh, 10);
    //将此字体设为QApplication的默认字体
    QApplication::setFont(font);
  }
}

void initStyleSheet(NdrApplication *app, QString const &sheetFile) {
  // QString sheetName = "default";
  QFile file(sheetFile);
  file.open(QFile::ReadOnly);
  QString styleSheet = QLatin1String(file.readAll());
#if defined Q_OS_WIN || defined Q_OS_MAC
  QString appath = app->applicationDirPath();
#ifdef Q_OS_WIN
  styleSheet.replace("$[SKIN_PATH]", appath + "/skin");
#else
  styleSheet.replace("$[SKIN_PATH]", appath + "/../Resources/skin");
#endif
#else
  styleSheet.replace("$[SKIN_PATH]", "/usr/share/ndr-client/skin");
#endif
  qDebug() << styleSheet;
  app->setStyleSheet(styleSheet);
}
} // namespace utils
