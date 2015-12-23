from yahoo_finance import Share
import pandas as pd
from pandas import DataFrame
import matplotlib.pyplot as plt
import matplotlib.dates as Dates

start_date = '2015-01-01'
end_date = '2015-12-01'
# SCHB: Schwab U.S. Broad Market ETF
# SCHA: Schwab U.S. Small Cap ETF
# SCHH: Schwab U.S. REITS ETF
# SCHF: Schwab International Equity ETF
# SCHE: Schwab Emerging Market ETF
# SCHZ: Schwab U.S. Aggregate Bond ETF
# SCHP: Schwab U.S. Tips ETF
# DSUM: PowerShare Chinese Yuan Dim Sum Bond ETF
sticker = ['SCHB', 'SCHZ', 'DSUM']#,'SCHA','SCHH','SCHF','SCHE','SCHZ','SCHP','DSUM']
stocks = [Share(x) for x in sticker]

for stock in stocks:
    data = stock.get_historical(start_date, end_date)
    price = [float(x['Close']) for x in data]
    date = [Dates.datestr2num(x['Date']) for x in data]
    plt.subplot(3,1,stocks.index(stock) + 1)
    plt.plot_date(date, price, 'r-')

plt.show()
