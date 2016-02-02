import numpy as np
import datetime as dt
import pandas as pd
import sys
import csv
import copy
import QSTK.qstkutil.qsdateutil as du
import QSTK.qstkutil.tsutil as tsu
import QSTK.qstkutil.DataAccess as da

if __name__ == '__main__':
    look_back = raw_input('Please Input Lookback:')
    look_back = int(look_back)
    while (True):
        date = raw_input('Please Input Date:')
        tick = raw_input('Please Input Stock:')
        tick = str(tick)
        [year, month, day] = [int(x) for x in str(date).split('-')]
        dt_end = dt.datetime(year, month, day)
        dt_end = du.getNYSEoffset(dt_end, 1)
        dt_begin = du.getNYSEoffset(dt_end, -look_back)
        ldt_timestamps = du.getNYSEdays(dt_begin, dt_end, dt.timedelta(hours = 16))
        dataobj = da.DataAccess('Yahoo')
        keys = ['actual_close', 'close']
        ldf_data = dataobj.get_data(ldt_timestamps, [tick], keys)
        d_data = dict(zip(keys, ldf_data))
        for s_key in keys:
            d_data[s_key] = d_data[s_key].fillna(method='ffill')
            d_data[s_key] = d_data[s_key].fillna(method='bfill')
            d_data[s_key] = d_data[s_key].fillna(1.0)

        df_close = d_data['actual_close']
        mean = pd.rolling_mean(df_close, look_back, look_back)
        std = pd.rolling_std(df_close, look_back, look_back)
        mean = float(mean[tick].ix[ldt_timestamps[-2]])
        std = float(std[tick].ix[ldt_timestamps[-2]])
        print mean, std
        print (float(df_close[tick].ix[ldt_timestamps[-2]]) - mean) / std
