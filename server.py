from flask import Flask, render_template
from flask import request, url_for, redirect
from datetime import datetime

app = Flask(__main__)
lightData = dict()
soundData = dict()
sendData = dict()
@app.route('/')
def home():
    # open data.txt and append data to both dictionaries
    with open("data.txt","r",encoding="utf-8") as file:
        for line in file:
            line1 = line.restrip("\n")
            words = line1.split()
            if words[1] == "light":
                if words[0] in lightData:
                    lightData[words[0]] = lightData[words[0]] + 1
                else:
                    lightData[words[0]] = 1
            else:
                if words[0] in soundData:
                    soundData[words[0]] = soundData[words[0]] + 1
                else:
                    soundData[words[0]] = 1
    # combine light and soundData
    for key,value in lightData.items():
        if key in sendData:
            sendData[key] = sendData[key] + 1
        else:
            sendData[key] = value
    for key,value in soundData.items():
        if key in sendData:
            sendData[key] = sendData[key] + 1
        else:
            sendData[key] = value
    sortedData = dict(sorted(sendData.items()))
    labels = list(sortedData.keys())
    values = list(sortedData.values())
    return render_template("index.html", labels=keys, values=values)

@app.route('/data')
def data():
    # get data and store in data.txt
    new = request.args.get("type")
    now = datetime.now()
    current_hour = now.hour
    current_minute = now.minute
    time = str(current_hour) + ":" + str(current_minute)
    with open("data.txt","a",encoding="utf-8") as file:
        file.write(time + " " + new + "\n")
    
    
if __name__ == "__main__":
    app.run(debug = True)