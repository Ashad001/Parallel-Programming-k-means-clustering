import csv
import pandas as pd
import os

#appended rows to be added in new file/current file
new_rows=[]
#headers used to distinguish
header = ['Company','tobe','Open','Close','volume']

#read the input file
with open('latest_data.csv',mode='r') as reader:
    csvFile = csv.reader(reader)
    for row1 in csvFile:
        row2=next(csvFile)
        row3=next(csvFile)
        # if float(row3[2]) > 100000.0:
        row3[2] = float(row3[2]) % 50000
        # print(row3[2])
        if row3[2] == 0:
            row3[2] = 50000
        #append of open and close price of stock
        new_rows.append(row1+row2[-1:]+row3[-1:])

#write in the output file    
with open('clean_data.csv','w') as file:
    csvwriter = csv.writer(file)
    csvwriter.writerow(header)
    csvwriter.writerows(new_rows)
    
#removing unwanted column    
data = pd.read_csv('clean_data.csv')
data.pop('tobe')
data.to_csv('lastest_data.csv')

#removing unwated files
os.remove('clean_data.csv')