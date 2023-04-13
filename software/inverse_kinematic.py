import ikpy.chain
import ikpy.utils.plot as plot_utils

import numpy as nps
import math

import matplotlib.pyplot as plt
import serial

rail_lower_limit = 0.0
rail_upper_limit = 0.8
robot_height = 0.735
robot_base_height = 0
z_height = 0.751

# change this to the right port
ser = serial.Serial("/dev/cu.usbmodem211401", 115200, timeout=1)


robot_arm_chain = ikpy.chain.Chain.from_urdf_file("raspsp1_no_rail.urdf",active_links_mask=[False,True,True,True,True,True,False,])
target_position = [0, 0, 0.751] #starting position
target_orientation = [0, 1, 0] #starting orientation
rail_position = 0.0
ik = [0] * 7

def get_defualt_position():
	global ik
	position = robot_arm_chain.forward_kinematics([0] * 7)
	print("Initial Position: [%.3f, %.3f, %.3f]" % (position[0][3], position[1][3], position[2][3]))
	ik = robot_arm_chain.inverse_kinematics((position[0][3],position[1][3],position[2][3]), target_orientation)
	
# move only the arm
def move_arm(x,y,z):
	global target_position
	global ik

	target_position = [x, y, z]
	ik = robot_arm_chain.inverse_kinematics(target_position, target_orientation, orientation_mode=None)
	print("IK Position: ", ik)

	position = robot_arm_chain.forward_kinematics(ik)
	print("FK Position: [%.3f, %.3f, %.3f]" % (position[0][3], position[1][3], position[2][3]))
	# print("The angles of each joints are : ", list(map(lambda r:math.degrees(r),ik.tolist())))
	print()
	plot(ik)


# move the arm and the rail
def move(x,y,z):
	global target_position
	global ik

	r, x, y, z = rail_sweep(x, y, z)

	print("Rail Position: ", r)
	print("Arm Position: [%.3f, %.3f, %.3f]" % (x, y, z))


	target_position = [x, y, z]
	ik = robot_arm_chain.inverse_kinematics(target_position, target_orientation, orientation_mode=None)
	print("IK Position: ", ik)

	position = robot_arm_chain.forward_kinematics(ik)
	# print("FK Position: [%.3f, %.3f, %.3f]" % (position[0][3], position[1][3], position[2][3]))
	# print("The angles of each joints are : ", list(map(lambda r:math.degrees(r),ik.tolist())))
	plot(ik)
	motor_steps = ik_to_steps(ik)
	rail_steps = r * 40000
	command = "G01 %d %d %d %d %d %d\n" % (motor_steps[0], motor_steps[1], motor_steps[2], motor_steps[3], motor_steps[4], rail_steps)
	ser.write(command.encode("ASCII"))


# calculate the optimal position for the rail, such that the arm can reach desired position
def rail_sweep(x, y, z):
	intervals = nps.linspace(rail_lower_limit,rail_upper_limit, num = 161)
	fk_array = []
	for i in intervals:
		sweep_position = [x - i, y, z]
		ik_t = robot_arm_chain.inverse_kinematics(sweep_position, target_orientation, orientation_mode=None)
		fk_t = robot_arm_chain.forward_kinematics(ik_t)
		fk_position = [i, fk_t[0][3], fk_t[1][3], fk_t[2][3]] # [rail, x, y, z]

		fk_array.append(fk_position)

	# print("FK Unsort: ", fk_array)

	# sort by distance from upright position
	# fk_array = sorted(fk_array, key=lambda k: k[1] ** 2 + k[2] ** 2 + (k[3] - z_height) ** 2)

	#sort by distance to target position
	fk_array = sorted(fk_array, key=lambda k: (k[0] + k[1] - x) ** 2 + (k[2] - y) ** 2 + (k[3] - z) ** 2)
	
	# print("FK Sorted: ", fk_array)
	print(fk_array[0])
	return fk_array[0]


def plot(ik):
	fig, ax = plot_utils.init_3d_figure()
	fig.set_figheight(9)  
	fig.set_figwidth(13)  
	robot_arm_chain.plot(ik, ax, target=target_position)
	plt.xlim(-0.5, 0.5)
	plt.ylim(-0.5, 0.5)
	ax.set_zlim(0, 0.6)
	plt.ion()
	plt.waitforbuttonpress()
	plt.close()


def get_position_input():
	x, y, z = map(float, input("Enter X,Y,Z coordinate: ").split())
	# print("Distance: ", distance)
	# if (distance > robot_height):
	# 	print("Warning: This position might be outside the motion range of the arm") # need to fix this

	if (abs(x) < 0.0001):
		x = 0.0001
	
	if (abs(y) < 0.0001):
		y = 0.0001

	if (abs(z) < 0.0001):
		z = 0.0001
	return x,y,z

def get_waypoint():
	n = int(input("Please Specify the number of waypoint coordinates: "))
	coordinates = []
	system_position = [] #[rail, x, y, z]

	for i in range(n):
		x, y, z = get_position_input()
		coordinates.append([x,y,z])
		system_position.append(rail_sweep(x,y,z))
	
	return system_position


def ik_to_steps(ik_rad):
	ik_deg = nps.rad2deg(ik_rad)
	upper_wrist = (ik_deg[5] * 420) / 90
	lower_wrist = (ik_deg[4] * -50) / 90
	elbow = (ik_deg[3] * 2200) / 90
	shoulder = (ik_deg[2] * 2000) / 90
	base = (ik_deg[1] * 4100) / 90

	#rail = 200 steps / 5 mm

	print("upper_wrist: ", upper_wrist)
	print("lower_wrist: ", lower_wrist)
	print("elbow: ", elbow)
	print("shoulder: ", shoulder)
	print("base", base)

	return([upper_wrist,lower_wrist,elbow,shoulder,base])

if __name__ == "__main__":
	# get_defualt_position()
	#move(0.1, 0, 0.6)

	# rail_sweep(0.5, 0, 0.751)

	# print(get_waypoint())
	command = "G01 0 0 0 0 0 0\n"
	ser.write(command.encode("ASCII"))
	while(True):
		x, y, z = get_position_input()
		move(x,y,z)

		# fk = rail_sweep(x, y, z)
		# ik = robot_arm_chain.inverse_kinematics(fk[1:4],target_orientation,orientation_mode = None)
		# plot(ik)

		# rail_steps = fk[0] * 40000
		# print("rail: ",rail_steps)
		# ik_to_steps(ik)

