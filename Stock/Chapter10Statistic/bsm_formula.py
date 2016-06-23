from math import log, sqrt, exp
from scipy import stats
import pandas as pd
import matplotlib.pyplot as plt
#Black Scholes Merton Function
def bsm_call_value(S0, K, T, r, sigma):
    S0 = float(S0)
    d1 = (log(S0 / K) + (r + 0.5 * sigma ** 2) * T) / (sigma * sqrt(T))
    d2 = (log(S0 / K) + (r - 0.5 * sigma ** 2) * T) / (sigma * sqrt(T))
    value = (S0 * stats.norm.cdf(d1, 0.0, 1.0) - K * exp(-r * T) *
             stats.norm.cdf(d2, 0.0, 1.0))
    return value

def bsm_vega(S0, K, T, r, sigma):
    S0 = float(S0)
    d1 = (log(S0 / K) + (r + 0.5 * sigma ** 2) * T) / (sigma * sqrt(T))
    vega = S0 * stats.norm.cdf(d1, 0.0, 1.0) * sqrt(T)
    return vega

def bsm_call_imp_vol(S0, K, T, r, C0, sigma_est, it = 100):
    for i in range(it):
        sigma_est -= ((bsm_call_value(S0, K, T, r, sigma_est) - C0) /
                      bsm_vega(S0, K, T, r, sigma_est))
    return sigma_est

if __name__ == '__main__':
    V0 = 17.6639
    r = 0.01

    h5 = pd.HDFStore('vstoxx_data_31032014.h5', 'r')
    futures_data = h5['futures_data']
    options_data = h5['options_data']
    h5.close()

    options_data['IMP_VOL'] = 0.0

    tol = 0.5
    for option in options_data.index:
        forward = futures_data[futures_data['MATURITY'] == \
                               options_data.loc[option]['MATURITY']]\
                               ['PRICE'].values[0]
        if (forward * (1 - tol) < options_data.loc[option]['STRIKE']
            < forward * (1 + tol)):
            imp_vol = bsm_call_imp_vol(
                V0,
                options_data.loc[option]['STRIKE'],
                options_data.loc[option]['TTM'],
                r,
                options_data.loc[option]['PRICE'],
                sigma_est = 2,
                it = 100)

            options_data['IMP_VOL'].loc[option] = imp_vol

    plot_data = options_data[options_data['IMP_VOL'] > 0]
    maturities = sorted(set(options_data['MATURITY']))
    plt.figure(figsize = (8,6))
    for maturity in maturities:
        data = plot_data[options_data.MATURITY == maturity]
        plt.plot(data['STRIKE'], data['IMP_VOL'],
                 label = maturity.date(), lw = 1.5)
        plt.plot(data['STRIKE'], data['IMP_VOL'], 'r.')
    plt.grid(True)
    plt.xlabel('Strike')
    plt.ylabel('Implied volatility of volatility')
    plt.legend()
    plt.show()

    keep = ['PRICE', 'IMP_VOL']
    group_data = plot_data.groupby(['MATURITY', 'STRIKE'])[keep]
    
