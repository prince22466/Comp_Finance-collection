**CVA, general formula and conputation


**general formula

## Wrong-Way Risk when CDS Portfolio References the Counterparty

### Core CVA formula (discrete form)

\[
\text{CVA} \approx \text{LGD}\sum_i DF(t_i)\;EPE(t_i)\;\Delta PD_i
\]

where:

- **LGD** = Loss Given Default = \(1-R\)  
- \(DF(t_i)\) = discount factor  
- \(EPE(t_i)\) = expected positive exposure at time \(t_i\)  
- \(\Delta PD_i = PD(t_i) - PD(t_{i-1}) = S(t_{i-1}) - S(t_i)\)

---

## Why Wrong-Way Risk is Large for CDS on the Counterparty

If the portfolio consists mainly of CDS whose **reference entity = counterparty**, then:

- CDS spread drives **default probability**
- The same CDS spread drives **portfolio MtM → exposure**

Therefore exposure and default are **not independent**.

Credit deterioration simultaneously:

1. Increases \(\Delta PD_i\)  
2. Changes \(EPE(t_i)\) (often increases if long protection)

This creates strong **positive dependence** → Wrong-Way Risk (WWR).

---

## Mathematical view (joint expectation)

True CVA term:

\[
\mathbb{E}\!\left[E^+(t_i)\,\mathbf{1}_{\{\tau \in (t_{i-1},t_i]\}}\right]
\]

Independence assumption (often used):

\[
\mathbb{E}[E^+(t_i)] \cdot \mathbb{Q}(\tau \in (t_{i-1},t_i])
\]

For CDS on the counterparty, this separation is invalid because both terms depend on the same credit factor.

---

## Sensitivity decomposition (shows WWR explicitly)

If spread \(s\) drives both exposure and PD:

\[
\frac{\partial \text{CVA}}{\partial s}
\approx
\text{LGD}\sum_i DF(t_i)
\left[
\frac{\partial EPE(t_i)}{\partial s}\,\Delta PD_i
+
EPE(t_i)\,\frac{\partial (\Delta PD_i)}{\partial s}
\right]
\]

Two components:

- **Exposure / WWR term**
  \[
  \frac{\partial EPE}{\partial s}\,\Delta PD
  \]

- **Pure PD term**
  \[
  EPE\,\frac{\partial \Delta PD}{\partial s}
  \]

In standard portfolios the first term is often small.  
For CDS on the counterparty it can be **large or dominant**.

---

## Intuition

For a bought-protection CDS on the counterparty:

- Spread widens → CDS MtM increases → exposure rises  
- Spread widens → default probability rises  

Worst credit states produce **larger exposure AND higher default probability**.

That is the definition of Wrong-Way Risk.

---

## Practical implications

- Independence-based CVA underestimates risk  
- CS01 of CVA contains a large exposure-driven component  
- Hedging CVA becomes self-referential  
- Coupled simulation (joint credit + exposure) is required

Typical desk approaches:

- Joint spread / exposure simulation  
- Stress WWR scenarios  
- WWR add-ons or multipliers



**general conputation
