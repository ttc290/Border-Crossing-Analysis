#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <map>
#include <cmath>

using namespace std;

int main(int argc, char **argv) {
	//ifstream f_data("../input/Border_Crossing_Entry_Data.csv");
	//ifstream f_data("../insight_testsuite/tests/test_1/input/Border_Crossing_Entry_Data.csv");
	string ifile = argv[1];
	string ofile = argv[2];

	ifstream f_data(ifile.c_str()); // open input file to get data

	const char * delim = ","; // input file is comma-separated
	string fline;

	getline(f_data, fline); // ignore headers
	char * fields[8]; // 8 fields: Port Name,State,Port Code,Border,Date,Measure,Value,Location
	char * time[3]; // time fields: Month, Day, Year

	map<string, map<string, map<int, map<int, map<int, map<string, int> > > > > > record; // each monthly record is represented by columns: Border, Measure, Year, Month, Day, Date, Total Value

	while ( getline(f_data, fline) ) {
		int i = 0;
     	char * dup;
		char * token;
     	dup = strdup(fline.c_str());
		token = strtok(dup, delim);

        // tokenize the line
		while(token != NULL) {
			fields[i] = token;
			i++;
			token = strtok(NULL, delim);
		}

		string border = string(fields[3]); // Border
		string date = string(fields[4]); // Date

		string m_d_y = string(strtok(strdup(date.c_str()), " ")); // parsing mm/dd/yyyy from Date

		int j = 0;
     	char * t_dup;
		char * t_token;
		const char * d_delim = "/"; // time fields are separated by "/"

     	t_dup = strdup(m_d_y.c_str());
		t_token = strtok(t_dup, d_delim);

        // tokenize the time fields
		while(t_token != NULL) {
			time[j] = t_token;
			j++;
			t_token = strtok(NULL, d_delim);
		}

		int month = atoi(time[0]); // Month
		int day = atoi(time[1]); // Day
		int year = atoi(time[2]); // Year

		string measure = string(fields[5]); // Measure
		int value = atoi(fields[6]); // Value

		record[border][measure][year][month][day][date] += value; // add to total monthly value
	}

	map<int, map<int, map<int, map<string, map<int, map<string, map<string, int> > > > > > > report; // organize monthly record by Year, Month, Day, Date, Value, Measure, Border

	// sort by Border
	for (map<string, map<string, map<int, map<int, map<int, map<string, int> > > > > >::iterator b_it = record.begin(); b_it != record.end(); b_it++) {
		
		// sort by Measure
		for (map<string, map<int, map<int, map<int, map<string, int> > > > >::iterator m_it = b_it->second.begin(); m_it != b_it->second.end(); m_it++) {
			double count = 0.0; // the running number of previous months
			int trailing_tot = 0; // the running total of values

			// sort by Year
			for (map<int, map<int, map<int, map<string, int> > > >::iterator y_it = m_it->second.begin(); y_it != m_it->second.end(); y_it++) {

				// sort by Month
				for (map<int, map<int, map<string, int> > >::iterator mo_it = y_it->second.begin(); mo_it != y_it->second.end(); mo_it++) {

					// sort by Day
					for (map<int, map<string, int> >::iterator da_it = mo_it->second.begin(); da_it != mo_it->second.end(); da_it++) {

						// sort by Date
						for (map<string, int>::iterator d_it = da_it->second.begin(); d_it != da_it->second.end(); d_it++) {

							if (count == 0) {
								report[y_it->first][mo_it->first][da_it->first][d_it->first][d_it->second][m_it->first][b_it->first] = 0; // No record of previous month, running monthly average = 0
							} else {
								report[y_it->first][mo_it->first][da_it->first][d_it->first][d_it->second][m_it->first][b_it->first] = round(trailing_tot / count); // calculate running monthly average
							}

							count++;
							trailing_tot += d_it->second;
						}
					}
				}
			}
		} 
	}

	ofstream myfile(ofile.c_str()); // open output file to write data

	myfile << "Border,Date,Measure,Value,Average\n";

	// access monthly record in descending order
	// By Year
	for (map<int, map<int, map<int, map<string, map<int, map<string, map<string, int> > > > > > >::reverse_iterator y_it = report.rbegin(); y_it != report.rend(); y_it++) {

		// By Month
		for (map<int, map<int, map<string, map<int, map<string, map<string, int> > > > > >::reverse_iterator mo_it = y_it->second.rbegin(); mo_it != y_it->second.rend(); mo_it++) {

			// By Day
			for (map<int, map<string, map<int, map<string, map<string, int> > > > >::reverse_iterator da_it = mo_it->second.rbegin(); da_it != mo_it->second.rend(); da_it++) {

				// By Date
				for (map<string, map<int, map<string, map<string, int> > > >::reverse_iterator d_it = da_it->second.rbegin(); d_it != da_it->second.rend(); d_it++) {

					// By Value
					for (map<int, map<string, map<string, int> > >::reverse_iterator v_it = d_it->second.rbegin(); v_it != d_it->second.rend(); v_it++) {

						// By Measure
						for (map<string, map<string, int> >::reverse_iterator m_it = v_it->second.rbegin(); m_it != v_it->second.rend(); m_it++) {

							// By Border
							for (map<string, int>::reverse_iterator b_it = m_it->second.rbegin(); b_it != m_it->second.rend(); b_it++) {

								myfile << b_it->first << "," << d_it->first << "," << m_it->first << "," << v_it->first << "," << b_it->second << endl; // write to file line by line
							}
						}
					} 
				}
			}

		}
	}

	myfile.close();

	return 0;
}