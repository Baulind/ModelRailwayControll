#Initiate flask aplication: 

from flask import Flask
from flask_sqlalchemy import SQLAlchemy
from os import path
db = SQLAlchemy()
DB_NAME = "database.db"

#Creating the application: 
def create_app(): 
    app = Flask(__name__)
    app.config['SECRET_KEY'] = "123456789"
    app.config['SQLALCHEMY_DATABASE_URI'] = f'sqlite:///{DB_NAME}'
    db.init_app(app)

    from .views import views
    from .auth import auth
    # url_prefix: how do I access all of the urls stored in blueprints 
    app.register_blueprint(views, url_prefix="/") 
    app.register_blueprint(auth, url_prefix="/a")

    from .models import Track, Train, Switch 

    create_database(app)

    return app

def create_database(app): 
    #Check if the database already exciest: 
    if not path.exists("website/" + DB_NAME): 
        db.create_all(app=app)
        print("Created Database!")
