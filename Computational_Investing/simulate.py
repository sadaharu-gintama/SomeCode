#Inputs to the function include:
#Start date
#End date
#Symbols for for equities (e.g., GOOG, AAPL, GLD, XOM)
#Allocations to the equities at the beginning of the simulation (e.g., 0.2, 0.3, 0.4, 0.1)
#The function should return:
#Standard deviation of daily returns of the total portfolio
#Average daily return of the total portfolio
#Sharpe ratio (Always assume you have 252 trading days in an year. And risk free rate = 0) of the total portfolio
#Cumulative return of the total portfolio

import QSTK.qstkutil.qsdateutil as du
import QSTK.qstkutil.tsutil as tsu
import QSTK.qstkutil.DataAccess as da

import datetime as dt
import numpy as np
import matplotlib.pyplot as plt
import pandas as pd

def simulate(start_date, end_date,
             symbols, allocation):
    # Get trading date time
    dt_timeofday = dt.timedelta(hours = 16)
    ldt_timestamps = du.getNYSEdays(start_date, end_date, dt_timeofday)
    # Get data from yahoo
    c_dataobj = da.DataAccess('Yahoo', cachestalltime=0)
    ls_key = ['close']
    ldf_data = c_dataobj.get_data(ldt_timestamps, symbols, ls_key)
    d_data = dict(zip(ls_key, ldf_data))
    # normalize daily price
    price = d_data['close'].values
    normalized_price = price / price[0:1]
    #caluclate return
    daily_portfolio_value = normalized_price.dot(allocation)
    daily_return = list()
    daily_return.append(0.0)
    for i in range(1, len(daily_portfolio_value)):
        daily_return.append(daily_portfolio_value[i]/ daily_portfolio_value[i-1] - 1)
    # standard deviation of return
    std = np.std(daily_return)
    #print std
    # average daily return
    average_daily_return = np.average(daily_return)
    #print average_daily_return
    # sharpe ratio
    sharpe_ratio = (np.sqrt(252.0) * average_daily_return / std)
    #print sharpe_ratio
    # accumulate return
    accumulate_return = daily_portfolio_value[-1] / daily_portfolio_value[0]
    #print accumulate_return
    return std, average_daily_return, sharpe_ratio, accumulate_return


if __name__ == '__main__':
    dt_start = dt.datetime(2011,1,1)
    dt_end = dt.datetime(2011,12,31)
    ls_symbol = ['AAPL','GLD', 'GOOG', 'XOM']
    allocation = [0.4,0.4,0.0,0.2]
    print simulate(dt_start, dt_end, ls_symbol, allocation)
