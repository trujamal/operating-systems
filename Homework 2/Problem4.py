import random
import sys
import math

# Using the rand() function
# (that returns a random number uniformly distributed between 
# 0 and RANDM AX), write a simple program the generates the arrival 
# times of 1000 processes (i.e., when each process arrives) that follow a 
# Poisson distribution with an average arrival rate poisson of 10 processes per
# second. Submit the arrival times of the first 10 processes and the actual 
# average arrival rate over the 1000 processes. [Hint 1: A Poisson arrivals
# means Exponential inter-arrival times. Hint 2: Use the CDF of Exponential
# Distribution.] [10 pts]

# Initialize array
randomValArr = []
smallerValArr = []
sizeL = 1000
sizeS = 10

def mathHandler(random_number, lambda_value):
    return -math.log(1-random_number)/lambda_value

# Run the 10 cases
for i in range(0,sizeS):
    our_randomNumber = float(random.randint(0,sys.maxsize))
    our_randomNumber = our_randomNumber/sys.maxsize
    smallerValArr.append(mathHandler(our_randomNumber,10))

print("Arrival Times")
print
print(smallerValArr)
print

# Running 1000 cases then taking the average.
for i in range(0,sizeL):
    our_randomNumber = float(random.randint(0,sys.maxsize))
    our_randomNumber = our_randomNumber/sys.maxsize
    smallerValArr.append(mathHandler(our_randomNumber,10))

resultingavg = (sum(smallerValArr)/sizeL) * 100

print("Average Arrival Rate")
print
print(resultingavg)


print("Program Completed")