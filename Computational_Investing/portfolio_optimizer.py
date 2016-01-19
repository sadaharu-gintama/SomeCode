import pandas as pd
import numpy as np
import math
import copy
import matplotlib.pyplot as plt
import QSTK.qstkutil.qsdateutil as du
import datetime as dt
import QSTK.qstkutil.tsutil as tsu
import QSTK.qstkutil.DataAccess as da
import QSTK.qstkstudy.EventProfiler as ep
'''
(c) 2011, 2012 Georgia Tech Research Corporation
This source code is released under the New BSD license.  Please see
http://wiki.quantsoftware.org/index.php?title=QSTK_License
for license details.
Created on Jan 16, 2013
@author: Sourabh Bajaj
@contact: sourabh@sourabhbajaj.com
@summary: EventProfiler
Modified on Nov 17, 2015 
By Jose Antonio Dura Olmos
contact: jadura@jadura.no-ip.org
summary : Fixed error with substraction to get a market neutral plot.
          Changed alpha of error bars from 0.1 to 0.6 because they were
        hard to see with that alpha for some people.


def eventprofiler(df_events_arg, d_data, i_lookback=20, i_lookforward=20,
                s_filename='study', b_market_neutral=True, b_errorbars=True,
                s_market_sym='SPY'):
   \''' Event Profiler for an event matix\'''
    df_close = d_data['close'].copy()
    df_rets = df_close.copy()

    # Do not modify the original event dataframe.
    df_events = df_events_arg.copy()
    tsu.returnize0(df_rets.values)

    if b_market_neutral == True:
#BEGIN MODIFICATION 
#The following line :
#        df_rets = df_rets - df_rets[s_market_sym]
#Is replaced by the following two lines
        for sym in df_events.columns:
            df_rets[sym] = df_rets[sym] - df_rets[s_market_sym]
#END MODIFICATION 
        del df_rets[s_market_sym]
        del df_events[s_market_sym]

    df_close = df_close.reindex(columns=df_events.columns)

    # Removing the starting and the end events
    df_events.values[0:i_lookback, :] = np.NaN
    df_events.values[-i_lookforward:, :] = np.NaN

    # Number of events
    i_no_events = int(np.logical_not(np.isnan(df_events.values)).sum())
    assert i_no_events > 0, "Zero events in the event matrix"
    na_event_rets = "False"

    # Looking for the events and pushing them to a matrix
    for i, s_sym in enumerate(df_events.columns):
        for j, dt_date in enumerate(df_events.index):
            if df_events[s_sym][dt_date] == 1:
                na_ret = df_rets[s_sym][j - i_lookback:j + 1 + i_lookforward]
                if type(na_event_rets) == type(""):
                    na_event_rets = na_ret
                else:
                    na_event_rets = np.vstack((na_event_rets, na_ret))

    if len(na_event_rets.shape) == 1:
        na_event_rets = np.expand_dims(na_event_rets, axis=0)

    # Computing daily rets and retuns
    na_event_rets = np.cumprod(na_event_rets + 1, axis=1)
    na_event_rets = (na_event_rets.T / na_event_rets[:, i_lookback]).T

    # Study Params
    na_mean = np.mean(na_event_rets, axis=0)
    na_std = np.std(na_event_rets, axis=0)
    li_time = range(-i_lookback, i_lookforward + 1)

    # Plotting the chart
    plt.clf()
    plt.axhline(y=1.0, xmin=-i_lookback, xmax=i_lookforward, color='k')
    if b_errorbars == True:
        plt.errorbar(li_time[i_lookback:], na_mean[i_lookback:],
                    yerr=na_std[i_lookback:], ecolor='#AAAAFF',
                    alpha=0.6) #Changed alpha from 0.1 to 0.6 (Jose A Dura)
    plt.plot(li_time, na_mean, linewidth=3, label='mean', color='b')
    plt.xlim(-i_lookback - 1, i_lookforward + 1)
    if b_market_neutral == True:
        plt.title('Market Relative mean return of ' +\
                str(i_no_events) + ' events')
    else:
        plt.title('Mean return of ' + str(i_no_events) + ' events')
    plt.xlabel('Days')
    plt.ylabel('Cumulative Returns')
    plt.savefig(s_filename, format='pdf')
'''
def find_events(ls_symbols, d_data, price):
    ''' Finding the event dataframe '''
    df_close = d_data['actual_close']
    ts_market = df_close['SPY']

    print "Finding Events"

    # Creating an empty dataframe
    df_events = copy.deepcopy(df_close)
    df_events = df_events * np.NAN

    # Time stamps for the event range
    ldt_timestamps = df_close.index

    for s_sym in ls_symbols:
        for i in range(1, len(ldt_timestamps)):
            # Calculating the returns for this timestamp
            f_symprice_today = df_close[s_sym].ix[ldt_timestamps[i]]
            f_symprice_yest = df_close[s_sym].ix[ldt_timestamps[i - 1]]

            # Event is found if the symbol is down more then 3% while the
            # market is up more then 2%
            if f_symprice_today < price and f_symprice_yest >= price:
                df_events[s_sym].ix[ldt_timestamps[i]] = 1

    return df_events

if __name__ == '__main__':
    dt_start = dt.datetime(2008, 1, 1)
    dt_end = dt.datetime(2009, 12, 31)
    ldt_timestamps = du.getNYSEdays(dt_start, dt_end, dt.timedelta(hours=16))

    for date in ['2008', '2012']:
        file_name = 'sp500' + date
        print file_name
        dataobj = da.DataAccess('Yahoo')
        ls_symbols = dataobj.get_symbols_from_list(file_name)
        ls_symbols.append('SPY')
        ls_keys = ['open', 'high', 'low', 'close', 'volume', 'actual_close']
        ldf_data = dataobj.get_data(ldt_timestamps, ls_symbols, ls_keys)
        d_data = dict(zip(ls_keys, ldf_data))

        for s_key in ls_keys:
            d_data[s_key] = d_data[s_key].fillna(method='ffill')
            d_data[s_key] = d_data[s_key].fillna(method='bfill')
            d_data[s_key] = d_data[s_key].fillna(1.0)

        for price in [5.0, 6.0, 7.0, 8.0, 9.0, 10.0]:
            print price
            df_events = find_events(ls_symbols, d_data, price)
            s_file = date + str(price) + '.pdf'
            ep.eventprofiler(df_events, d_data, i_lookback=20, i_lookforward=20,
                          s_filename=s_file, b_market_neutral=True,
                          b_errorbars=True, s_market_sym='SPY')
