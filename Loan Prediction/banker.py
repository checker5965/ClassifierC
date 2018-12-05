import pandas as pd

data=pd.read_csv('loantrain.csv')
data = data.dropna(axis=0)
data = data.drop(['Loan_ID'], axis=1)
dummyCols = ['Gender', 'Married', 'Dependents', 'Education', 'Self_Employed', 'Property_Area']
data = pd.get_dummies(data, columns = dummyCols)
status = {'Y': 1, 'N': 0}
data.Loan_Status = [status[item] for item in data.Loan_Status]
i = 0
cols = data.columns.tolist()
for col in cols:
    if col == 'Loan_Status':
        cols = cols[:i] + cols[i+1:] + [cols[i]]
        break
    i+=1
data = data[cols] 
data.to_csv("cleanBanker.csv", sep=',', encoding='utf-8', index=False, header=None)