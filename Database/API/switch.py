from pydantic import BaseModel


class switch (BaseModel):

    id : int
    controllerId: int
    pin: int

