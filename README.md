# Options Market Making Simulator

A simple C++ simulation demonstrating how options market makers operate.

## what it does

- simulates a market maker quoting bid/ask prices for options
- tracks positions and p&l as stock price moves
- shows risk management through inventory-based price adjustments
- runs a 10-day simulation with random customer orders

## how to run

```bash
g++ -o options_sim main.cpp
./options_sim
```

## features

- 6 options: 3 calls (95, 100, 105 strike) and 3 puts (95, 100, 105 strike)
- $2 bid-ask spread for profit
- position-based price adjustments
- random stock price movements
- portfolio value tracking

## output

shows daily trading activity, position updates, and final portfolio performance. 