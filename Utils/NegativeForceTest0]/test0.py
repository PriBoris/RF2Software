

import struct

rawFileName = "0.raw"

with open(rawFileName) as f:
    lines = f.readlines()


time = []
positionAbs = []
positionRel = []
force = []

actualTime = 0.0
for line in lines:

	time.append(actualTime)
	actualTime = actualTime + 0.1

	positionAbs.append(struct.unpack('>i', bytes.fromhex(line.split()[0]))[0])

	newPositionRel = float(struct.unpack('>i', bytes.fromhex(line.split()[1]))[0]) / 100.0
	positionRel.append(newPositionRel)

	newForce = float(struct.unpack('>i', bytes.fromhex(line.split()[2]))[0]) / 1000.0
	force.append(newForce)


import numpy as np
import matplotlib.pyplot as plt


plt.figure()

plt.subplot(2,1,1)
plt.plot(time, positionRel)
plt.grid(True)
plt.xlabel("time [s]")
plt.ylabel("position [%]")

plt.subplot(2,1,2)
plt.plot(time, force)
plt.grid(True)
plt.xlabel("time [s]")
plt.ylabel("force [kg]")

plt.show()


