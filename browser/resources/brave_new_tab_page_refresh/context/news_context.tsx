/* Copyright (c) 2025 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

import * as React from 'react'

import { loadTimeData } from '$web-common/loadTimeData'
import usePromise from '$web-common/usePromise'

const newsFeatureEnabled = loadTimeData.getBoolean('newsFeatureEnabled')

export function NewsProvider(props: React.PropsWithChildren) {
  const { result: Context } = usePromise(
    () =>
      newsFeatureEnabled
        ? import(
            '../../../../components/brave_news/browser/resources/shared/Context'
          ).then((m) => m.BraveNewsContextProvider)
        : Promise.resolve(null),
    [],
  )
  return Context ? <Context>{props.children}</Context> : <>{props.children}</>
}
