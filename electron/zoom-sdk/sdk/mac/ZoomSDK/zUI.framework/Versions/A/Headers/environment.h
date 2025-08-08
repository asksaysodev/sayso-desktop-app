#ifndef __ZUI_ENVIRONMENT_H__
#define __ZUI_ENVIRONMENT_H__

#include <map>
#include <memory>
#include <vector>
#include "model/model.h"
#include "view.h"
#include "zui_data_define.h"
#include "window.h"

namespace zui {

  class App;

  struct ZUI_API Variant
  {
    enum class DataType
    {
      kInt,
      kFloat,
      kString,
      kFunc,
    };
    DataType type;

    union
    {
      Published<int> i;
      Published<float> f;
      std::string s;
      std::function<void(const std::string&)> func;
    };

    Variant() : type(DataType::kInt), i() {}
    Variant(int val) : type(DataType::kInt), i(val) {}
    Variant(float val) : type(DataType::kFloat), f(val) {}
    Variant(const std::string& val) : type(DataType::kString) {
      new(&s)std::string(val);
    }
    Variant(const std::function<void(const std::string&)>& f) : type(DataType::kFunc), func(f) {}

    ~Variant()
    {
      switch (type)
      {
      case DataType::kInt:
        i.~Published();
        break;
      case DataType::kFloat:
        f.~Published();
        break;
      case DataType::kString:
      {
        using std::string;
        s.std::string::~string();
      }
        break;
      case DataType::kFunc:
        func.~function<void(const std::string&)>();
        break;
      }
    }

    Variant(const Variant& var)
    {
      type = var.type;
      switch (type)
      {
      case DataType::kInt:
        i = var.i;
        break;
      case DataType::kFloat:
        f = var.f;
        break;
      case DataType::kString:
        new(&s)std::string(var.s);
        break;
      case DataType::kFunc:
        func = var.func;
        break;
      }
    }

    Variant& operator=(const Variant& var)
    {
      type = var.type;
      switch (type)
      {
      case DataType::kInt:
        i = var.i;
        break;
      case DataType::kFloat:
        f = var.f;
        break;
      case DataType::kString:
        new(&s)std::string(var.s);
        break;
      case DataType::kFunc:
        new(&func)std::function<void(const std::string&)>(var.func);
        break;
      }
      return *this;
    }
  };

  class ZUI_API Environment
  {
  public:
    static Environment& Instance();

    Environment();
    ~Environment();

    void SetApp(std::shared_ptr<App> app);
    std::shared_ptr<App> GetApp() { return _app; }

    void Insert(const std::string& key, const Variant& val);

    // view func
    void InsertView(const std::string& viewId, std::shared_ptr<View> view);
	  void Replace(const std::string& viewId, std::shared_ptr<View> view);
    void RemoveView(const std::string& viewId);
    void RemoveView(std::shared_ptr<View> view);
    void CleanUpView(std::shared_ptr<View> view);
    bool ContainView(const std::string& viewId);
    std::shared_ptr<View> GetView(const std::string& viewId);

    // viewNode func
    void InsertRootViewNode(std::shared_ptr<ViewNode> root);
    void RemoveRootViewNode(std::shared_ptr<ViewNode> root);
    void RemoveRootViewNode(const std::string& surfaceId);
    void RemoveAllRootViewNode();
    std::vector<std::shared_ptr<ViewNode>> GetAllRootNode();
    std::shared_ptr<ViewNode> GetRootNode(const std::string& surfaceId);
    std::shared_ptr<ViewNode> GetInternalWindowView(size_t winId);

    void SetEngine(void* engine) { _engine = engine; }
    void* GetEngine() { return _engine; }

    Published<int>& GetInt(const std::string& key);
    Published<float>& GetFloat(const std::string& key);
    std::string& GetString(const std::string& key);
    std::function<void(const std::string&)>& GetFunc(const std::string& key);

    std::string CreateViewId();
    void ScrollToView(const std::string& viewId, const std::string& highlightColor);
    void ScrollToTop(const std::string& scrollViewId);
    
    std::shared_ptr<ZUIWindow> CreateZUIWindow(std::string customID, ZUIWindow::WindowData& windowData);
    std::shared_ptr<ZUIWindow> CreateZUIWindow(std::string customID, zui::WindowType type);
    std::shared_ptr<ZUIWindow> CreateZUIWindow(std::string customID, const View& view, zui::WindowType type);
    
    std::shared_ptr<ZUIWindow> CreateZUIWindow(ZUIWindow::WindowData& windowData);
    std::shared_ptr<ZUIWindow> CreateZUIWindow(zui::WindowType type);
    std::shared_ptr<ZUIWindow> CreateZUIWindow(const View& view, zui::WindowType type);

    void ShowWindow(size_t windowID);
    void HideWindow(size_t windowID);
    void CenterWindow(size_t windowID);
    void UpdateWindowSize(size_t windowID, const Layout::Size& windowSize);
    void UpdateWindowTitle(size_t windowID, const std::string& windowTitle);
    void UpdateWindowPosition(size_t windowID, const Layout::Position& windowPosition);
    void SetWindowCustomID(size_t windowID, std::string customID);
    
    void ShowWindow(std::string customID);
    void HideWindow(std::string customID);
    void CenterWindow(std::string customID);
    void UpdateWindowSize(std::string customID, const Layout::Size& windowSize);
    void UpdateWindowTitle(std::string customID, const std::string& windowTitle);
    void UpdateWindowPosition(std::string customID, const Layout::Position& windowPosition);
    void SetZUIWindowEventSink(size_t windowID, IZUIWindowEventSink* sink);
    
    void AttachViewToWindow(std::string customID, const View& view);
    void AttachViewToWindow(size_t windowID, const View& view);
    
    void DetachViewFromWindow(std::string customID);
    void DetachViewFromWindow(size_t windowID);
    
    void RegisterWindow(std::shared_ptr<ZUIWindow> window);
    void WindowOnActive(size_t windowID, bool isActive);
    void SetWindowOnActive(size_t windowID, std::function<void(bool)> callback);
    void SetWindowOnActive(std::string customID, std::function<void(bool)> callback);

  private:
    bool isVail(const std::string& key, Variant::DataType dataType);

  public:
    static const std::string openWindow;
    static const std::string closeWindow;
    static const std::string destoryApp;
    static const std::string openUrl;
    static const std::string clientCMMLanguage;
    static const std::string openSystemNotifications;

  private:
    std::map<std::string, Variant> _data;
    std::shared_ptr<App> _app;
    std::map<std::string, std::shared_ptr<View>> _views;
    std::vector<std::shared_ptr<ViewNode>> _rootViewNodeMap;
    std::map<size_t, std::weak_ptr<ZUIWindow>> _zuiWindows;
    std::map<std::string, size_t> _zuiWindowIDMap;
    void* _engine = nullptr; // for linux
    int _viewId = 0;
  };
  static Environment& gEnvironment = Environment::Instance();

}
#endif
