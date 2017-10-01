import matplotlib.pyplot as plt
import numpy as np

# Your peak result
peaks = np.loadtxt('C:/Users/Jens/Source/Repos/filterass/ECGScanner/Output.txt')

x = []
y = []

# Extract your peaks x,y
for point in peaks:
    x.append(point[0])
    y.append(point[1])

# Load filtered heart data
data = np.loadtxt('C:/Users/Jens/Source/Repos/filterass/ECGScanner/mwiOut.txt')

# Heart data
plt.plot(data, color='green')

# Your peaks
plt.scatter(x,y, color='blue')

thresh1 = []
thresh2 = []

thresholds = np.loadtxt('C:/Users/Jens/Source/Repos/filterass/ECGScanner/threshholds.txt')

for point in thresholds:
    thresh1.append(point[0])
    thresh2.append(point[1])


plt.plot(thresh1, color='orange')
plt.plot(thresh2, color='purple')

'''unstable = np.loadtxt('unstable.txt')

ux = []
uy = []

for point in unstable:
    ux.append(point[0])
    uy.append(point[1])

plt.scatter(ux,uy, color='red')

searchBack = np.loadtxt('searchBack.txt')



for points in searchBack:
	x1, y1 = [points[0], points[1]], [points[2],points[3]]
	plt.plot(x1, y1, color='pink', marker='o')'''

plt.show()