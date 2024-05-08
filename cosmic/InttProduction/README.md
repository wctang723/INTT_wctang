# DST production for INTT
The macros in this directory are copy from "sPHENIX-collabration/macros/InttProduction/". 

## Usage
1. Run steps 
	- ./intt_makelist.sh {type} {runnumber}
		- This will make a intt list, i.e. intt0.list, intt1.list, ...
	- root -l -b -q Fun4All_Intt_Combiner.C
		- This will grap the lists and combine the evt files, and finally make it as a DST. Caution: the output DST will be the INTTRAWHIT DST.
		- One can change the output filename (line 57) of it and also the # of events (the first arguent) one want to produce. 
