import numpy as np
import pandas as pd
import pandas.io.data as web

goog = web.DataReader('AXP', data_source = 'google',
                      start = '3/14/2014', end = '3/14/2015')

goog.tail()

goog['Log_Ret'] = np.log(goog['Close'] / goog['Close'].shift(1))
goog['Volatility'] = pd.rolling_std(goog['Log_Ret'], window = 252) \
                     * np.sqrt(252)

goog[['Close', 'Volatility']].plot(subplots = True, color = 'blue',
                                   figsize = (8, 6))

