print("in csv to JSON")

#import CSV

#With open('smallDagTest.csv', 'rb') as f:
#	reader = csv.reader(f)
#	for row in reader:
#		print row

import csv 
import sys
 

#f = open('smallDagTest.csv', 'rb')
#reader = csv.reader(f)
#for row in reader:
#	print row
 
#f.close()


#with open('smallDagTest.csv') as f:
	
#	content = f.readline()
#	content2 = f.readline()
	#content2.append(f.readline())
# you may also want to remove whitespace characters like `\n` at the end of each line
#content2 = [x.strip() for x in content2] 
#content2 = content2.split(",")
#content2 = content2.split("|")
#content2[2] = content2[2].split("[]");
#print content
#print "hi"
#print content2

#print content2[2]


import csv
import json

csvfile = open('dagInfo.csv', 'r')
jsonfile = open('file.json', 'w')

#sample = {'ObjectInterpolator': 1629,  'PointInterpolator': 1675, 'RectangleInterpolator': 2042}
data = {}
arra = [{"source": 0, "target": 1, "value": 2.0},
{"source": 0, "target": 2, "value": 2.0},
{"source":1 , "target": 3, "value": 2.0},
{"source": 2, "target": 3, "value": 2.0},
{"source": 4, "target": 1, "value": 1.0}]

nodes = [
{"name": "W"},
 {"name": "I"},
 {"name": "B"},
 {"name": "S"},
 {"name": "0"}
 ]

nameArray =[]
linksArray = []
dataArray1 = []
dataArray2 = []

nameIndex = 0;
linkIndex = 0;
currentLevel = 0;

for line in csvfile:
	print line
	x = csvfile.tell()
	#if line[0] == "9999999999999":
	#	break
	#print "top of loop"	
	if int(line[0]) == int(currentLevel):
		print "populating dataArray1 with" + line
		dataArray1.append(line.strip(',\n').split(','))
	elif int(line[0]) == int(currentLevel) + 1:
		print "populating dataArray2 with" + line
		dataArray2.append(line.strip(',\n').split(','))
	else:
	
		nextline = line.strip(',\n').split(',')
		print "currentLevel is " + str(currentLevel)
		for item in dataArray1:
			nodeName = {}
			nodeName['name'] = item[1]
			nameArray.append(nodeName)
			itemSize = len(item)
			for i in range(4, itemSize):  
				for item2 in dataArray2:
					if item2[1] == item[i]:
						linkName = {}
						linkName['source'] = int(item[3])
						linkName['target'] = int(item2[3])
						linkName['value'] = int(item2[2])
						linksArray.append(linkName);	
					
		
		currentLevel += 1
		dataArray1 = dataArray2
		dataArray2 = []
		dataArray2.append(nextline)
		print linksArray
		print nameArray
		print "dataArray1"
		print dataArray1
		print "dataArray2"
		print dataArray2

	


data['directed'] = True
data['graph'] = {}
data['nodes'] = nameArray
data['links'] = linksArray
data['multigraph'] = False
sample = json.dumps(data)


jsonfile.write(sample)





