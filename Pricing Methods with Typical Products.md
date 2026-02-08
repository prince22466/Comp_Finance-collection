# Derivative Pricing Methods → Typical Products

Compact practitioner mapping between numerical method families and the types of derivatives they are best suited for.

---

## Summary Table

| Pricing Method | Best When (Problem Structure) | Typical Derivatives / Products | Notes |
|--------------|------------------------------|-------------------------------|-------|
| **Closed-form (analytic)** | 1 risk factor, European payoff, no path dependence, known distribution | European calls/puts, digitals/binaries, forwards/futures, geometric Asians, simple FX options, simple bond options | Fastest, exact, analytic Greeks, ideal for calibration baselines |
| **PDE / FDM (finite differences, incl. trees)** | ≤ 2 state variables, early exercise, barriers, boundary features | American options, Bermudan swaptions, barriers (KO/KI/double), callable/putable bonds, convertibles, local-vol exotics, many structured notes | Deterministic runtime, smooth Greeks, handles free boundary (optimal stopping) |
| **Monte Carlo (MC / LSMC)** | Path dependence or many risk factors (3+), baskets, hybrids, exposures | Arithmetic Asians, lookbacks, cliquets, basket/rainbow/worst-of, callable exotics (Longstaff–Schwartz), CVA/DVA/FVA/MVA, hybrid equity-rates-credit trades | Dimension scales well, easy to parallelize/GPU, statistical noise |
| **FFT / Fourier methods** | Characteristic function known, need many strikes or fast surface pricing | Vanilla surfaces under Heston/Bates/jump models, variance swaps, spread options (convolution), calibration engines | Very fast for strike grids, complexity ~ O(N log N) |

---

## Quick Selection Rule

- European only → **Closed-form**
- Early exercise or barriers (low dimension) → **PDE/FDM**
- Path-dependent or high dimension → **Monte Carlo**
- Many strikes / calibration → **FFT**

---

## Complexity Intuition

| Method | Typical Complexity | Parallelism | Greeks Quality |
|--------|------------------|-------------|----------------|
| Closed-form | O(1) | trivial | analytic (exact) |
| PDE/FDM | O(N_S × N_t) | limited/moderate | smooth |
| Monte Carlo | O(N_paths × N_steps) | excellent (GPU/cluster) | noisy (variance reduction needed) |
| FFT | O(N log N) | good | smooth |

---

## Practical Desk Guidance

- **Equity vanilla desk** → Closed-form + FFT calibration  
- **American/convertible desk** → PDE/FDM  
- **Structured/basket/XVA desk** → Monte Carlo  
- **Vol surface calibration** → FFT  

---

**Rule of thumb:**  
> Low dimension → grid (PDE)  
> High dimension → simulation (MC)
