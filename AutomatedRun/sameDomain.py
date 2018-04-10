

import sys




inputFile = open(sys.argv[1], 'r')

domains = {}

for line in inputFile:

	line = line.strip(',\n');

	if(line[:4] in domains):
		domains[line[:4]].append(line)
	else:
		domains[line[:4]] = [line]

print(domains)

notFinished = True

count = 0

for key in domains:
	outFile = open(str(count) + ".dat", 'w')
	for item in domains[key]:
		outFile.write(item + '\n')
	count += 1
	outFile.close()

print(count)
		
