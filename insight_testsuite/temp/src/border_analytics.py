import csv
import sys
import datetime
from collections import defaultdict

# function to generate nested dictionary
def nestedDict():
   return defaultdict(nestedDict)

data = nestedDict()
report = nestedDict() 

date_format = '%m/%d/%Y %I:%M:%S %p'

# read input line by line 
with open(sys.argv[1], "rb") as csvFile:
 datareader = csv.reader(csvFile)
 next(datareader) # ignore header
 for row in datareader:
 	Border, Date, Measure, Value = row[3:7]
 	date_obj = datetime.datetime.strptime(Date, date_format)
 	try:
 		data[Border][Measure][date_obj] = int(Value) + data[Border][Measure][date_obj] # total number of crossings
 	except:
 		data[Border][Measure][date_obj] = int(Value) # if there is no key then initialize the value to current record

for (border_key, border_value) in sorted(data.items()):
	for (measure_key, measure_value) in sorted(border_value.items()):
		count = 0.0;
		trailing_tot = 0;
		for (date_key, date_value) in sorted(measure_value.items()):
			if (count == 0):
				report[date_key][date_value][measure_key][border_key] = 0
			else:
				report[date_key][date_value][measure_key][border_key] = int(round(trailing_tot / count)) # calculate the trailing average
			count = count + 1
			trailing_tot = trailing_tot + date_value

fields = ["Border", "Date", "Measure", "Value", "Average"]
					
# unroll the nested dictionary and write to file
with open(sys.argv[2], "wb") as f:
	w = csv.writer(f)
	w.writerow(fields)
	for (date_k, date_v) in sorted(report.items(), reverse = True):
		for (value_k, value_v) in sorted(date_v.items(), reverse = True):
			for (measure_k, measure_v) in sorted(value_v.items(), reverse = True):
				for (border_k, border_v) in sorted(measure_v.items(), reverse = True):
					w.writerow([border_k, date_k.strftime(date_format), measure_k, value_k, border_v])
