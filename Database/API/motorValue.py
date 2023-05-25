from pydantic import BaseModel


class motorValue (BaseModel):

    id : int
    motorId: int
    target: int
    time: str
    sender: str