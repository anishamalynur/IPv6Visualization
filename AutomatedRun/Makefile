

dag: addressDag.C
	python ipasn.py first1000Lines.txt 
	python sameDomain.py ASNip 
	g++ -std=c++11 -g addressDag.C
	./run.sh

clean:
	rm *.dat
