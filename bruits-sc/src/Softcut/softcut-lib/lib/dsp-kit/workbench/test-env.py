import numpy as np
import matplotlib.pyplot as plt

arr = np.load('test-env.npy')

print(arr)

plt.plot(arr[0][0])
plt.show()