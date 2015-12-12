from __future__ import division
from decimal import Decimal, getcontext
getcontext().prec = 320
import math

####
# do not forget to comment out the print statements in balloc, binit and bfree and then giving the program's output to input here
####
# ready for plot on fooplot.com
####



lineCount = 0
cursize = 32
blocksize = 512
currentSum = 0
res = []


with open('results.txt', 'r') as f:
	while(cursize < 65536):
		curRes = []
		while(blocksize < cursize * 1024):
			for i in xrange(0,4):  	
				f.readline()
				f.readline()
				f.readline()
				line = f.readline()
				s = line.split(" ")
				currentSum += float(s[2])
				# print currentSum
			avg = float(currentSum) / float(4)
			curRes.append(avg)
			currentSum = 0
			blocksize *= 2
		res.append(curRes)
		blocksize = 512
		cursize *= 2

cursize = 32
blocksize = 512

for i in xrange(0, len(res)):
	print "cursize: " + str(cursize)
	for j in xrange(0, len(res[i])):
		print str(int(math.log(blocksize,2))) + "," + str(res[i][j])
		blocksize *= 2
	cursize *= 2
	blocksize = 512
	print