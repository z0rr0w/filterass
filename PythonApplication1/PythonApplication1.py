import matplotlib.pyplot as plt
import numpy as np

# Your peak result
#peaks = np.loadtxt('C:/Users/Me/Documents/Visual Studio 2015/Projects/filterass/ECGScanner/Output.txt')
peaks = np.loadtxt('C:/Users/jacob/source/repos/filterass/ECGScanner/Output.txt')


# Their R-peaks
#rpeaks = np.loadtxt('Rpeak.txt')

x = []
y = []

# Extract your peaks x,y
for point in peaks:
    x.append(point[0])
    y.append(point[1])

x2 = []
y2 = []

# Extract their peaks x,y
'''for point in rpeaks:
    x2.append(point[0])
    y2.append(point[1])'''

# Load filtered heart data
#data = np.loadtxt('C:/Users/Me/Documents/Visual Studio 2015/Projects/filterass/ECGScanner/mwiOut.txt')
data = np.loadtxt('C:/Users/jacob/source/repos/filterass/ECGScanner/mwiOut.txt')


for point in data:
    x2.append(point[0])
    y2.append(point[1])

# Heart data
#plt.plot(data)
plt.plot(x2,'k')
plt.plot(y2,'k')
# Your peaks
plt.scatter(x,y)

# Their peaks
#plt.scatter(x2,y2)
plt.show()