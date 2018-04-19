

#level, nodeVal, #of Nodes, node#, rest are next level nodes
import csv
import sys
import json
import copy

csvfile = open(sys.argv[1], 'r')
jsonfile = open(sys.argv[2], 'w')
#'dagInfo4.csv'

#sample = {'ObjectInterpolator': 1629,  'PointInterpolator': 1675, 'RectangleInterpolator': 2042}
data = {}
CheckDuplicates = []
nameArray =[]
linksArray = []
dataArray1 = []
dataArray2 = []

nameIndex = 0;
linkIndex = 0;
currentLevel = 0;
nextLevel = 0;
useNextLine = 0;
for line in csvfile:
	#print "TOP OF THE LOOP\n"
	#print line
	x = csvfile.tell()
	#if line[0] == "9999999999999":
	#	break
	#if useNextLine:
	nextLevel = currentLevel +1
	#print("nextlevel is " + str(nextLevel))
	#print("currentlevel is " + str(currentLevel))
		
	#print("line[0]  is " + line[0])

	theLine = line.strip(',\n').split(',')
	if int(theLine[0]) == int(currentLevel):
		#print ("populating dataArray1 with" + line)
		dataArray1.append(line.strip(',\n').split(','))
		#print( dataArray1)
	elif int(theLine[0]) == nextLevel:
		#print ("populating dataArray2 with" + line)
		dataArray2.append(line.strip(',\n').split(','))
		#print( dataArray2)
	else:
		
		nextline = line.strip(',\n').split(',')
		#print "next level is" + str(nextline)
		print ("currentLevel is " + str(currentLevel))
		dataArray2Copy = copy.deepcopy(dataArray2)
	
		#print ("dataArray1: " + str(dataArray1))
		#print("dataArray2: "+ str(dataArray2))
	
		for item in dataArray1:
			nodeName = {}
			nodeName['name'] = item[1]
			nameArray.append(nodeName)
			itemSize = len(item)
			for i in range(4, itemSize -1, 2):
				found = False
			#for i in range(4, itemSize):

				for j, item2 in enumerate(dataArray2):
				#for item2 in dataArray2:
					if item2[1] == item[i]:
						dataArray2Copy[j][1] = True;
						linkName = {}
						linkName['source'] = int(item[3])
						linkName['target'] = int(item2[3])
						#print(" i val" + str(i))
						##print("item size val" + str(itemSize))
						#print ("item[i] = " + item[i])
						#print ("item[i+1] = " + item[i])
						linkName['value'] = int(item[i+1]) # item2[2]
		
						#linkName['value'] = int(item2[2])
						linksArray.append(linkName)
						if([item[3],item2[3]] in CheckDuplicates):
							print("ERROR DUPL")

						CheckDuplicates.append([item[3],item2[3]])
						
						found = True
				if(found == False):
					print("ERROR")
		#print(dataArray2Copy)
		for item in dataArray2Copy:
			if(item[0] == "9999999999999"):
				continue
				#break
			if(item[1] != True):
				print("ERROR2")
								
							
					
		
		currentLevel += 1
		dataArray1 = dataArray2
		print("dataArray1 is now: " + str(dataArray1))
		dataArray2 = []
		dataArray2.append(nextline)
		#print("dataArray2 is now: " + str(dataArray2))
		#print("curLevel is now: " + str(currentLevel))
#print (linksArray)
#print (nameArray)
#print ("dataArray1")
#print (dataArray1)
#print ("dataArray2")
#print (dataArray2)

	


data['directed'] = True
data['graph'] = {}
data['nodes'] = nameArray
data['links'] = linksArray
data['multigraph'] = False
sample = json.dumps(data)


jsonfile.write(sample)
