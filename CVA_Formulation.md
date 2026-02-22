**CVA, general formula and conputation


**general formula(discrete form)

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





## when does the portfolio have have wrong-way risk exposure?Why Wrong-Way Risk is Large for CDS on the Counterparty

If the portfolio consists heavily of CDSs which referencing to counterparties, then:

- CDS spread drives **default probability**
- The same CDS spread drives **portfolio MtM → exposure**

Therefore exposure and default are **not independent**.

Credit deterioration simultaneously:

1. Increases \(\Delta PD_i\)  
2. Changes \(EPE(t_i)\) (often increases if long protection)

This creates strong **positive dependence** → Wrong-Way Risk (WWR).



**general conputation
includes:
1. compute positive exposure of the portfolio at each time points by simulation.
2. 
