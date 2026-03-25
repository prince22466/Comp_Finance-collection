The project is to show the Discontinuity cases using MC and FDM for pricing exotics.


base model: bs SDE:
the value V(t, S) satisfies:
∂V/∂t + r S ∂V/∂S + 1/2 σ² S² ∂²V/∂S² − rV  = 0, 
with r=0(assuming no discounting effect for simplicity),
it becomes ∂V/∂t + 1/2 σ² S² ∂²V/∂S² = 0, and this is used in FDM2D and Barrier pricing.

subprojects:
1. Barrier(Down-and-out call)
    CN for 2D PDE, to observe:
    Oscillations near strike at t near T,
    Rannacher = implicit damping of initial kink
    Grid refinement near strike helps
    Greeks stability

2. MC for digital call

base model:
black scholes