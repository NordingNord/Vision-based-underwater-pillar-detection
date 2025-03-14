###############################################
# Standard Imports                            #
###############################################
import pywt
import numpy as np
import matplotlib.pyplot as plt
import math
import statistics 
import cv2 as cv
import time
import csv
import ellipsis as el
import open3d as o3d

# DLT from: https://temugeb.github.io/opencv/python/2021/02/02/stereo-camera-calibration-and-triangulation.html
# Used for comparison to my own c++ implementation
# def DLT(P1, P2, point1, point2):

#     A = [point1[1]*P1[2,:] - P1[1,:],
#          P1[0,:] - point1[0]*P1[2,:],
#          point2[1]*P2[2,:] - P2[1,:],
#          P2[0,:] - point2[0]*P2[2,:]
#         ]
#     A = np.array(A).reshape((4,4))

#     B = A.transpose() @ A
#     from scipy import linalg
#     U, s, Vh = linalg.svd(B, full_matrices = False)
#     print(Vh)

#     print('Triangulated point: ')
#     print(Vh[3,0:3]/Vh[3,3])
#     return Vh[3,0:3]/Vh[3,3]

# intrinsic_1 = np.array([[0.35520833333*1.4573042096686729e+03, 0., 0.35520833333*(5.6118918886024528e+02+0.5)-0.5],[0.,0.35520833333*1.4571772302602453e+03, 0.35520833333*(9.4238009607042807e+02+0.5)-0.5],[0., 0., 1. ]])
# intrinsic_2 = np.array([[0.35520833333*1.4559082053767202e+03, 0., 0.35520833333*(5.6422591354649398e+02+0.5)-0.5],[0.,0.35520833333*1.4557162795186625e+03, 0.35520833333*(9.4971024389872707e+02+0.5)-0.5],[ 0., 0., 1.]])

# rotation_1 = np.eye(3)
# rotation_2 = np.array([[9.9999025308859502e-01, 1.3210364715320931e-03, -4.2129076003026055e-03],[-1.3230173719132052e-03, 9.9999901555929249e-01, -4.6744569710903378e-04],[4.2122859401305226e-03, 4.7301489089870296e-04, 9.9999101641168364e-01]])

# translation_1 = np.array([[0],[0],[0]])
# translation_2 = np.array([[-6.0232602731165007e-02],[5.0696392732741975e-04],[-2.1759794874252371e-04]])

# RT1 = np.concatenate([rotation_1,translation_1],axis=-1)
# RT2 = np.concatenate([rotation_2,translation_2],axis=-1)

# P1 = intrinsic_1 @ RT1
# P2 = intrinsic_2 @ RT2

# point_1 = np.array([31.7902,35.6918])
# point_2 = np.array([51.9658,85.7148])
# p3d = DLT(P1,P2,point_1,point_2)

# first_projection = np.array([[1437.368512, 0.0, 573.360367, -86.409523], [0.0, 1437.368512, 922.768173, 0.0], [0.0, 0.0, 1.0, 0.0]])
# second_projection = np.array([[1437.368512, 0.0, 573.360367, 0.0],[0.0, 1437.368512, 922.768173, 0.0],[0.0, 0.0, 1.0, 0.0]])

# first_point = np.array([235.007416, 664.029175])
# second_point = np.array([183.132416, 664.029175])

# Q = cv.triangulatePoints(first_projection, second_projection, first_point.T, second_point.T)
# Q = np.transpose(Q[:3] / Q[3])

# print(Q)

# -- Read 3d data --
x_coordinates = []
y_coordinates = []
z_coordinates = []
points_3d = []
colors = []

with open('points_3d_good.csv',mode = 'r') as file:
    csv_file = csv.DictReader(file)
    for line in csv_file:
        x_coordinates.append(float(line.get("x")))
        y_coordinates.append(float(line.get("y")))
        z_coordinates.append(float(line.get("z")))

        point_3d = []
        point_3d.append(float(line.get("x")))
        point_3d.append(float(line.get("y")))
        point_3d.append(float(line.get("z")))
        points_3d.append(np.array(point_3d))

        color = []
        color.append(float(line.get("r"))/255.0)
        color.append(float(line.get("g"))/255.0)
        color.append(float(line.get("b"))/255.0)
        colors.append(np.array(color))
colors = np.array(colors)
points_3d = np.array(points_3d)


# -- point cloud --
point_cloud = o3d.geometry.PointCloud()
point_cloud.points = o3d.utility.Vector3dVector(points_3d)
point_cloud.colors = o3d.utility.Vector3dVector(colors)
o3d.visualization.draw_geometries([point_cloud])


# -- 3D plot --
# figure_3d = plt.figure()
# axis = plt.axes(projection='3d')
# print(len(colors))
# print(len(x_coordinates))
# axis.scatter(x_coordinates,y_coordinates,z_coordinates,)
# axis.set_title('3D estimation')
# axis.set_xlabel('x', fontsize=12)
# axis.set_ylabel('y', fontsize=12)
# axis.set_zlabel('z', fontsize=12)
# plt.show()
