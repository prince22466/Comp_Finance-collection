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


### Primary Models
| Model | Discrete/Continous | Assumptions | Building Prcess | Advantage | Weakness | Improvement |
|---|---|---|---|---|---|---|
| Kaplan-Meier | discrete | No assumption, purely data explorative model | rank the tabelu data by time of event, then calculate things like P(T>5) = P(T>5 / T>3) * P(T>3), then do this for each time point t, then get a table of P(T>t_i) | simple process, conditional prob calculation avoid data censorship problem | the plunges of prob of survival | could incorporate interpolation techniques for curve smoothing |
| Exponential model | continuous | constant hazard rate, which means hazard rate is inherent property of an object, and it has nothing to do with time and other factors | use max likelihood of event occurance at each time points or interval for calibrating harzard rate | simple and smooth prob of survival overtime | same hazard rate be applied on different objects | group by different types of objects, then calibrate group wise hazard rates |
| Cox Proportional Hazard model | continous | constant baseline hazard | use max ratio of likelihood of event occurance at each time points or interval for calibrating harzard rate | incorporate features or factors, so that different objects have different hazard rate due to different features | the likelihood ratio is static over time | incorporate the dynamic factor of time and other factors, for example, time*prepay_incentive |
