/* Copyright (c) 2025 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_BRAVE_WALLET_BROWSER_POLKADOT_POLKADOT_TX_META_H_
#define BRAVE_COMPONENTS_BRAVE_WALLET_BROWSER_POLKADOT_POLKADOT_TX_META_H_

#include "base/numerics/safe_conversions.h"
#include "base/values.h"
#include "brave/components/brave_wallet/browser/polkadot/polkadot_extrinsic.h"
#include "brave/components/brave_wallet/browser/tx_meta.h"
#include "brave/components/brave_wallet/common/brave_wallet.mojom.h"

namespace brave_wallet {

// Polkadot transaction metadata class
class PolkadotTxMeta : public TxMeta {
 public:
  PolkadotTxMeta();
  PolkadotTxMeta(const mojom::AccountIdPtr& from,
                 const PolkadotUnsignedTransfer& extrinsic);
  ~PolkadotTxMeta() override;

  PolkadotTxMeta(const PolkadotTxMeta&) = delete;
  PolkadotTxMeta& operator=(const PolkadotTxMeta&) = delete;

  // TxMeta
  base::Value::Dict ToValue() const override;
  mojom::TransactionInfoPtr ToTransactionInfo() const override;
  mojom::CoinType GetCoinType() const override;

  static bool ValueToTxMeta(const base::Value::Dict& value,
                            PolkadotTxMeta* tx_meta);

 private:
  std::string recipient_;
  base::StrictNumeric<uint128_t> amount_ = uint128_t{0};
  bool transfer_all_ = false;
};

}  // namespace brave_wallet

#endif  // BRAVE_COMPONENTS_BRAVE_WALLET_BROWSER_POLKADOT_POLKADOT_TX_META_H_
