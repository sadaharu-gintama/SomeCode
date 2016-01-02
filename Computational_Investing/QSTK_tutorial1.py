import QSTK.qstkutil.qsdateutil as du
import QSTK.qstkutil.tsutil as tsu
import QSTK.qstkutil.DataAccess as da

import datetime as dt
import matplotlib.pyplot as plt
import pandas as pd

ls_symbols = ['AAPL','GLD','GOOG','$SPX','XOM']
dt_start = dt.datetime(2010,1,1)
dt_end = dt.datetime(2010,1,15)
dt_timeofday = dt.timedelta(hours = 16)
ldt_timestamps = du.getNYSEdays(dt_start, dt_end, dt_timeofday)

c_dataobj = da.DataAccess('Yahoo')
ls_keys = ['open','high','low','close','volume','actual_close']

ldf_data = c_dataobj.get_data(ldt_timestamps, ls_symbols, ls_keys)

d_data = dict(zip(ls_keys, ldf_data))
d_data['close']

na_price = d_data['close'].values

plt.clf()
plt.plot(ldt_timestamps, na_price)
plt.legend(ls_symbols)
plt.ylabel('Ajusted Close')
plt.xlabel('Date')
plt.savefig('ajustedclose.pdf',format = 'pdf')

na_normalized_price = na_price / na_price[0:1]
