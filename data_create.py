import pandas as pd
from yahoo_fin import stock_info as si

# List of tickers in the S&P 500 index
tickers = si.tickers_sp500()

# Retrieve data for each ticker and filter to latest date
data_list = []
for ticker in tickers:
    data = si.get_data(ticker)
    latest_data = data[['open', 'close', 'volume']].iloc[-1]
    data_list.append(latest_data)

# Combine data for all tickers into a single DataFrame
df = pd.concat(data_list, axis=0, keys=tickers)

# Save DataFrame to CSV file
df.to_csv('latest_data.csv')


# start = dt.datetime.now() - dt.timedelta(days= 365 * 2)
# end = dt.datetime.now()

# data = web.DataReader(list(comanies_dow), 'yahoo', start, end)
# open_values = np.array(data['Open'].T)
# clone_value = np.array(data['close'].T)
# print(open_values.shape)