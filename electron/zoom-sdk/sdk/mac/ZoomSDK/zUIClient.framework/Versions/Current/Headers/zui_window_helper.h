#pragma once
#include "zUI/zui_base.h"
#include "zUI/environment.h"

namespace zui
{
  static std::shared_ptr<ZUIWindow> CreateZUIWindow(std::string customID, ZUIWindow::WindowData& windowData)
  {
    return zui::gEnvironment.CreateZUIWindow(customID, windowData);
  }

  static std::shared_ptr<ZUIWindow> CreateZUIWindow(std::string customID, zui::WindowType type)
  {
    return zui::gEnvironment.CreateZUIWindow(customID, type);
  }

  static std::shared_ptr<ZUIWindow> CreateZUIWindow(std::string customID, const View& view, zui::WindowType type)
  {
    return zui::gEnvironment.CreateZUIWindow(customID, view, type);
  }

  static std::shared_ptr<ZUIWindow> CreateZUIWindow(ZUIWindow::WindowData& windowData)
  {
    return zui::gEnvironment.CreateZUIWindow(windowData);
  }

  static std::shared_ptr<ZUIWindow> CreateZUIWindow(zui::WindowType type)
  {
    return zui::gEnvironment.CreateZUIWindow(type);
  }

  static std::shared_ptr<ZUIWindow> CreateZUIWindow(const View& view, zui::WindowType type)
  {
    return zui::gEnvironment.CreateZUIWindow(view, type);
  }

  static void ShowWindow(size_t windowID)
  {
    zui::gEnvironment.ShowWindow(windowID);
  }
  static void HideWindow(size_t windowID)
  {
    zui::gEnvironment.HideWindow(windowID);
  }
  static void CenterWindow(size_t windowID)
  {
    zui::gEnvironment.CenterWindow(windowID);
  }
  static void UpdateWindowSize(size_t windowID, const Layout::Size& windowSize)
  {
    zui::gEnvironment.UpdateWindowSize(windowID, windowSize);
  }
  static void UpdateWindowTitle(size_t windowID, const std::string& windowTitle)
  {
    zui::gEnvironment.UpdateWindowTitle(windowID, windowTitle);
  }
  static void UpdateWindowPosition(size_t windowID, const Layout::Position& windowPosition)
  {
    zui::gEnvironment.UpdateWindowPosition(windowID, windowPosition);
  }
  static void SetWindowCustomID(size_t windowID, std::string customID)
  {
    zui::gEnvironment.SetWindowCustomID(windowID, customID);
  }

  static void ShowWindow(std::string customID)
  {
    zui::gEnvironment.ShowWindow(customID);
  }
  static void HideWindow(std::string customID)
  {
    zui::gEnvironment.HideWindow(customID);
  }
  static void CenterWindow(std::string customID)
  {
    zui::gEnvironment.CenterWindow(customID);
  }
  static void UpdateWindowSize(std::string customID, const Layout::Size& windowSize)
  {
    zui::gEnvironment.UpdateWindowSize(customID, windowSize);
  }
  static void UpdateWindowTitle(std::string customID, const std::string& windowTitle)
  {
    zui::gEnvironment.UpdateWindowTitle(customID, windowTitle);
  }
  static void UpdateWindowPosition(std::string customID, const Layout::Position& windowPosition)
  {
    zui::gEnvironment.UpdateWindowPosition(customID, windowPosition);
  }

  static void AttachViewToWindow(std::string customID, const View& view)
  {
    zui::gEnvironment.AttachViewToWindow(customID, view);
  }
  static void AttachViewToWindow(size_t windowID, const View& view)
  {
    zui::gEnvironment.AttachViewToWindow(windowID, view);
  }

  static void DetachViewFromWindow(std::string customID)
  {
    zui::gEnvironment.DetachViewFromWindow(customID);
  }
  static void DetachViewFromWindow(size_t windowID)
  {
    zui::gEnvironment.DetachViewFromWindow(windowID);
  }

  static void RegisterWindow(std::shared_ptr<ZUIWindow> window)
  {
    zui::gEnvironment.RegisterWindow(window);
  }

  static void SetWindowOnActive(size_t windowID, std::function<void(bool)> callback)
  {
    zui::gEnvironment.SetWindowOnActive(windowID, callback);
  }
  static void SetWindowOnActive(std::string customID, std::function<void(bool)> callback)
  {
    zui::gEnvironment.SetWindowOnActive(customID, callback);
  }
}
