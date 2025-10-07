import json

def get_DT_list(DT_numbers):
    list = []
    with open('UDS_database.json') as json_data:
        database = json.load(json_data)
    for i in range(0, len(database)):
        if int(database[i]['PID'], 16) in DT_numbers:
            for j in range(0, len(database[i]['Data'])):
                list.append([i, j, (database[i]['Data'][j]['Name'])])
    return list

def get_drawing_parameters(i, j):
    with open('UDS_database.json') as json_data:
        database = json.load(json_data)
    data_offset = database[i]['Data'][j]['DataOffset']
    data_length = database[i]['Data'][j]['DataLength']
    value_factor = database[i]['Data'][j]['ValueFactor']
    value_offset = database[i]['Data'][j]['ValueOffset']
    return [data_offset, data_length, value_factor, value_offset]

def get_DT_value(i):
    with open('UDS_database.json') as json_data:
        database = json.load(json_data)
    return int(database[i]['PID'], 16)

def get_unit(i, j):
    with open('UDS_database.json') as json_data:
        database = json.load(json_data)
    return database[i]['Data'][j]['Unit']