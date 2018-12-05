import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.tree import DecisionTreeClassifier
from sklearn.ensemble import RandomForestRegressor
from sklearn.metrics import mean_absolute_error
from sklearn.preprocessing import Imputer
from xgboost import XGBRegressor
from sklearn.ensemble import GradientBoostingRegressor

f = open('train.csv', 'rU')
data=pd.read_csv(f)
data = data.fillna(0)
data = data.drop(data.index[50000:])
y = data.Purchase
features = ['Product_ID', 'Gender', 'Age', 'Occupation', 'City_Category', 'Marital_Status', 'Product_Category_1', 'Product_Category_2', 'Product_Category_3']
data = data[features]
objcols = ['Product_ID', 'Gender', 'Age', 'City_Category']
reasonableCol = []
for col in objcols:
    if (data[col].nunique()<=10):
        reasonableCol.append(col)
for col in objcols:
    if col not in reasonableCol:
        data = data.drop(col, axis=1)
data = pd.get_dummies(data, columns = reasonableCol)
X = data
Xtr, XT, ytr, yT = train_test_split(X,y)
model = DecisionTreeClassifier(random_state=1)
model.fit(Xtr,ytr)
predictions = model.predict(XT)
print("The mean absolute error for Decision Tree is: {:.2f}".format(mean_absolute_error(yT, predictions)))
model = RandomForestRegressor(n_estimators=50, random_state=1)
model.fit(Xtr,ytr)
predictions = model.predict(XT)
print("The mean absolute error for Random Forest Regressor is: {:.2f}".format(mean_absolute_error(yT, predictions)))
model = XGBRegressor(n_estimators=1000, learning_rate=0.05)
model.fit(Xtr, ytr, early_stopping_rounds=5, eval_set=[(XT, yT)], verbose=False)
predictions = model.predict(XT)
print("The mean absolute error for XGB Regressor is: {:.2f}".format(mean_absolute_error(yT, predictions)))
model=GradientBoostingRegressor()
model.fit(Xtr, ytr)
predictions = model.predict(XT)
print("The mean absolute error for Gradient Boosting Regressor is: {:.2f}".format(mean_absolute_error(yT, predictions)))