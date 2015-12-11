from __future__ import division
from decimal import Decimal, getcontext

getcontext().prec = 320

lineCount = 0
cursize = 32
blocksize = 512
currentSum = 0
res = []

with open('results.txt', 'r') as f:
	while(cursize < 65536):
		curRes = []
		while(blocksize < cursize * 1024):
		    for i in xrange(0,5):
		    	
		    	f.readline()
		    	f.readline()
		    	f.readline()
		    	line = f.readline()
	    		s = line.split(" ")
	    		currentSum += Decimal(s[2])
			avg = Decimal(currentSum) / Decimal(5)
			curRes.append(avg)
			currentSum = 0
		    blocksize *= 2
		res.append(curRes)
		blocksize = 512
		cursize *= 2


for i in xrange(0, len(res)):
	for j in xrange(0, len(res[i])):
		print res[i][j],
	print