#!/usr/bin/env python

import rospy
import random
from std_msgs.msg import String
from std_msgs.msg import Int16
from std_msgs.msg import Float32MultiArray
from geometry_msgs.msg import Twist

class Foward(): 
    def __init__(self):
        #creates a publisher to the turtlebot's actuators
        self.cmd_pub = rospy.Publisher('cmd_vel_mux/input/navi', Twist, queue_size=1)
        self.moving_pub = rospy.Publisher('movement', String, queue_size=1)
        self.turn_pub = rospy.Publisher('turnflag', Int16, queue_size=1)
        self.obstacle_sub = rospy.Subscriber('obstacles', Float32MultiArray, self.determineStop)
        self.r = rospy.Rate(250) # 250hz
        self.flag = -1
        self.leftCache = 0
        self.rightCache = 0
        self.move_cmd = Twist()
        self.start()

    #determines if turtlebot needs to stop moving forward at current trajectory
    def determineStop(self, msg):
        left = msg.data[0]
        front = msg.data[1]
        right = msg.data[2]

        #if forward direction has obstacle stop
        if(front < .55):
            self.stop()

        #if left is close to obstacle and getting closer turn
        elif(left < .4 and left < self.leftCache):
            self.stop()

        #if right is close to obstacle and getting closer turn
        elif(right < .4 and right < self.rightCache):
            self.stop()
        
        #none of the issues have arisen so we can move now
        else:
            self.move(1)

        self.rightCache = right
        self.leftCache = left

    #moves the robot at the linear speed given
    def move(self, value):
        self.turnFlag = random.randint(0,2)
        self.move_cmd.linear.x = value
        self.turn_pub.publish(self.turnFlag)
        self.moving_pub.publish("")
        self.cmd_pub.publish(self.move_cmd)

    def stop(self):
        self.move_cmd.linear.x = 0.0
        self.cmd_pub.publish(self.move_cmd)

    def start(self):
        while not rospy.is_shutdown():
            self.r.sleep()
        
def main():
    rospy.init_node('Forward')
    try:
        Foward()
    except rospy.ROSInterruptException:
        pass

if __name__ == '__main__':
    main()