import numpy as np
import pandas as pd
from sklearn.model_selection import train_test_split
from tensorflow.python import keras
from tensorflow.python.keras.models import Sequential
from tensorflow.python.keras.layers import Dense, Flatten, Conv2D

dataset =  pd.read_csv('train.csv')
Ytr = keras.utils.to_categorical(dataset.label,10)
Xtr = dataset.values[:,1:]
nrows = Xtr.shape[0]
Xtr = Xtr.reshape(nrows, 28, 28, 1)
Xtr = Xtr/255
model = Sequential()
model.add(Conv2D(20, kernel_size=(3, 3), activation='relu', input_shape=(28, 28, 1)))
model.add(Conv2D(20, kernel_size=(3, 3), activation='relu'))
model.add(Flatten())
model.add(Dense(128, activation='relu'))
model.add(Dense(10, activation='softmax'))
model.compile(loss=keras.losses.categorical_crossentropy, optimizer='adam', metrics=['accuracy'])
model.fit(Xtr, Ytr, batch_size=128, epochs=2, validation_split = 0.2)