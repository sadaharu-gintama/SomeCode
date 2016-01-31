import numpy as np
import datetime as dt
import pandas as pd
import sys
import csv
import copy
import QSTK.qstkutil.qsdateutil as du
import QSTK.qstkutil.tsutil as tsu
import QSTK.qstkutil.DataAccess as da

def find_events(ls_symbols, d_data, price):
    ''' Finding the event dataframe '''
    df_close = d_data['actual_close']

    print "Finding Events"

    df_events = copy.deepcopy(df_close)
    df_events = df_events * np.NAN

    ldt_timestamps = df_close.index

    for s_sym in ls_symbols:
        for i in range(1, len(ldt_timestamps)):
            f_symprice_today = df_close[s_sym].ix[ldt_timestamps[i]]
            f_symprice_yest = df_close[s_sym].ix[ldt_timestamps[i - 1]]
            if f_symprice_today < price and f_symprice_yest >= price:
                df_events[s_sym].ix[ldt_timestamps[i]] = 1
    return df_events

if __name__ == '__main__':
    dt_start = dt.datetime(2008, 1, 1)
    dt_end = dt.datetime(2009, 12, 31)
    ldt_timestamps = du.getNYSEdays(dt_start, dt_end, dt.timedelta(hours = 16))
    #get list of symbol
    dataobj = da.DataAccess('Yahoo')
    symbols = dataobj.get_symbols_from_list('sp5002012')

    keys = ['actual_close']
    ldf_data = dataobj.get_data(ldt_timestamps, symbols, keys)
    d_data = dict(zip(keys, ldf_data))
    for s_key in keys:
        d_data[s_key] = d_data[s_key].fillna(method='ffill')
        d_data[s_key] = d_data[s_key].fillna(method='bfill')
        d_data[s_key] = d_data[s_key].fillna(1.0)

    p_list = [6.0, 7.0, 8.0, 9.0, 10.0]
    for price in p_list:
        df_events = find_events(symbols, d_data, price)
        file = 'trade' + str(int(price)) + '.csv'
        with open(file, 'w') as f:
            for i in range(1,len(ldt_timestamps)):
                for symbol in symbols:
                    if df_events[symbol].ix[ldt_timestamps[i]] == 1:
                        f.write('{},{},BUY,100,\n'.format(ldt_timestamps[i].strftime('%Y,%m,%d'),symbol))
                        new_time = du.getNYSEoffset(ldt_timestamps[i],5)
                        f.write('{},{},SELL,100,\n'.format(new_time.strftime('%Y,%m,%d'),symbol))
