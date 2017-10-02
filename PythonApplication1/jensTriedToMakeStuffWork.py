import matplotlib.pyplot as plt
import numpy as np

# Your peak result
##peaks = np.loadtxt('C:/Users/Jens/Source/Repos/filterass/ECGScanner/Output.txt')
peaks = np.loadtxt('C:/Users/jacob/source/repos/filterass/ECGScanner/Output.txt')


x = []
y = []

# Extract your peaks x,y
for point in peaks:
    x.append(point[0])
    y.append(point[1])

# Load filtered heart data
#data = np.loadtxt('C:/Users/Jens/Source/Repos/filterass/ECGScanner/mwiOut.txt')
data = np.loadtxt('C:/Users/jacob/source/repos/filterass/ECGScanner/mwiOut.txt')

# Heart data
plt.plot(data, color='black',label='Filtered heart data')

# Your peaks
plt.scatter(x,y, color='blue',label='Peaks')

thresh1 = []
thresh2 = []

#thresholds = np.loadtxt('C:/Users/Jens/Source/Repos/filterass/ECGScanner/threshholds.txt')
thresholds = np.loadtxt('C:/Users/jacob/source/repos/filterass/ECGScanner/threshholds.txt')

for point in thresholds:
    thresh1.append(point[0])
    thresh2.append(point[1])


plt.plot(thresh1, color='red',label='Threshold 1')
plt.plot(thresh2, color='orange',label='Threshold 2')

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
#irregs = np.loadtxt('C:/Users/Jens/Source/Repos/filterass/ECGScanner/irreg.txt')
irregs = np.loadtxt('C:/Users/jacob/source/repos/filterass/ECGScanner/irreg.txt')
irrx = []
irry = []

for point in irregs:
    irrx.append(point[0])
    irry.append(point[1])

#plt.scatter(irrx,irry,color='red')

plt.vlines(x=irrx[0],color='green',linestyle='--',ymin=0, ymax=max(data), label='Irregular Heart Rate')
for point in irrx:
    plt.vlines(x=point,color='green',linestyle='--',ymin=0, ymax=max(data))

searchBack = np.loadtxt('C:/Users/jacob/source/repos/filterass/ECGScanner/searchBack.txt')
searchBackx = []

for point in searchBack:
    searchBackx.append(point)

plt.vlines(x=searchBackx[0],color='purple',linestyle='--',ymin=0, ymax=max(data), label='SearchBack initiated')
for point in searchBackx:
    plt.vlines(x=point,color='purple',linestyle='--',ymin=0, ymax=max(data))

plt.legend(loc='upper left')

plt.show()