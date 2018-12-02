import pandas as pd

data=pd.read_csv('turkiye-student-evaluation_generic.csv')
data.Q28 = [item-1 for item in data.Q28]
data.to_csv("gradePredict.csv", sep=',', encoding='utf-8', index=False, header=None)