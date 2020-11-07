import numpy as np

from scipy.io import wavfile
samplerate, data = wavfile.read('FisiereWav/19.wav')

wavFileInfo = open("wafeInfo.txt", "a")
wavFileInfo.write(str(samplerate)+'\n')
wavFileInfo.write(str(data.size)+'\n')
wavFileInfo.close()

print(samplerate)
print(data.size)
print (data)

np.savetxt("waveData.txt", data, fmt="%2.0f")
