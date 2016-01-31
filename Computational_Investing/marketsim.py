import numpy as np
import datetime as dt
import pandas as pd
import sys
import csv
import QSTK.qstkutil.qsdateutil as du
import QSTK.qstkutil.tsutil as tsu
import QSTK.qstkutil.DataAccess as da

def ReadCSV(order_file):
    trade_date = list()
    symbol = list()
    order = list()
    with open(order_file, 'rb') as csvfile:
        reader = csv.reader(csvfile, delimiter = ',')
        for row in reader:
            trade_date.append(dt.datetime(int(row[0]), int(row[1]), int(row[2]), 16, 0,0))
            symbol.append(row[3])
            if row[4] == 'BUY':
                order.append(int(row[5]))
            else:
                order.append(-int(row[5]))
    sorted_order = sorted(zip(trade_date, symbol, order))
    return zip(*sorted_order)

def ReadData(trade_date, symbol):
    dt_start = trade_date[0]
    dt_end = trade_date[-1]
    ldt_timestaps = du.getNYSEdays(dt_start, dt_end, dt.timedelta(hours = 16))
    dataobj = da.DataAccess('Yahoo')
    keys = ['close', 'actual_close']
    ldf_data = dataobj.get_data(ldt_timestaps, symbol, keys)
    d_data = dict(zip(keys, ldf_data))
    for s_key in keys:
        d_data[s_key] = d_data[s_key].fillna(method='ffill')
        d_data[s_key] = d_data[s_key].fillna(method='bfill')
        d_data[s_key] = d_data[s_key].fillna(1.0)
    return d_data

def UpdateCash(cash, tday, d_close):
    price = d_close.at[tday[0],tday[1]]
    cash = cash - float(price * tday[2])
    return cash

def UpdateOwn(own, tday):
    own[tday[1]] += tday[2]
    return own

def UpdateDailyValue(day, cash, own, dclose):
    daily_value = cash
    symbol = own.keys()
    for s in symbol:
        no_of_stock = own[s]
        price = dclose.at[day, s]
        daily_value += float(price * no_of_stock)
    return daily_value

if __name__ == '__main__':
    p_list = [6,7,8,9,10]
    for price in p_list:
        start_value = 50000.0
        order_file = 'trade' + str(price) + '.csv'
        trade_date, symbol, order = ReadCSV(order_file)
        d_data = ReadData(trade_date, set(symbol))
        d_close = d_data['close']

        # prepare output
        dt_start = trade_date[0]
        dt_end = trade_date[-1]

        ldt_timestaps = du.getNYSEdays(dt_start, dt_end, dt.timedelta(hours = 16))
        cash = start_value
        ls_time = list()
        own = dict(zip(set(symbol), [0] * len(set(symbol))))

        for day in ldt_timestaps:
            for tday in zip(trade_date, symbol, order):
                if day == tday[0]:
                    cash = UpdateCash(cash, tday, d_close)
                    own = UpdateOwn(own, tday)
            daily_value = UpdateDailyValue(day, cash, own, d_close)
            ls_time.append(daily_value)

        ls_return = list()
        for i in range(1, len(ls_time)):
            ls_return.append(ls_time[i] / ls_time[i-1] - 1.0)

        std_metric = np.std(ls_return)
        sharpe = np.sqrt(252.0) * np.average(ls_return) / std_metric
        with open('result' + str(price) + '.txt','w') as f:
            f.write('STD:' + str(std_metric) + '\n')
            f.write('Sharpe:' + str(sharpe) + '\n')
            f.write('Return:' + str(ls_time[-1] / start_value) + '\n')
