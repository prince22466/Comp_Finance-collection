# Prepayment Demo (IRRBB Context)

This repository contains a single notebook, `demo.ipynb`, that demonstrates mortgage prepayment modeling and its cashflow impact at pool level over a 10-month horizon.

## What The Notebook Covers

The notebook has two parts:

1. Constant market rate scenario  
   - Uses a fixed market rate (`r_market = 5.4%`)
   - Simulates monthly cashflows for a pool of 10 fixed-rate mortgages
   - Uses a simple loan-level logistic prepayment model(a demo model, which in practice should be calibrated using read data)

2. Dynamic market rate scenario  
   - Simulates short-rate paths with a 1-factor Hull-White model (Monte Carlo)
   - Recomputes monthly prepayment principal per path under simulated rates

## Core Assumptions (As Implemented)

- Pool contains 10 example loans with predefined balance, rate, term, and LTV.
- Prepayment decision is loan-level and binary (`True/False`) from a logistic score.
- If prepayment happens, it is **full prepayment** of remaining balance after scheduled principal.
- Scheduled payment is standard level-payment amortization.
- Part 2 uses simulated short rates from Hull-White Euler discretization.
- Current implementation does not roll a single evolving pool state month-to-month in Part 2; each month is evaluated from the same starting pool for that path step.

## Main Functions In `demo.ipynb`

- `level_payment(...)`: monthly amortizing payment
- `prepayment_loan(...)`: logistic prepayment trigger using:
  - incentive (`r_mort - r_market`)
  - seasoning effect
  - LTV penalty for high LTV
- `generate_cashflow_with_full_prepay(...)`: loan cashflow projection with full-prepay logic
- `pool_cf(...)`: aggregates loan cashflows for the pool
- `hull_white_mc(...)`: Monte Carlo short-rate path simulation
- `monthly_prepay_path(...)`: computes monthly prepay principal by MC path
- `plot_monthly_prepay_distribution(...)`: pathwise prepayment visualization

## Notebook Requirements

Install:

```bash
pip install pandas numpy matplotlib
```
