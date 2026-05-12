# Survival Model
### modeling target: time-to-event as random variable
### application: credit event, prepayment event, machine failure, patient death, etc
------

## Survival Model Prob Foundation

### survival prob
T, the arrival time of event

s(t) = P(T>t), s(t) the chance of an object surviving longer than time t(or no event from 0 to t).

s(0) = 1(definitely survial at time 0), s(+∞) = 0(definitely event arrive when time goes infinite) 

from conditional prob formula, P(A ∩ B)= P(A ∣ B) * P(B), thus s(t) can also be expressed in such manner.

for example, A={T>5}, B={T>3}, A ∩ B ={T>5}, then P(T>5) = P(T>5 / T>3) * P(T>3). such calculation is used in **Kaplan-Meier** model


### hazard funtion, and hazard rate
h(t) = P(t < T < t+Δt / T>t) /Δt , is the hazard function, which calculates the likelihood(not chance, because it can be larger than 1) of event occurance per Δt,  given survival time greater than t, aka,  intensity.

hazard rate λ, is the h(t) when Δt->0. λ is mainly used in continous hazard model(such as exponential model), and hazard rate has its own unit time according to the case of usage. 

for example, λ could be monthly rate being used to caluclate monthly chance of prepayment or credit default as 1 - exp(-λ), rate of each minute for prob of machine failure in each minute, etc  


prob of event occurance during period of time ≈  harzard rate * time interval


### Common models
