from yahoo_finance import Share
import pandas as pd
from pandas import DataFrame

start_date = '2000-01-01'
end_date = '2015-01-01'
# SCHB: Schwab U.S. Broad Market ETF
# SCHA: Schwab U.S. Small Cap ETF
# SCHH: Schwab U.S. REITS ETF
# SCHF: Schwab International Equity ETF
# SCHE: Schwab Emerging Market ETF
# SCHZ: Schwab U.S. Aggregate Bond ETF
# SCHP: Schwab U.S. Tips ETF
# DSUM: PowerShare Chinese Yuan Dim Sum Bond ETF
sticker = ['SCHB','SCHA','SCHH','SCHF','SCHE','SCHZ','SCHP','DSUM']
stocks = [Share(x) for x in sticker]
rtns = DataFrame()
for stock in stocks:
    data = stock.get_historical(start_date, end_date)
    price = [float(x['Close']) for x in data]
    name = stock.symbol
    df = DataFrame(price, columns=[name])
    df[name][1:] = df[name][1:].values / df[name][:-1].values - 1  
    df[name][:1] = 0
    rtns = pd.concat([rtns,df], axis=1, join='inner')

rtns.corr().to_csv("Data.csv")
