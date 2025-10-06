import json

def get_DT_list(DT_numbers):
    list = []
    with open('UDS_database.json') as json_data:
        database = json.load(json_data)
    for i in range(0, len(database)):
        if int(database[i]['PID'], 16) in DT_numbers:
            for j in range(0, len(database[i]['Data'])):
                list.append(database[i]['Data'][j]['Name'])
    return list
