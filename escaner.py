import cv2
import numpy as np

angulo_laser_y_camara=45
angulo_recorrido=0

def a_binario(frame):
    # To binary
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    lower_red = np.array([159, 50, 200])    
    upper_red = np.array([180, 255, 255]) 
    binary= cv2.inRange(hsv, lower_red, upper_red)
    binary = cv2.bitwise_not(binary)
    #Treat noise
    kernel = np.ones((2, 2), np.uint8)
    binary = cv2.erode(binary, kernel, iterations=1)
    binary = cv2.dilate(binary, kernel, iterations=1)
    #cv2.imshow("Imagen binaria tratada",binary)
    #cv2.waitKey(0) 
    return binary

def hallar_contornos(binary):
    # Canny edge detection
    canny = cv2.Canny(binary, 100, 200)
    #cv2.imshow("Bordes",canny)
    #cv2.waitKey(0) 
    # Find contours
    contours, hierarchy = cv2.findContours(canny, cv2.RETR_TREE, cv2.CHAIN_APPROX_NONE)
    return contours

def media_puntos(frame,contornos):
    mPuntos = {}
    if len(contornos) > 0:
        for contour in contornos:
            for point in contour:
                x, y = point[0]
                if y in mPuntos:
                    mPuntos[y] = (mPuntos[y] + x) / 2
                else:
                    mPuntos[y] = x
    mPuntos = [(x, y) for y, x in mPuntos.items()]
    # height, width = frame.shape[:2]
    # visualization = np.zeros((height, width, 3), dtype=np.uint8)
    # for x,y in mPuntos:
    #     cv2.circle(visualization, (int(x), int(y)), 1, (0, 255, 0), -1)
    # cv2.imshow("Puntos 2D",visualization)
    # cv2.waitKey(0) 
    return mPuntos

def hallar_puntos_3d(puntos):
    global angulo_laser_y_camara,angulo_recorrido
    angle_laser_camara_radians = np.deg2rad(angulo_laser_y_camara)
    angle_recorrido_radians = np.deg2rad(angulo_recorrido)
    with open('3d_coordinates.txt', 'a') as file:
        for (x, y) in puntos:
            r = x / np.sin(angle_laser_camara_radians)
            z = np.sqrt(r**2 - x**2)
            x_3d = x * np.cos(angle_recorrido_radians) - z * np.sin(angle_recorrido_radians)
            z_3d = x * np.sin(angle_recorrido_radians) + z * np.cos(angle_recorrido_radians)
            # Write coordinates to file
            file.write(f'{x_3d},{y},{z_3d}\n')




def main():
    global angulo_recorrido

    #USING A VIDEO
    cap = cv2.VideoCapture('video.mp4')
    frame_count = int(cap.get(cv2.CAP_PROP_FRAME_COUNT))
    angle_per_frame = 360.0 / frame_count

    if not cap.isOpened():
        print("Error: Could not open video.")
        return

    while cap.isOpened():
        ret, frame = cap.read()
        if not ret:
            break  # End of video

        #USING AN IMAGE
        #frame = cv2.imread("imagen1.png")

        binary=a_binario(frame)
        contours=hallar_contornos(binary)
        puntos_2d = media_puntos(frame,contours)
        hallar_puntos_3d(puntos_2d)

        angulo_recorrido+=angle_per_frame
        if angulo_recorrido >= 360:
            break



if __name__ == "__main__":
    main()
