import csv
import os

def find_DT_numbers(filename):
    with open(os.path.join("data/", filename), newline='\r\n') as file:
        DT_list = []
        reader = csv.reader(file, delimiter=',')
        for row in reader:
            if row[1] in DT_list:
                return DT_list
            else:
                DT_list.append(row[1])