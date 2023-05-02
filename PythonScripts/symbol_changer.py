import pandas as pd
import random
import string

df = pd.read_csv('prices.csv')

column_name = 'Stock_symbol'

# Define a function to generate random letters
def random_letters(n):
    return ''.join(random.choices(string.ascii_uppercase, k=n))

df[column_name] = df[column_name].apply(lambda x: random_letters(4))

# Write the updated DataFrame to a new CSV file
df.to_csv('updated_prices.csv', index=False)
