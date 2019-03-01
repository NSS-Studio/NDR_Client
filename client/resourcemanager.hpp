#pragma once

#include <QMutex>
#include <QSharedPointer>

class PPPoE;
class MainWindow;
class LoginDialog;
class LocalStorage;
class PopUpInfomationDialog;
class AboutDialog;
class ResourceManager final {
public:
  ResourceManager() = default;
  ~ResourceManager();
  ResourceManager(ResourceManager const &) = delete;
  ResourceManager(ResourceManager &&) = delete;
  void operator=(ResourceManager const &) = delete;
  void InitResourceManager() noexcept;
  PPPoE *getPPPoE() const noexcept;
  LoginDialog *getLoginDialog() const noexcept;
  PopUpInfomationDialog *getPopUpInfomationDialog() const noexcept;
  AboutDialog *getAboutDialog() const noexcept;
  LocalStorage *getProfile() const noexcept;

private:
  PPPoE *pppoe = nullptr;
  MainWindow *mainWindow = nullptr;
  LocalStorage *profile = nullptr;
  LoginDialog *loginDialog = nullptr;
  AboutDialog *aboutDialog = nullptr;
  PopUpInfomationDialog *popUpInfomationDialog = nullptr;
  QMutex mutex;
  enum class InitState { DONE, UNDONE };
  InitState initState = InitState::UNDONE;
};
