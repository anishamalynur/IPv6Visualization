print("in csv to JSON")

#import CSV

#With open('smallDagTest.csv', 'rb') as f:
#	reader = csv.reader(f)
#	for row in reader:
#		print row


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

#level, nodeVal, #of Nodes, node#, rest are next level nodes
import csv
import json

csvfile = open('dagInfo4.csv', 'r')
jsonfile = open('file5.json', 'w')

#sample = {'ObjectInterpolator': 1629,  'PointInterpolator': 1675, 'RectangleInterpolator': 2042}
data = {}

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
	print "TOP OF THE LOOP\n"
	print line
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
		print ("populating dataArray1 with" + line)
		dataArray1.append(line.strip(',\n').split(','))
		print( dataArray1)
	elif int(theLine[0]) == nextLevel:
		print ("populating dataArray2 with" + line)
		dataArray2.append(line.strip(',\n').split(','))
		print( dataArray2)
	else:
		
		nextline = line.strip(',\n').split(',')
		print "next level is" + str(nextline)
		print ("currentLevel is " + str(currentLevel))
		print ("dataArray1: " + str(dataArray1))
		print("dataArray2: "+ str(dataArray2))
		for item in dataArray1:
			nodeName = {}
			nodeName['name'] = item[1]
			nameArray.append(nodeName)
			itemSize = len(item)
			for i in range(4, itemSize -1, 2):
			#for i in range(4, itemSize):
				print( "i value before increment" + str(i))
				#i += 1
				print ("i value after increment" + str(i))  
				for item2 in dataArray2:
					if item2[1] == item[i]:
						linkName = {}
						linkName['source'] = int(item[3])
						linkName['target'] = int(item2[3])
						print(" i val" + str(i))
						##print("item size val" + str(itemSize))
						print ("item[i] = " + item[i])
						print ("item[i+1] = " + item[i])
						linkName['value'] = int(item[i+1]) # item2[2]

						#linkName['value'] = int(item2[2])
						linksArray.append(linkName)
								
							
					
		
		currentLevel += 1
		dataArray1 = dataArray2
		print("dataArray1 is now: " + str(dataArray1))
		dataArray2 = []
		dataArray2.append(nextline)
		print("dataArray2 is now: " + str(dataArray2))
		print("curLevel is now: " + str(currentLevel))
print (linksArray)
print (nameArray)
print ("dataArray1")
print (dataArray1)
print ("dataArray2")
print (dataArray2)

	


data['directed'] = True
data['graph'] = {}
data['nodes'] = nameArray
data['links'] = linksArray
data['multigraph'] = False
sample = json.dumps(data)


jsonfile.write(sample)
