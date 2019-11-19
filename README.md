# Border Crossing Analysis

## Table of Contents
1. [Problem](README.md#problem)
1. [The approach](README.md#the-approach)
1. [Expected output](README.md#expected-output)
1. [How to run the program](README.md#how-to-run-the-program)
1. [Tips on getting an interview](README.md#tips-on-getting-an-interview)
1. [Repo directory structure](README.md#repo-directory-structure)

## Problem

The Bureau of Transportation Statistics regularly makes available [data](https://data.transportation.gov/api/views/keg4-3bc2/rows.csv?accessType=DOWNLOAD) on the number of vehicles, equipment, passengers and pedestrians crossing into the United States by land.

For this dataset, I calculated the total number of times vehicles, equipment, passengers and pedestrians cross the U.S.-Canadian and U.S.-Mexican borders each month. I also want to know the running monthly average of total number of crossings for that type of crossing and border.

## The approach

Data is read from input file line by line. `Border`, `Date`, `Measure` are used as keys in a nested dictiionary with `Value` being the total number of crossings for its respective key. A second nested dictionary (being nested in the order of `Date`, `Value`, `Measure`, `Border`) is created to keep track of the `Average` value.

## Expected output

Output is sorted in descending order by 
* `Date`
* `Value` (or number of crossings)
* `Measure`
* `Border`

The column, `Average`, is for the running monthly average of total crossings for that border and means of crossing in all previous months.

For example, the output file `report.csv` may look like this:

```
Border,Date,Measure,Value,Average
US-Mexico Border,03/01/2019 12:00:00 AM,Pedestrians,346158,114487
US-Canada Border,03/01/2019 12:00:00 AM,Truck Containers Full,6483,0
US-Canada Border,03/01/2019 12:00:00 AM,Trains,19,0
US-Mexico Border,02/01/2019 12:00:00 AM,Pedestrians,172163,56810
US-Canada Border,02/01/2019 12:00:00 AM,Truck Containers Empty,1319,0
US-Mexico Border,01/01/2019 12:00:00 AM,Pedestrians,56810,0

```

## How to run the program

The program is written in Python version 2.7. Simply run `run.sh` to create the output file `report.csv` in the output folder.

## Repo directory structure

The directory structure for the repo should look like this:

    ├── README.md
    ├── run.sh
    ├── src
    │   └── border_analytics.py
    ├── input
    │   └── Border_Crossing_Entry_Data.csv
    ├── output
    |   └── report.csv
    ├── insight_testsuite
        └── run_tests.sh
        └── tests
            └── test_1
            |   ├── input
            |   │   └── Border_Crossing_Entry_Data.csv
            |   |__ output
            |   │   └── report.csv
