from pydantic import BaseModel


class switchValue (BaseModel):

    id : int
    switchId: int
    target: int
    time: str
    sender: str