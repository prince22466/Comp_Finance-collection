**Option(Vanilla and some Exotics)

| Product | Path dependent | Early exercise | Continuous payoff function | Typical pricing method |
|---|---|---|---|---|
| European call | No (terminal only) | No | Yes (continuous, kink at strike) | Closed-form (Black–Scholes) |
| American put | Not path dependent in payoff; valuation depends on early-exercise decision | Yes | Yes (continuous, kink at strike) | Tree / PDE-FDM; Least Squares MC |
| Digital (cash-or-nothing) | No (terminal only) | No (typically European) | No (discontinuous at strike, around strike a small change of price can lead to payoff jumps from 0 → 1 and vice versa, therefore Δpayoff / Δprice is infinite.) | Closed form (BS); replication via vanillas |
| Barrier knock-out | Yes (barrier hit event) | No (standard barriers European) | No (discontinuous around barrier / event, same logic as Digital around strike) | PDE-FDM; MC with Brownian bridge (bias correction); FFT in some models |
| Bermudan swaption | Payoff not path dependent; valuation depends on optimal stopping across dates | Yes (discrete dates) | Yes (continuous, kink at exercise boundary) | Tree / lattice (short-rate); Least Squares MC; PDE (low dimension) |
| Cliquet | Yes (sequence of returns with caps/floors) | No (standard versions) | Generally continuous but piecewise / non-smooth | Monte Carlo; PDE (low dimension); convolution methods in some models |


| Asset class | Vanilla (default market model) | Vol quote convention (market standard) | Models for exotics (common) |
|---|---|---|---|
| Equity | Black–Scholes (lognormal diffusion, RN measure; calibrated to implied vol surface) | Lognormal implied vol surface **σ(K, T)**, parameterized by **strike** or **log-moneyness** *log(K/F)*; skew/smile; **delta-space quoting** common in OTC | **Local Vol (Dupire)**; **Heston** (stoch vol); **SLV** (local–stoch vol hybrid) |
| FX | **Garman–Kohlhagen** (BS with domestic & foreign discounting) | Quoted via **ATM vol**, **25Δ Risk Reversal (RR)**, **25Δ Butterfly (BF)**; delta conventions: **spot delta / forward delta / premium-adjusted delta**; typically **lognormal (Black) vol** | **SABR**; **Local Vol**; **Stochastic vol** (e.g., Heston) |
| Rates | Vanilla quoting: **Black-76** for caps/floors; swaptions in **Black (lognormal)** or **Bachelier (normal)** (esp. low/negative rates) | **Black implied vol** or **Normal (Bachelier) vol** on an **expiry × tenor** grid (not delta-based) | **LMM (Libor Market Model)**; **Hull–White** (short-rate); **SABR** for smile interpolation; multi-factor short-rate models |
| Credit | CDS pricing via **reduced-form (hazard rate / intensity)**; bootstrapped from CDS spreads | CDS options often quoted as **Black implied vol on CDS spread** (lognormal on forward spread); typically not delta-based | **Stochastic intensity** models; **copula** models for tranche products; **base correlation** framework (CDO tranches) |


