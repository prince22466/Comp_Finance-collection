# Survival Model
### modeling target: time-to-event as random variable
### application: credit event, prepayment event, machine failure, patient death, etc
------

## Survival Model Prob Foundation
T, the arrival time of event

s(t) = P(T>t), s(t) the chance of an object surviving longer than time t(or no event from 0 to t)

from conditional prob formula, P(A ∩ B)= P(A ∣ B) * P(B), thus s(t) can also be expressed in such manner.

for example, A={T>5}, B={T>3}, A ∩ B ={T>5}, then P(T>5) = P(T>5, T>3) * P(T>3). such calculation is used in **Kaplan-Meier** model

h(t) = 1 - s(t), the chance of an object surviving shorter than time t(or an event from 0 to t)



