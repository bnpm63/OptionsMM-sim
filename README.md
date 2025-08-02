# Options Market Making Simulator

Lightweight options market making simulator with dynamic pricing algorithms, inventory risk management, and real-time P&L tracking. Implements simplified option valuation with synthetic market data and automated order processing.

## Features

- Simulates a market maker quoting bid/ask prices for options
- Tracks positions and p&l as stock price moves
- Shows risk management through inventory-based price adjustments
- Runs a 10-day simulation with random customer orders
- 6 options: 3 calls (95, 100, 105 strike) and 3 puts (95, 100, 105 strike)
- $2 bid-ask spread for profit
- Position-based price adjustments
- Random stock price movements

## Execution

```bash
g++ -o options_sim main.cpp
./options_sim
```

## output

shows daily trading activity, position updates, and final portfolio performance. 