"""
PoE F2013
Team AutoSchematic
Samantha Kumarasena, Josh Langowitz, Daniel Leong, David Zhu
Website
"""
# imports
from flask import Flask, request, session, g, redirect, url_for, \
     abort, render_template, flash
import os
from werkzeug import secure_filename
from getWireData import *
from talkToArduino import *
import serial

# config
DEBUG = True
SECRET_KEY = 'i6TIU_oI7yoiHp_p9Ppu_oGTGiu6TIRFdy5ufr__sda-3'
USERNAME = 'admin'
PASSWORD = 'default'
UPLOAD_FOLDER = './uploads/'
ALLOWED_EXTENSIONS = set(['fzz'])

def allowed_file(filename):
    return '.' in filename and \
           filename.rsplit('.', 1)[1] in ALLOWED_EXTENSIONS


# app setup
app = Flask(__name__)
app.config.from_object(__name__)
app.config['UPLOAD_FOLDER'] = UPLOAD_FOLDER
app.jinja_env.add_extension('pyjade.ext.jinja.PyJadeExtension')

@app.route('/cut/<filename>', methods=['GET','POST'])
def cut(filename):
    # serial setup
    print "hello"
    ser=serial.Serial('/dev/ttyACM0') #connect to serial port
    waitForCompletion(ser)
    wireData=getWireData(UPLOAD_FOLDER, filename)
    sendWires(ser, wireData)
    # print wireData
    return render_template('wires.jade', title='Wires', wireData=wireData)
@app.route('/')
def homepage():
    return render_template('index.jade', title='AutoSchematic')

@app.route('/team')
def teampage():
    return render_template('team.jade', title='Team')

@app.route('/details')
def details():
    return render_template('details.jade', title='Details')

@app.route('/upload', methods=['GET','POST'])
def uploads():
    if request.method == 'POST':
        file = request.files['file']
        if file and allowed_file(file.filename):
            filename = secure_filename(file.filename)
            file.save(os.path.join(app.config['UPLOAD_FOLDER'], filename))
    return render_template('upload.jade', title='Upload')

@app.route('/wires')
def wires():
    return

if __name__ == '__main__':
    app.run()