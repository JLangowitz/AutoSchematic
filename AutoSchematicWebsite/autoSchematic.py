"""
PoE F2013
Team AutoSchematic
Samantha Kumarasena, Josh Langowitz, Daniel Leong, David Zhu
Website
"""
# imports
from flask import Flask, request, session, g, redirect, url_for, \
     abort, render_template, flash, redirect
import os
from werkzeug import secure_filename
from getWireData import *
from talkToArduino import *
import serial
import random

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

@app.route('/cut/<filename>', methods=['POST'])
def cut(filename):
    # serial setup
    if request.method == 'POST':
        try:
            ser=serial.Serial('/dev/ttyACM0') #connect to serial port
            waitForCompletion(ser)
            wireData=getWireData(UPLOAD_FOLDER, filename)
            sendWires(ser, wireData)
            title='Cut in Progress'
        except:
            print 'Cannot connect to arduino'
            err = 'There has been an error communicating with our device.\
            Please try again later.'
            title='Sorry!'
        return render_template('cut.jade', filename=filename, title=title, \
                url= 'http://placekitten.com/%s/%s' %(str(random.randint(200,600)),str(random.randint(200,600))), \
                err=err)
        
    # print wireData
@app.route('/')
def homepage():
    return render_template('index.jade', title='AutoSchematic')

@app.route('/upload', methods=['GET','POST'])
def uploads():
    # print request.json['files']
    if request.method == 'POST':
        print 'upload'
        print request
        print request.__dict__
        print request.form
        file = request.files['file']
        if file and allowed_file(file.filename):
            filename = secure_filename(file.filename)
            file.save(os.path.join(app.config['UPLOAD_FOLDER'], filename))
        else:
            return render_template('upload.jade',title='Upload', url=url_for('static', filename='AutoSchematicLogo'), err='Please upload a .fzz file')
        return redirect('/cut/'+filename.replace(' ','_').replace('.fzz',''), code=307)
    return render_template('upload.jade', title='Upload', url=url_for('static', filename='AutoSchematicLogo'))

if __name__ == '__main__':
    app.run()