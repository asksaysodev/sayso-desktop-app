/*****************************************************************************
*
* Copyright (C) 2024, Zoom Video Communications, Inc
*
*****************************************************************************/
#ifndef _ZUI_WINDOW_H_
#define _ZUI_WINDOW_H_

#include "zUI/view.h"

namespace zui {
  class IZUIWindowEventSink
  {
  public:
#ifdef _WIN32
    virtual bool IsLayeredWndStyleInNeed() = 0;
    virtual void GetWndStyleInNeed(bool& layered, bool& transparent) = 0;
#endif
  };

  class ZUI_API ZUIWindow : public std::enable_shared_from_this<ZUIWindow>
  {
  public:
    struct WindowData
    {
      zui::WindowType type = zui::WindowType::Modal;
      std::string title = std::string("ZUI Window");
      Layout::Size size = Layout::Size(NAN, NAN);
      Layout::Position position = Layout::Position(NAN, NAN);
      void* parent = nullptr;
      size_t id = 0;
    };
    
    enum class DataUpdate {
        Size,
        Title,
        Position,
    };
    
  public:
    ZUIWindow() = delete;
    ZUIWindow(ZUIWindow::WindowData windowData);
    ZUIWindow(std::string customID, ZUIWindow::WindowData windowData);
    ZUIWindow(std::string customID, zui::WindowType type, void* parent = nullptr);
    ZUIWindow(zui::WindowType type, void* parent = nullptr);
    ZUIWindow(const View& view, zui::WindowType type, void* parent = nullptr);
    ZUIWindow(std::string customID, const View& view, zui::WindowType type, void* parent = nullptr);
    ~ZUIWindow();
    
    void ShowWindow();
    void HideWindow();
    void CenterWIndow();
    
    void AttachView(const View& view);
    void DetachView();
    
    void Size(const Layout::Size& windowSize);
    void Title(const std::string& windowTitle);
    void Position(const Layout::Position& position);
    
    void SetOnActive(std::function<void(bool)> callback);
    void WindowActive(bool active);
    
    WindowData GetWindowData();
    void SetCustomID(std::string customID);
    
    std::string GetCustomID();
    size_t GetZUIWindowID();
    void SetRegistered(bool registered);

  private:
    static unsigned int s_windowCounter;
    
  private:
    void InitWindowData(zui::WindowType type);
    
    WindowData _windowData;
    std::string _customID;
    bool _envRegistered = false;
    
    std::shared_ptr<ViewNode> viewNode;
    std::function<void(bool)> _onActiveCallback;
  };
}

#endif
