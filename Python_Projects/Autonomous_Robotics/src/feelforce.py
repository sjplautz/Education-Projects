#!/usr/bin/env python

import rospy
from std_msgs.msg import String
from std_msgs.msg import Float32MultiArray
from sensor_msgs.msg import LaserScan

class ForceMapper(): 
    def __init__(self):
        self.r = rospy.Rate(250) # 250hz
        self.thresh = 1
        self.sub = rospy.Subscriber('/scan', LaserScan, self.scan)
        self.pub_obstacles = rospy.Publisher('obstacles', Float32MultiArray, queue_size=3)
        self.obstacleArray = Float32MultiArray()
        self.start()
       
    def start(self):
        while not rospy.is_shutdown():
            self.pub_obstacles.publish(self.obstacleArray)        
            self.r.sleep()

    def scan(self, msg):
        size = len(msg.ranges)
        #accounts for ranges to improve pathing
        left = self.minimum(417, 639, msg)
        front = self.minimum(214, 416, msg)
        right = self.minimum(0, 213, msg)
        self.obstacleArray.data = [left, front, right]

    def minimum(self, start, end, msg):
        smallest = 10.0
        index = start
        while(index <= end):
            if(smallest > msg.ranges[index]):
                smallest = msg.ranges[index]
            index += 1
        return smallest

        
def main():
    rospy.init_node('ForceMapper')
    try:
        force = ForceMapper()
    except rospy.ROSInterruptException:
        pass

if __name__ == '__main__':
    main()