"""
Script for parsing temperature data,
constructing the graph and writting it
to file.

@author: Dejan Josifovic
"""

import csv
from datetime import datetime
from matplotlib.dates import DateFormatter
import matplotlib.pyplot as plt

# variables
INPUT_FILE = '../TEMP.TXT'
OUTPUT_FILE_PREFIX = '../temp_'
OUTPUT_FILE_TYPE = '.png'
INPUT_DATE_FORMAT = '%Y-%m-%d %H:%M:%S'
OUTPUT_DATE_FORMAT = '%Y-%m-%d %H:%M'
PLOT_WIDTH = 15
PLOT_HIGHT = 15

# global lists for parsed data
cycle_data = []
temp_data = []
raw_time_data = []


# parse single csv row
def parse_row(row):
    cycle_data.append(row[0])
    raw_time_data.append(row[1])
    temp_data.append(row[2])


# read csv file row by row
def read():
    with open(INPUT_FILE, 'r') as f:
        reader = csv.reader(f, delimiter=';')
        for row in reader:
            parse_row(row)


# construct graph and write it to file
def plot_graph():
    t_time_data = [datetime.strptime(date, INPUT_DATE_FORMAT) for date in raw_time_data]

    fig, ax = plt.subplots(figsize=(PLOT_WIDTH, PLOT_HIGHT))
    ax.plot_date(t_time_data, temp_data, '-')

    ax.xaxis.set_major_formatter(DateFormatter(OUTPUT_DATE_FORMAT))
    ax.autoscale_view()
    ax.grid(True)

    fig.autofmt_xdate()
    plt.title('Temperature transition from: ' + raw_time_data[0] + ' to: ' + raw_time_data[-1])
    plt.xlabel('Time')
    plt.ylabel('Temperature')
    name = OUTPUT_FILE_PREFIX + raw_time_data[-1].replace(':', '-').replace(' ', '-') + OUTPUT_FILE_TYPE
    plt.savefig(name)


# main
if __name__ == '__main__':
    read()
    plot_graph()
