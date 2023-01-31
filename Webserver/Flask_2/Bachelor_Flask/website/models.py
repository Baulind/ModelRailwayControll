from . import db 

class Track(db.Model): 
    id = db.Column(db.Integer, db.ForeignKey('Train.id'), primary_key = True)
    name = db.Column(db.String(100))
    train_current = db.Column(db.Integer, db.ForeignKey('Train.id'))

class Train(db.Model): 
    id = db.Column(db.Integer, db.ForeignKey('Track.id'), primary_key = True)
    name = db.Column(db.String(100))
    train_current = db.Column(db.Integer, db.ForeignKey('Track.id'))
    speed = db.Column(db.Float(100))

class Switch(db.Model): 
    traffic_id = db.Column(db.Integer, primary_key = True)
    switch_id = db.Column(db.Integer, primary_key = True)
    traffic_name = db.Column(db.String(100))
    switch_name = db.Column(db.String(100))

