#!/usr/bin/env python

import rospy
import random
from std_msgs.msg import String
from std_msgs.msg import Int16
from geometry_msgs.msg import Twist
from std_msgs.msg import Float32MultiArray

class Turn(): 
    def __init__(self):
        #creates publisher to turtlebot's actuators
        self.cmd_pub = rospy.Publisher('cmd_vel_mux/input/navi', Twist, queue_size=1)
        self.turnflag_sub = rospy.Subscriber('turnflag', Int16, self.setTurnFlag)
        self.obstacle_subscriber = rospy.Subscriber('obstacles', Float32MultiArray, self.determineTurn)
        self.movement_sub = rospy.Subscriber('movement', String, self.generateMovingTurn)
        self.r = rospy.Rate(250) # 250hz
        self.rightCache = 0
        self.leftCache = 0
        self.turnFlag = 0
        self.move_cmd = Twist()
        self.start()

    #sets the randomly generated turn flag for left or right turns
    def setTurnFlag(self, msg):
        self.turnFlag = msg.data

    #determines if a manual turn needs to be made to avoid obstacles
    def determineTurn(self, msg):
        left = msg.data[0]
        front = msg.data[1]
        right = msg.data[2]

        #if forward direction has obstacle stop
        if(front < .55):
            self.generateTurn()

        #if left is close to obstacle and getting closer turn
        if(left < .4 and left < self.leftCache):
            self.generateTurn()

        #if right is close to obstacle and getting closer turn
        if(right < .4 and right < self.leftCache):
            self.generateTurn()

        self.rightCache = right
        self.leftCache = left

    #randomly generates a left or right turn
    def generateTurn(self):
        #makes a right turn on 1
        if(self.turnFlag == 1):
            angularVelocity = -10
        #makes a left turn on anything else
        else:
            angularVelocity = 10
        self.turn(angularVelocity)

    #publishes a turn command as the robot is stopped   
    def turn(self, factor):
        self.move_cmd.linear.x = 0
        self.move_cmd.angular.z = factor
        self.cmd_pub.publish(self.move_cmd)

    #creates random turns while robot is moving
    def generateMovingTurn(self, msg):
        angularVelocity = random.randint(-1,1)
        self.movingTurn(angularVelocity)

    #publishes a turn command as the robot is moving forward
    def movingTurn(self, factor):
        self.move_cmd.linear.x = 1
        self.move_cmd.angular.z = 0
        self.cmd_pub.publish(self.move_cmd)

    def stopTurn(self, msg):
        self.move_cmd.linear.x = 0
        self.move_cmd.angular.z = factor
        self.cmd_pub.publish(self.move_cmd)

    def start(self):
        while not rospy.is_shutdown():
            self.r.sleep()
        
def main():
    rospy.init_node('Turn')
    try:
        Turn()
    except rospy.ROSInterruptException:
        pass

if __name__ == '__main__':
    main()