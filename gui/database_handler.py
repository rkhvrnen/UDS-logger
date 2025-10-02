import json

def get_DT_list():
    list = []
    with open('UDS_database.json') as json_data:
        database = json.load(json_data)
    for i in range(0, len(database)):
        for j in range(0, len(database[i]['Data'])):
            list.append(database[i]['Data'][j]['Name'])
    return list
