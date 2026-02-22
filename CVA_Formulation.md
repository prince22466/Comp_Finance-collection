## CVA, general formula and conputation


## general formula(discrete form)

CVA ≈ LGD ∑ᵢ DF(tᵢ) · E[E(tᵢ) · ΔPDᵢ]

- **LGD** = Loss Given Default = \(1-R\)  
-  DF(tᵢ) = discount factor  
-  E(tᵢ) = positive exposure at time \(t_i\)  
-  ΔPDᵢ= PD(t_i) - PD(t_{i-1}) = S(t_{i-1}) - S(t_i)\), the prob of default between t_i and t_(i-1)
-  E[....], meaans expection

if E(tᵢ) and ΔPDᵢ are independent(aka, no wrong-way risk),
thus, 
CVA ≈ LGD ∑ᵢ DF(tᵢ) · EE(tᵢ) · ΔPDᵢ
-  EE(tᵢ) = expected positive exposure at time \(t_i\)


## when does the portfolio have wrong-way risk exposure?

If the portfolio consists heavily of CDSs which referencing to counterparties, then:

- CDS spread drives **default probability**
- The same CDS spread drives **portfolio MtM → exposure**

Therefore exposure and default are **not independent**.

Credit deterioration simultaneously:

1. Increases prob of default 
2. Changes exposure (often increases if long protection)

This creates strong **positive dependence** → Wrong-Way Risk (WWR).



## general conputation
if consider wrong-way risk, it includes:
1. simulate spread(CDS) and exposures together(aka, given one spread scenario, derive prob default and simulate exposure). 
2. then take the average of product of expousre and prob default, then sum over each time point.

if no wrong-way risk, then

simulate spread(CDS) and exposures independently. 
