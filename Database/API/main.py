# Install FastAPI framework
# pip3 install "fastapi[all]"
# https://fastapi.tiangolo.com/tutorial/

# uvicorn main:app --reload

import uvicorn

from fastapi import FastAPI, Response, status, Body
from fastapi.staticfiles import StaticFiles
from typing import Union, Annotated
import sqlite3

app = FastAPI()

# http://localhost:8000/
@app.get("/")
def root():
    return {"message": "Welcome to SmartHouse Cloud REST API - Powered by FastAPI"}

if __name__ == '__main__':
    uvicorn.run(app, host="127.0.0.1", port=8000)
