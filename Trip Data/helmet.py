import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.metrics import accuracy_score  
from sklearn.tree import DecisionTreeClassifier
from sklearn.ensemble import RandomForestRegressor
from sklearn.metrics import mean_absolute_error
from xgboost import XGBRegressor
from sklearn.ensemble import GradientBoostingRegressor


f = open('train.csv', 'rU')
data=pd.read_csv(f)
mtype = {'Member': 1, 'Casual': 0}
data['Member type'] = [mtype[item] for item in data['Member type']]
y = data["Member type"]
features = ['Duration', 'Start station number', 'End station number', 'Member type']
data = data[features]
data.to_csv("bikes.csv", sep=',', encoding='utf-8', index=False, header=None)
data = data.drop(['Member type'], axis=1)
X = data
Xtr, XT, ytr, yT = train_test_split(X,y)
model = DecisionTreeClassifier(random_state=1)
model.fit(Xtr,ytr)
predictions = model.predict(XT)
print("The accuracy of Decision Tree Regressor is: {:.2f}%".format(accuracy_score(yT, predictions)*100))
print("The mean absolute error for Decision Tree is: {:.2f}".format(mean_absolute_error(yT, predictions)))
model = RandomForestRegressor(n_estimators=50, random_state=1)
model.fit(Xtr,ytr)
predictions = model.predict(XT)
print("The accuracy of Random Forest Regressor is: {:.2f}%".format(accuracy_score(yT, predictions.round())*100))
print("The mean absolute error for Random Forest Regressor is: {:.2f}".format(mean_absolute_error(yT, predictions)))
model = XGBRegressor(n_estimators=1000, learning_rate=0.05)
model.fit(Xtr, ytr, early_stopping_rounds=5, eval_set=[(XT, yT)], verbose=False)
predictions = model.predict(XT)
print("The accuracy of XGB Regressor is: {:.2f}%".format(accuracy_score(yT, predictions.round())*100))
print("The mean absolute error for XGB Regressor is: {:.2f}".format(mean_absolute_error(yT, predictions)))
model=GradientBoostingRegressor()
model.fit(Xtr, ytr)
predictions = model.predict(XT)
print("The accuracy of Gradient Boosting Regressor is: {:.2f}%".format(accuracy_score(yT, predictions.round())*100))
print("The mean absolute error for Gradient Boosting Regressor is: {:.2f}".format(mean_absolute_error(yT, predictions)))