import numpy as np
from scipy.stats import norm
import matplotlib.pyplot as plt


class MCPricer:
    def __init__(self, num_paths: int, num_steps: int, 
                 s0: float, dt: float, sigma: float,
                 K:float=None, 
                 B:float = None,
                 paths_input=  None): # for quick testing
        """
        risk rate is assumed to be zero for simplicity
        """
        if num_paths <= 0:
            raise ValueError("num_paths must be > 0")
        if num_steps <= 0:
            raise ValueError("num_steps must be > 0")
        if s0 <= 0.0:
            raise ValueError("s0 must be > 0")
        if dt <= 0.0:
            raise ValueError("dt must be > 0")

        self.num_paths = int(num_paths)
        self.num_steps = int(num_steps)
        self.s0 = float(s0)
        self.dt = float(dt)
        self.K = K
        self.sigma = sigma
        self.B=B
        self.paths = paths_input
        if self.paths is not None:
                self.prob_surv_paths=np.zeros([self.paths.shape[0],self.paths.shape[1]-1], dtype=float)
        else:
            self.prob_surv_paths=np.zeros([num_paths,num_steps], dtype=float)# for barrier with MC conditioning

    def simulate_paths(
        self,
        dt,
        volatility: float,
        seed: int | None = None,
    ) -> None:
        if volatility < 0.0:
            raise ValueError("volatility must be >= 0")
        
        if(self.paths is None):
            self.simulate_returns_model(dt,volatility,seed)
            #self.paths


    def simulate_returns_model(self,
        dt:float,
        volatility: float,
        seed: int | None = None,
        model: str = "GBM",) -> None:
        """
        naive simulation,
        simulate returns based on model, by default, the GBM model is used, 
        risk free rate is assumed to be zero for simplicity
        """

        # path initilization
        paths = np.empty((self.num_paths, self.num_steps+1), dtype=float)
        paths[:, 0] = self.s0

        rng = np.random.default_rng(seed)
        dWs = rng.standard_normal((self.num_paths, self.num_steps)) # matrix of indenpdent standard normal variables

        if model=="GBM":
            ############ GBM model, with drift =0 (risk free rate assumes to be 0) ############
            ############ GBM model, dS/S = r*dt + sigma*dWt ############
            ############ GBM model, S_next = S_current * exp(-0.5 * σ² Δt + σ ΔW) ############
            diffusion = volatility * dWs
            returns = np.exp(-0.5* volatility**2 * dt + diffusion)# one step has 1 dt

            #first column equal S0
            paths[:,0]=self.s0
            for c_num in range(1,paths.shape[1]):
                paths[:,c_num]=paths[:,c_num-1]*returns[:,c_num-1]

            self.paths = paths
        else:
            NotImplementedError(f"model {model} is not implemented in simulate_returns_model.")


    def conditioning_on_paths(self) -> np.ndarray:
        if self.paths.shape[0] != self.prob_surv_paths.shape[0] :
            raise ValueError("self.paths and self.prob_surv_paths must have same count of rows.")
        if self.paths.shape[1]-1 != self.prob_surv_paths.shape[1]:
            raise ValueError(f"it should have 1 less count in col for prob_surv_paths. {self.paths.shape[1]} vs {self.prob_surv_paths.shape[1]}")
        
        for row_num, p in enumerate(self.paths):
            if np.max(p) >= self.B:
                continue
            else:
                for i in range(len(p)-1):
                    prob_hit_interval = np.exp(-2*(self.B-p[i])*(self.B-p[i+1])/(self.sigma**2*self.dt)) # the formula is model based(GBM)
                    self.prob_surv_paths[row_num][i]=1 - prob_hit_interval # prob of survival
    
    
    def prob_BM_A_B(self,A,B,n) -> float:
        # probability of reaching higher than B start from A in n steps, 
        # assuming the price follows GBM, with zero drift and volatility sigma
        # distribution of delta is normal with mean 0 and variance sigma^2*n
        # P(ST​>B∣St​=A)=P(log(ST​/A)>log(B/A)) =P(log(ST​/A) - log(B/A) > 0)
        target = np.log(B/A)
        # create a normal disitribution, which is with mean 0 and variance sigma^2*n
        
        z = (target + 0.5*(self.sigma**2) * n*self.dt) / (self.sigma * np.sqrt(n*self.dt))
        prob = 1.0 - norm.cdf(z)
        return prob

    def final_prices(self) -> np.ndarray:
        return self.paths[:, -1]

    def vanilla_final_payoffs(self) -> np.ndarray:
        if self.K is not None :
            return np.maximum(self.paths[:, -1] - self.K,0.0) # payoff of vanilla call option
        else:
            print("self.K is None.")

    def vanilla_option_price(self) -> float:
        return self.vanilla_final_payoffs().mean()
    

    def digital_final_payoffs(self) -> np.ndarray:
        if self.K is not None :
            vaniila_payoff= np.maximum(self.paths[:, -1] - self.K,0.0) # payoff of vanilla call option
            return np.where(vaniila_payoff>0.0,1.0,0.0) # payoff of digital call option
        else:
            print("self.K is None.")

    def digital_option_price_naiveMC(self) -> float:
        all_payoffs = self.digital_final_payoffs()
        ave=all_payoffs.mean()
        variance = all_payoffs.var()
        return ave,variance
    
    def digital_option_price_condMC(self) ->float:
        #half step
        half_step = int(self.num_steps/2)
        # conditioning on the price at half step, for each path, caluclate the prob
        rest_steps = self.num_steps - half_step
        half_step_prices = self.paths[:, half_step:half_step+1].flatten() # price at half step
        probs_half_step_prices = [self.prob_BM_A_B(x,self.K,rest_steps) for x in half_step_prices]
        price = np.mean(probs_half_step_prices)
        variance = np.var(probs_half_step_prices)
        return price,variance

    def barrier_option_price_naiveMC(self) -> float:
        indexer = np.array([0.0 if np.max(p)>=self.B else 1.0 for p in self.paths])# index of hitting upper bar
        print(f"indexer: {indexer}")
        return (self.vanilla_final_payoffs()*indexer).mean()

    def barrier_option_price_condMC(self) -> float:
        final_prob_surv = self.prob_surv_paths.prod(axis=1) # final prob of survival for each path, by multiplying the prob of survival at each step
        print(f"final_prob_surv: {final_prob_surv}")
        return (self.vanilla_final_payoffs()*final_prob_surv).mean()

    def barrier_option_price_comparison(self) -> None:
        #run simulation and conditioning
        self.simulate_paths(self.dt,self.sigma, seed=42)
        self.conditioning_on_paths()
        print("interval_prob_surv_paths: ", self.prob_surv_paths)


        # price compare
        price_naive = self.barrier_option_price_naiveMC()
        price_cond = self.barrier_option_price_condMC()
        print(f"Barrier option price naive MC vs conditioning MC: {price_naive} <-> {price_cond}")

        # variance compare
        indexer = np.array([0.0 if np.max(p)>=self.B else 1.0 for p in self.paths])# index of hitting upper bar
        var_naive = (self.vanilla_final_payoffs()*indexer).var()
        final_prob_surv = self.prob_surv_paths.prod(axis=1) # final prob of survival for each path, by multiplying the prob of survival at each step
        var_cond= (self.vanilla_final_payoffs()*final_prob_surv).var()
        print(f"Barrier option price variance naive MC vs conditioning MC: {var_naive} <-> {var_cond}")
    


    def digital_option_price_comparison(self) -> float:
        #run simulation and conditioning
        self.simulate_paths(self.dt,self.sigma, seed=42)

        # price compare
        price_naive,var_naive = self.digital_option_price_naiveMC()
        price_cond,var_cond = self.digital_option_price_condMC()
        print(f"Digtial option price naive MC vs conditioning MC: {price_naive} <-> {price_cond}")

        # variance compare
        print(f"Digital option price variance naive MC vs conditioning MC: {var_naive} <-> {var_cond}")

        return price_naive,price_cond


    def plot_paths(self,max_paths: int = 50) -> None:
        n = min(max_paths, self.paths.shape[0])

        plt.plot(self.paths[:n].T, linewidth=1.0)
        plt.xlabel("Step")
        plt.ylabel("Price")
        plt.title("MC Paths")
        plt.grid(alpha=0.3)
        plt.show()





