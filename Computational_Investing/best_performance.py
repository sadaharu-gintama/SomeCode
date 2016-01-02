import simulate
import numpy as np
if __name__ == '__main__':
    dt_start = simulate.dt.datetime(2011,1,1)
    dt_end = simulate.dt.datetime(2011,12,31)
    ls_symbol = ['BRCM','TXN', 'AMD', 'ADI']
    best_allocation = list()
    best_sharpe = 0.0
    for allocate1 in np.arange(0 , 11):
        for allocate2 in np.arange(0, 11 - allocate1):
            for allocate3 in np.arange(0, 11 - allocate1 - allocate2):
                allocate4 = 10 - allocate1 - allocate2 - allocate3
                allocation = 1.0 * np.array([allocate1, allocate2, allocate3, allocate4]) / 10.0
                vol, daily_ret, sharpe, cum_ret = simulate.simulate(dt_start, dt_end, ls_symbol, allocation)
                #print sharpe
                if sharpe > best_sharpe:
                    best_sharpe = sharpe
                    best_allocation = allocation

    print best_sharpe
    print best_allocation
