import csv
import os

def find_DT_numbers(filename):
    with open(os.path.join("data/", filename), newline='\r\n') as file:
        DT_list = []
        reader = csv.reader(file, delimiter=',')
        for row in reader:
            if int(row[1], 16) in DT_list:
                return DT_list
            else:
                DT_list.append(int(row[1], 16))

def get_data(file, DT, data_offset, data_length, value_factor, value_offset):
    time = []
    data = []
    with open(os.path.join("data/", file), newline='\r\n') as file:
        reader = csv.reader(file, delimiter=',')
        for row in reader:
            if(int(row[1], 16) == DT):
                time.append(float(row[0]))
                data.append(value_factor * int(row[2][2*data_offset:(2*data_offset+(2*data_length))], 16) + value_offset)
            else:
                continue
    return [time, data]