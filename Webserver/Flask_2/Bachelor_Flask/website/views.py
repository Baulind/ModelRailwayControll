#Store standard roots for the website: 
#Home page, and so on so on 

from flask import Blueprint, render_template

# Define that it is a blueprint of our application: 
# Blueprint: File where we just define ULR roots 

views = Blueprint('views', __name__)

#Define the route for the homepage: 
@views.route('/')
def home(): 
    return render_template("home.html")


@views.route('/controller')
def controller(): 
    return render_template("controller.html")