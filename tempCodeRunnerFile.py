#removing unwanted column    
data = pd.read_csv('clean_data.csv')
data.pop('tobe')
data.to_csv('lastest_data.csv')

#removing unwated files
os.remove('clean_data.csv')