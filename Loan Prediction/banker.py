import pandas as pd

data=pd.read_csv('loantrain.csv')
data = data.dropna(axis=0)
data = data.drop(['Loan_ID'], axis=1)
gender = {'Male': 0, 'Female': 1}
married = {'Yes': 1, 'No': 0}
dependents = {'0': 0, '1': 1, '2': 2, '3': 3, '3+': 4}
education = {'Graduate': 1, 'Not Graduate': 0}
selfemployed = {'No': 0, 'Yes': 1}
proparea = {'Urban': 2, 'Semiurban': 1, 'Rural': 0}
status = {'Y': 1, 'N': 0}
data.Gender = [gender[item] for item in data.Gender]
data.Married = [married[item] for item in data.Married]
data.Dependents = [dependents[item] for item in data.Dependents]   
data.Education = [education[item] for item in data.Education]
data.Self_Employed = [selfemployed[item] for item in data.Self_Employed]
data.Property_Area = [proparea[item] for item in data.Property_Area]  
data.Loan_Status = [status[item] for item in data.Loan_Status]
data.to_csv("cleanBanker.csv", sep=',', encoding='utf-8', index=False, header=None)