def Barrier_demo():
    ### for Up-and-Out Call Barrier
    ###
    num_paths = 5
    num_steps = 7
    s0 = 100.0
    dt = 1
    rate = 0.0
    sigma = 0.15
    K=100.0
    B =105.0
    paths=np.array([[100.        , 98.10,  104.30, 102.15,
                    102.03,  102.52,  101.15,  102.35 ],
                    [100.        ,  96.88,  96.72,  88.81,
                        96.98, 104.82, 102.52, 118.11],
                    [100.        , 104.78,  96.15,  99.77,
                        90.65,  98.98,  98.48,  96.67],
                    [100.        ,  100.11 , 104.58, 103.95,
                        99.60,  96.14, 101.40, 105.18],
                    [100.        , 104.21, 103.80, 134.78 ,
                        129.42, 122.95, 113.35, 120.55]])



    pricer = MCPricer(num_paths, num_steps, s0, dt,sigma, K, B,paths)
    pricer.barrier_option_price_comparison()
    pricer.plot_paths() #show 50 path by default


def Digital_demo():
    ### for Up-and-Out Call Barrier
    ###
    num_paths = 10000
    num_steps = 70
    s0 = 100.0
    dt = 1
    rate = 0.0
    sigma = 0.15
    K=110.0


    pricer1 = MCPricer(num_paths, num_steps, s0, dt,sigma, K)
    pricer1_naive, pricer1_cond =pricer1.digital_option_price_comparison()

    price_pump=0.01
    s0_1 = s0+price_pump
    pricer2 = MCPricer(num_paths, num_steps, s0_1, dt,sigma, K)
    pricer2_naive, pricer2_cond =pricer2.digital_option_price_comparison()

    #delta of digital
    print(f"Digital Option Geek-Delta comparison naive MC vs conditiong MC: {(pricer2_naive-pricer1_naive)/price_pump} vs {(pricer2_cond-pricer1_cond)/price_pump}")

if __name__ == "__main__":
    Digital_demo()