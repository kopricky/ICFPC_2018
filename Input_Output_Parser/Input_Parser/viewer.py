from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
import numpy as np

if __name__=='__main__':
    R = int(input())
    n = int(input())
    nx, ny, nz = np.zeros(n), np.zeros(n), np.zeros(n);
    for i in range(n):
        nx[i], nz[i], ny[i] = map(int, input().split())
    fig = plt.figure()
    ax = Axes3D(fig)
    ax.set_xlabel("X-axis")
    ax.set_ylabel("Y-axis")
    ax.set_zlabel("Z-axis")
    ax.set_xlim(0, R)
    ax.set_ylim(0, R)
    ax.set_zlim(0, R)
    ax.plot(nx, ny, nz, "o")
    plt.show()
