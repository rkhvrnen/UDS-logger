import serial
import sys
import datetime
import time
import os

port=sys.argv[1]
baud=115200

date_str = str(datetime.datetime.now()).split(' ')
filename_suffix = date_str[0] + '_' + date_str[1].split('.')[0]
ser = serial.Serial(port, baud, timeout = 2)
filename = "datalog_" + filename_suffix + ".csv"
filename = filename.replace(":", "")
filename = filename.replace("-", "")

f=open(filename,"w")
f.write("Time(s),PID,Data(Hex)\n")
start_time = time.time()

while True:
    try:
        read_data=ser.readline().decode().strip('\r\n')
        timestamp = time.time() - start_time
        #print(read_data.format(hex))
        print("TIME: {0}, PID: {1}, DATA: {2}".format(timestamp,read_data.split(',')[0],read_data.split(',')[1]))
        f.write("{0},{1}\n".format(timestamp,read_data))
    except Exception as e:
        pass
