#!/usr/bin/env python3

# Needed For Main
import os
import sys
import logging
import paho.mqtt.client as mqtt
import mysql.connector as mariadb
import json

# Needed For Wait Loop
import time
import socket

# Configure Default Variables
MQTT_HOST = os.getenv('MQTT_HOST')
MQTT_PORT = os.getenv('MQTT_PORT')
MQTT_TOPIC = os.getenv('MQTT_TOPIC')
MQTT_KEEP_ALIVE = os.getenv('MQTT_KEEP_ALIVE')
MYSQL_HOST = os.getenv('MYSQL_HOST')
MYSQL_PORT = os.getenv('MYSQL_PORT')
MYSQL_USER = os.getenv('MYSQL_USER')
MYSQL_PASSWORD = os.getenv('MYSQL_PASSWORD')
MYSQL_DB = os.getenv('MYSQL_DATABASE')

# Configure Logging
logging.basicConfig(level=os.getenv('LOGLEVEL','DEBUG'))

# Wait For Port Connection To Occur
def wait_for_port(port, host='127.0.0.1', timeout=5.0):
  start_time = time.perf_counter()
  while True:
    try:
      with socket.create_connection((host, port), timeout=timeout):
        break
    except OSError as ex:
      time.sleep(0.01)
      if time.perf_counter() - start_time >= timeout:
        raise TimeoutError('Waited too long for the port {} on host {} to start accepting connections.'.format(port, host)) from ex

# Subscribe To Channel
def on_connect(client, userdata, flags, rc):
  # Subscribe To Topics
  topics = MQTT_TOPIC.split(',')
  for topic in topics:
    mqttc.subscribe(topic, 0)
    logging.info('Subscribed To MQTT Topic ' + topic)
  mqttc.subscribe("$SYS/broker/log/#")

# Process Incoming Messages
def on_message(mosq, obj, msg):
  # Search or Write Topic
  cursor.execute("SELECT topic_id, topic FROM topics WHERE topic=\"" + msg.topic + "\";") #All queries like this must be replaced for security, don't want injection that easily - lol
  result=cursor.fetchall()
  if len(result) == 0:
    cursor.execute("INSERT INTO topics(topic) VALUES (\"" + msg.topic + "\");")
    topic_id=cursor.lastrowid
  else:
    topic_id=result[0][0]

  # Write Out Topic Value
  sql = "INSERT INTO data(value_timestamp,value,topic_id) VALUES (now(3), %s, %s);"
  cursor.execute(sql, (msg.payload.decode(), str(topic_id)))

  # Commit Data
  mariadb_connection.commit()
  logging.info('Writing MQTT Topic \"' + msg.topic + '\" And MQTT Value \"' + msg.payload.decode() + '\" To Database')

# No Actions On Subscribe
def on_subscribe(mosq, obj, mid, granted_qos):
  pass

# Initialise MQTT
mqttc = mqtt.Client()

# Assign event callbacks
mqttc.on_message = on_message
mqttc.on_connect = on_connect
mqttc.on_subscribe = on_subscribe

# Connect To The Database
try:
  wait_for_port(host=MYSQL_HOST, port=MYSQL_PORT, timeout=30.0)
except TimeoutError as e:
  logging.error('Took To Long To Connect To ' + MYSQL_HOST + ':' + MYSQL_PORT)
  sys.exit()

mariadb_connection = mariadb.connect(host=MYSQL_HOST, port=MYSQL_PORT, user=MYSQL_USER, password=MYSQL_PASSWORD, database=MYSQL_DB, auth_plugin='mysql_native_password')
logging.info('Succssfully Connect To MySql Server On ' + MYSQL_HOST + ':' + MYSQL_PORT)
cursor = mariadb_connection.cursor()

# Connect To MQTT
try:
  wait_for_port(host=MQTT_HOST, port=MQTT_PORT, timeout=30.0)
except TimeoutError as e:
  logging.error('ERROR: Took To Long To Connect To ' + MQTT_HOST + ':' + MQTT_PORT)
  sys.exit()

mqttc.connect(MQTT_HOST, int(MQTT_PORT), int(MQTT_KEEP_ALIVE))
logging.info('Succssfully Connect To MQTT Broker On ' + MQTT_HOST + ':' + MQTT_PORT)

# Loop Forever Or Close DB On Shutdown
mqttc.loop_forever()

# Close Database On Exit
mqttc.disconnect()
mariadb_connection.close()
logging.info('Disconnecting From MySql and MQTT - Shutting Down')
