/* Copyright (c) 2025 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_wallet/browser/polkadot/polkadot_tx_meta.h"

#include "base/strings/string_number_conversions.h"
#include "base/values.h"
#include "brave/components/brave_wallet/browser/brave_wallet_utils.h"
#include "brave/components/brave_wallet/common/brave_wallet.mojom.h"

namespace brave_wallet {

PolkadotTxMeta::PolkadotTxMeta() = default;

PolkadotTxMeta::PolkadotTxMeta(const mojom::AccountIdPtr& from,
                               const PolkadotUnsignedTransfer& extrinsic)
    : recipient_(base::HexEncodeLower(extrinsic.recipient())) {
  set_from(from.Clone());
  amount_ = extrinsic.send_amount();
}

PolkadotTxMeta::~PolkadotTxMeta() = default;

base::Value::Dict PolkadotTxMeta::ToValue() const {
  auto dict = TxMeta::ToValue();

  uint128_t amount = amount_;
  dict.Set("amount", base::HexEncodeLower(base::byte_span_from_ref(amount)));
  dict.Set("recipient", recipient_);
  dict.Set("transfer_all", transfer_all_);
  return dict;
}

uint128_t PolkadotTxMeta::GetSendAmount() const {
  return amount_;
}

std::string PolkadotTxMeta::GetRecipient() const {
  return recipient_;
}

// static
bool PolkadotTxMeta::ValueToTxMeta(const base::Value::Dict& value,
                                   PolkadotTxMeta* tx_meta) {
  uint128_t amount = 0;
  bool transfer_all = false;

  auto* json_amount = value.FindString("amount");
  if (!json_amount) {
    return false;
  }

  if (!base::HexStringToSpan(*json_amount, base::byte_span_from_ref(amount))) {
    return false;
  }

  auto json_transfer_all = value.FindBool("transfer_all");
  if (!json_transfer_all) {
    return false;
  }

  transfer_all = *json_transfer_all;

  auto* json_recipient = value.FindString("recipient");
  if (!json_recipient) {
    return false;
  }

  tx_meta->amount_ = amount;
  tx_meta->recipient_ = *json_recipient;
  tx_meta->transfer_all_ = transfer_all;

  return true;
}

mojom::TransactionInfoPtr PolkadotTxMeta::ToTransactionInfo() const {
  return mojom::TransactionInfo::New(
      id_, from_.Clone(), tx_hash_,
      mojom::TxDataUnion::NewPolkadotTxData(mojom::PolkadotTxdata::New(
          recipient_,
          mojom::uint128::New(amount_ >> 64, amount_ & 0xffffffffffffffff),
          false, 0)),
      status_, mojom::TransactionType::Other,
      std::vector<std::string>() /* tx_params */,
      std::vector<std::string>() /* tx_args */,
      base::Milliseconds(created_time_.InMillisecondsSinceUnixEpoch()),
      base::Milliseconds(submitted_time_.InMillisecondsSinceUnixEpoch()),
      base::Milliseconds(confirmed_time_.InMillisecondsSinceUnixEpoch()),
      origin_.has_value() ? MakeOriginInfo(*origin_) : nullptr, chain_id_, "",
      false, nullptr);
}

mojom::CoinType PolkadotTxMeta::GetCoinType() const {
  return mojom::CoinType::DOT;
}

}  // namespace brave_wallet
