from pydantic import BaseModel


class sensorValue (BaseModel):

    id : int
    sensorId: int
    value: int
    time: str