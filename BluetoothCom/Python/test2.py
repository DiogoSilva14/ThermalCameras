import cv2
import sys
import numpy as np

image = cv2.imread(sys.argv[1])

f = open(sys.argv[1], 'rb')
image_bytes = f.read()

decoded = cv2.imdecode(np.frombuffer(image_bytes, np.uint8), -1)
			
print(type(decoded))

cv2.imshow("Image", decoded)

cv2.waitKey(0)
