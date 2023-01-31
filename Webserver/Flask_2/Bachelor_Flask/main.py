from website import create_app

app = create_app()

#Run website, only if this file is running, not any other file
if __name__ == '__main__': 
    app.run(debug=True) #If we change something in code, it will automatically 
                        #rerun server and make changes on website

