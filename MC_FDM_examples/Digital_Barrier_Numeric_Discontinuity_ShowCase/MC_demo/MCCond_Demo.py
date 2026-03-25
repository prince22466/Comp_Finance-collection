## small experiment to show Naive MC vs MC_conditiong on varicance reduce
# Y = X + e, where X is N(0,4), e is N(0,1),
# aka, Y is a function of X and noise
# we run naive MC to estimate E[Y], directly,
# and we run MC_conditioning to estimate E[Y], by conditioning on X, which is running X only
# E[Y] = E[X], but Var[Y] > Var[X],
# because Var[Y]=E[Var[Y∣X]]​​+Var[E[Y∣X]], 1st component is noise
# the degree of reducing variance depends on Var[e], the noise component



import numpy as np

num_steps = 100
num_paths = 500
X0 = 3.0

# simulate X and e
rng = np.random.default_rng(seed=42)
Xs = rng.normal(loc=0.0, scale=2.0, size=(num_paths, num_steps))
es = rng.normal(loc=0.0, scale=5.0, size=(num_paths, num_steps))

# cumulative sum to make X a random walk, starting from X0, same for Y 
Xs = np.cumsum(Xs, axis=1) + X0 
Ys = Xs + es


# final 
final_Xs = Xs[:, -1]
final_Ys = Ys[:, -1]


#compare
print("mean check Naiive MC vs MC with condtioning: ", final_Ys.mean(), " vs ", final_Xs.mean())
print("variance check Naiive MC vs MC with condtioning: ", final_Ys.var(), " vs ", final_Xs.var())