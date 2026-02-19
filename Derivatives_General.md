**Option(Vanilla and some Exotics)

| Product | Path dependent | Early exercise | Continuous payoff function | Typical pricing method |
|---|---|---|---|---|
| European call | No (terminal only) | No | Yes (continuous, kink at strike) | Closed-form (Black–Scholes) |
| American put | Not path dependent in payoff; valuation depends on early-exercise decision | Yes | Yes (continuous, kink at strike) | Tree / PDE-FDM; MC with LSM |
| Digital (cash-or-nothing) | No (terminal only) | No (typically European) | No (discontinuous at strike) | Closed form (BS); replication via vanillas |
| Barrier knock-out | Yes (barrier hit event) | No (standard barriers European) | No (discontinuous around barrier / event) | PDE-FDM; MC with Brownian bridge (bias correction); FFT in some models |
| Bermudan swaption | Payoff not path dependent; valuation depends on optimal stopping across dates | Yes (discrete dates) | Yes (continuous, kink at exercise boundary) | Tree / lattice (short-rate); LSM MC; PDE (low dimension) |
| Cliquet | Yes (sequence of returns with caps/floors) | No (standard versions) | Generally continuous but piecewise / non-smooth | Monte Carlo; PDE (low dimension); convolution methods in some models |



