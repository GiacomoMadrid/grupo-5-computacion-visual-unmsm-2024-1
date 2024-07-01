# USING RED
    # To binary
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    lower_red = np.array([0, 150, 150])    
    upper_red = np.array([10, 255, 255]) 
    binary= cv2.inRange(hsv, lower_red, upper_red)
    binary = cv2.bitwise_not(binary)
    #Treat noise
    kernel = np.ones((2, 2), np.uint8)
    binary = cv2.erode(binary, kernel, iterations=1)
    binary = cv2.dilate(binary, kernel, iterations=1)
    cv2.imshow("Imagen binaria tratada",binary)
    cv2.waitKey(0) 