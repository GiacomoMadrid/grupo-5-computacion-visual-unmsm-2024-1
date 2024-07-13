import cv2
import numpy as np
import matplotlib.pyplot as plt

angulo_laser_y_camara = 270
angulo_recorrido = 0

def a_binario_blanco(frame):
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    _, binary = cv2.threshold(gray, 120, 255, cv2.THRESH_BINARY_INV)
    binary = cv2.bitwise_not(binary)
    return binary

def a_binario_rojo(frame):
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    lower_red1 = np.array([0, 50, 200])
    upper_red1 = np.array([30, 255, 255])
    lower_red2 = np.array([129, 50, 200])
    upper_red2 = np.array([180, 255, 255])
    mask1 = cv2.inRange(hsv, lower_red1, upper_red1)
    mask2 = cv2.inRange(hsv, lower_red2, upper_red2)
    mask = cv2.bitwise_or(mask1, mask2)
    binary = cv2.bitwise_not(mask)
    return binary

def tratar_ruido(binary):
    kernel = np.ones((4, 4), np.uint8)
    binary = cv2.erode(binary, kernel, iterations=1)
    binary = cv2.dilate(binary, kernel, iterations=1)
    return binary

def hallar_contornos(binary):
    # Canny edge detection
    canny = cv2.Canny(binary, 100, 200)
    # Find contours
    contours, hierarchy = cv2.findContours(canny, cv2.RETR_TREE, cv2.CHAIN_APPROX_NONE)
    return contours

def media_puntos(frame, contornos):
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
    return mPuntos

def hallar_puntos_3d(puntos, angulo_recorrido):
    global angulo_laser_y_camara
    angle_laser_camara_radians = np.deg2rad(angulo_laser_y_camara)
    angle_recorrido_radians = np.deg2rad(angulo_recorrido)
    puntos_3d = []

    for (x, y) in puntos:
        r = x / np.sin(angle_laser_camara_radians)
        z = np.sqrt(r**2 - x**2)
        x_3d = x * np.cos(angle_recorrido_radians) - z * np.sin(angle_recorrido_radians)
        z_3d = x * np.sin(angle_recorrido_radians) + z * np.cos(angle_recorrido_radians)
        puntos_3d.append([x_3d, y, z_3d])

    return np.array(puntos_3d)



def main():
    global angulo_recorrido
    acum_puntos_3d=[]

    #Open video
    cap = cv2.VideoCapture('video3.mp4')
    frame_count = int(cap.get(cv2.CAP_PROP_FRAME_COUNT))
    angle_per_frame = 360.0 / frame_count
    if not cap.isOpened():
        print("Error: Could not open video.")
        return
    #Process frames
    while cap.isOpened():
        ret, frame = cap.read()
        if not ret:
            break  # End of video

        binary=a_binario_rojo(frame)
        binary=tratar_ruido(binary)
        contours=hallar_contornos(binary)
        puntos_2d = media_puntos(frame,contours)
        puntos_3d = hallar_puntos_3d(puntos_2d,angulo_recorrido)
        acum_puntos_3d.extend(puntos_3d)

        angulo_recorrido+=angle_per_frame
        if angulo_recorrido >= 360:
            break

    # Write 3d points to a txt file
    with open('3d_coordinates.txt', 'a') as file:
        for point in acum_puntos_3d:
            file.write(f'{point[0]},{point[1]},{point[2]}\n')



    all_puntos_3d = acum_puntos_3d
    # Plot the accumulated 3D points using Matplotlib
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')

    x_coords = [point[0] for point in all_puntos_3d]
    y_coords = [-point[1] for point in all_puntos_3d]
    z_coords = [point[2] for point in all_puntos_3d]

    ax.scatter(x_coords, z_coords, y_coords, c='red', marker='o')
    ax.set_title('Accumulated 3D Points')
    ax.set_xlabel('X Coordinate')
    ax.set_ylabel('Z Coordinate')
    ax.set_zlabel('Y Coordinate')
    ax.invert_yaxis()

    plt.show()


if __name__ == "__main__":
    main()