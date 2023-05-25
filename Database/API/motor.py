from pydantic import BaseModel


class motor (BaseModel):

    id : int
    controllerId: int
    enablePin: int
    directionPin: int