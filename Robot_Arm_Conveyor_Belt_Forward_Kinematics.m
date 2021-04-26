%Alexander Gruhl
%April 24, 2021
%Caculations of x, y, z, and end effector angle of robot manipulator using
%DH forwared kinematics
%For full calculations please see DH calculations image

%link lengths of robot manipulator
l1 = 5;
l2 = 15.5;
l3 = 12;
l4 = 14.5;

%theta angles of each joint (thetas in degrees)
theta_1 = deg2rad(45);
theta_2 = deg2rad(64);
theta_3 = deg2rad(-110);
theta_4 = deg2rad(46);

%x, y, and z position of end effector
dx = cos(theta_1)*(l2*cos(theta_2)+l3*cos(theta_2+theta_3)) + l4*cos(theta_1)*cos(theta_2+theta_3+theta_4);
dy = sin(theta_1)*(l2*cos(theta_2)+l3*cos(theta_2+theta_3)) + l4*sin(theta_1)*cos(theta_2+theta_3+theta_4);
dz = l1 + l2*sin(theta_2) + l3*sin(theta_2+theta_3) + l4*sin(theta_2+theta_3+theta_4);

%end effector position (phi in degrees)
phi = theta_2 + theta_3 + theta_4;
phi = rad2deg(phi);

%output of x, y, z, and end effect angle values
display(dx)
display(dy)
display(dz)
display(phi)
