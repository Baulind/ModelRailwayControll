# Install FastAPI framework
# pip3 install "fastapi[all]"
# https://fastapi.tiangolo.com/tutorial/

# uvicorn main:app --reload

import uvicorn

from fastapi import FastAPI, Response, status, Body
from fastapi.staticfiles import StaticFiles
from typing import Union, Annotated
import sqlite3
from motorValue import motorValue
from motor import motor
from sensorValue import sensorValue
from sensor import sensor
from switchValue import switchValue
from switch import switch

app = FastAPI()


# http://localhost:8000/
@app.get("/")
def root():
    return {"message": "Welcome to SmartHouse Cloud REST API - Powered by FastAPI"}


@app.get("/motor")
def all_motors():
    con = sqlite3.connect('Database\db.sqlite')
    cur = con.cursor()
    result: list[int] = list()
    cmd = f"SELECT DISTINCT(mv.motorId) FROM motorValue mv;"
    cur.execute(cmd)
    for i in cur.fetchall():
        result.append(i[0])
    return result

@app.get("/switch")
def all_switches():
    con = sqlite3.connect('Database\db.sqlite')
    cur = con.cursor()
    result: list[int] = list()
    cmd = f"SELECT DISTINCT(sv.switchId) FROM switchValue sv;"
    cur.execute(cmd)
    for i in cur.fetchall():
        result.append(i[0])
    return result

@app.get("/sensor")
def all_sensors():
    con = sqlite3.connect('Database\db.sqlite')
    cur = con.cursor()
    result: list[int] = list()
    cmd = f"SELECT DISTINCT(sv.sensorId) FROM sensorValue sv;"
    cur.execute(cmd)
    for i in cur.fetchall():
        result.append(i[0])
    return result

@app.get("/motor/{id}")
def motor_value_by_id(id: int):
    con = sqlite3.connect('Database\db.sqlite')
    cur = con.cursor()
    result: list[motorValue] = list()
    cmd = f"SELECT * from motorValue mv WHERE mv.motorId == {id};"
    cur.execute(cmd)
    for i in cur.fetchall():
        result.append(motorValue(id=i[0], motorId=i[1], target=i[2], time=i[3], sender=i[4]))
    return result

@app.get("/switch/{id}")
def switch_value_by_id(id: int):
    con = sqlite3.connect('Database\db.sqlite')
    cur = con.cursor()
    result: list[switchValue] = list()
    cmd = f"SELECT * FROM switchValue sv WHERE sv.switchId == {id};"
    cur.execute(cmd)
    for i in cur.fetchall():
        result.append(switchValue(id=i[0], switchId=i[1], target=i[2], time=i[3], sender=i[4]))
    return result

@app.get("/sensor/{id}")
def sensor_value_by_id(id: int):
    con = sqlite3.connect('Database\db.sqlite')
    cur = con.cursor()
    result: list[sensorValue] = list()
    cmd = f"SELECT * FROM sensorValue sv WHERE sv.sensorId  == {id};"
    cur.execute(cmd)
    for i in cur.fetchall():
        result.append(sensorValue(id=i[0], sensorId=i[1], value=i[2], time=i[3], sender=i[4]))
    return result

@app.get("/motor/{id}/now")
def current_motor_value_by_id(id: int):
    con = sqlite3.connect('Database\db.sqlite')
    cur = con.cursor()
    cmd = f"SELECT * from motorValue mv WHERE mv.motorId == {id} ORDER by mv.id DESC;"
    cur.execute(cmd)
    i = cur.fetchone()
    return motorValue(id=i[0], motorId=i[1], target=i[2], time=i[3], sender=i[4])

@app.get("/switch/{id}/now")
def current_switch_value_by_id(id: int):
    con = sqlite3.connect('Database\db.sqlite')
    cur = con.cursor()
    cmd = f"SELECT * FROM switchValue sv WHERE sv.switchId == {id} ORDER by sv.id DESC;"
    cur.execute(cmd)
    i = cur.fetchone()
    return switchValue(id=i[0], switchId=i[1], target=i[2], time=i[3], sender=i[4])

@app.get("/sensor/{id}/now")
def current_sensor_value_by_id(id: int):
    con = sqlite3.connect('Database\db.sqlite')
    cur = con.cursor()
    cmd = f"SELECT * FROM sensorValue sv WHERE sv.sensorId  == {id} ORDER by sv.id DESC;"
    cur.execute(cmd)
    i = cur.fetchone()
    return sensorValue(id=i[0], sensorId=i[1], value=i[2], time=i[3], sender=i[4])

if __name__ == '__main__':
    uvicorn.run(app, host="127.0.0.1", port=8000)