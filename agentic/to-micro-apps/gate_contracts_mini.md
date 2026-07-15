# GET /futures/{settle}/contracts

## Description
Get a list of all futures contracts.

## Parameters
- settle (path, required): btc or usdt
- limit (query, optional): maximum number of records
- offset (query, optional): offset

## Base URL
https://api.gateio.ws/api/v4

## Example Response
[
  {"name": "BTC_USDT", "type": "direct", "leverage_min": "1", "leverage_max": "100", "status": "trading"}
]

## Key Fields
- name: contract name
- type: direct or inverse
- leverage_min/max: credit multiplier
- status: trading, delisting, delisted
