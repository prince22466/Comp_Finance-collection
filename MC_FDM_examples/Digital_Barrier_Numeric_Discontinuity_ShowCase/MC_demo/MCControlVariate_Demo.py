## small experiment to show Naive MC vs MC_ControlVariate on varicance reduce
# where X is N(0,4), e is N(0,5),
# we run naive MC to estimate E[Y], directly,
# and we run MC_ControlVariate to estimate E[X], by using formular Z = X - b(C - E[C])
# E[Z] = E[X], but Var[X] > Var[Z] if C and b is well choosen
# VAR[Z] = VAR[X] + b^2 * VAR[C] - 2*b*Cov[X,C], given X and C is positively correlated( or Cov[X,C]>0),
# so if b^2 * VAR[C] - 2*b*Cov[X,C] < 0, then VAR[Z] < VAR[X], and variance reduced
# for demo, b is set as 0.3, C is chosen as high coviance with X


import numpy as np

num_steps = 10
num_paths = 5
X0 = 3.0
b=0.3
a = 5

# simulate X and e
rng = np.random.default_rng(seed=42)
Xs = rng.normal(loc=0.0, scale=2.0, size=(num_paths, num_steps))
es = rng.normal(loc=0.0, scale=5.0, size=(num_paths, num_steps))

Cs = a*Xs + es # make C have postive covariance with X, so that control variate can work well
E_C=Cs.mean()


# cumulative sum to make X a random walk, starting from X0, 
Zs = Xs - b*(Cs - E_C)
Xs = np.cumsum(Xs, axis=1) + X0 
Zs = np.cumsum(Zs, axis=1) + X0  # control variate,

# final 
final_Xs = Xs[:, -1]
final_Zs = Zs[:, -1]
#compare
print("mean check Naiive MC vs MC with Control Variate: ", final_Xs.mean(), " vs ", final_Zs.mean())
print("variance check Naiive MC vs MC with Control Variate: ", final_Xs.var(), " vs ", final_Zs.var())