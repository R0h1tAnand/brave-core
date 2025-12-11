/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/browser/ui/webui/brave_web_ui_controller_factory.h"

// Needed since we define CHROME_BROWSER_WEB_APPLICATIONS_WEB_APP_PROVIDER_H_
// below
#include "chrome/browser/web_applications/web_app_provider.h"

#if !BUILDFLAG(IS_ANDROID)
#include "brave/grit/brave_theme_resources.h"
#include "chrome/common/webui_url_constants.h"
#include "ui/base/resource/resource_bundle.h"
#include "url/gurl.h"
#endif

namespace {

base::RefCountedMemory* GetBraveFaviconResourceBytes(
    Profile* profile,
    const GURL& page_url,
    ui::ResourceScaleFactor scale_factor) {
#if BUILDFLAG(IS_ANDROID)
  return nullptr;
#else
  if (page_url.host() != chrome::kChromeUINewTabHost) {
    return nullptr;
  }
  // TODO: Use IDR_BRAVE_NTP_FAVICON_DARK if we are in a dark theme.
  return static_cast<base::RefCountedMemory*>(
      ui::ResourceBundle::GetSharedInstance().LoadDataResourceBytesForScale(
          IDR_BRAVE_NTP_FAVICON_LIGHT, scale_factor));
#endif
}

}  // namespace

// NOLINTNEXTLINE
#define CHROME_BROWSER_WEB_APPLICATIONS_SYSTEM_WEB_APPS_SYSTEM_WEB_APP_MANAGER_H_
#define CHROME_BROWSER_WEB_APPLICATIONS_WEB_APP_PROVIDER_H_

#define BRAVE_CHROME_WEBUI_CONTROLLER_FACTORY \
  return BraveWebUIControllerFactory::GetInstance();

// `ChromeWebUIControllerFactory::GetFaviconResourceBytes` loads favicon data
// for WebUIs based upon the WebUI URL. (Add additional comments.)
#define BRAVE_CHROME_WEBUI_CONTROLLER_FACTORY_GET_FAVICON_FOR_URL \
  GetBraveFaviconResourceBytes(profile, url, selected_resource_scale)); \
  if (!bitmap.get()) bitmap = (

#include <chrome/browser/ui/webui/chrome_web_ui_controller_factory.cc>

#undef BRAVE_CHROME_WEBUI_CONTROLLER_FACTORY_GET_FAVICON_FOR_URL
#undef BRAVE_CHROME_WEBUI_CONTROLLER_FACTORY
#undef CHROME_BROWSER_WEB_APPLICATIONS_WEB_APP_PROVIDER_H_
#undef CHROME_BROWSER_WEB_APPLICATIONS_SYSTEM_WEB_APPS_SYSTEM_WEB_APP_MANAGER_H_
