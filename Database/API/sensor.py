from pydantic import BaseModel


class sensor (BaseModel):

    id : int
    controllerId: int
    pin: int
    motorId: int