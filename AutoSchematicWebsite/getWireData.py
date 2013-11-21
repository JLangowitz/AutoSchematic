import zipfile as zf
import xml.etree.ElementTree as et
import math


# def main():
#     print getWireData('./uploads/','Electrical_Diagram')

def getWireData(filepath, filename):
    """
    Gets the wire data from the specified file and parses it 
    into a format to send to the arduino. See helper functions for details.

    filepath: folder the file is in
    filename: name of file, without extension

    returns: list of (color, length) tuples of the wires.
    """
    wires = getFromFritzing(filepath, filename)
    colors={'#418dd9':'b',  #blue
        '#cc1414':'r',      #red
        '#404040':'k',      #black
        '#ffe24d':'y',      #yellow
        '#47cc79':'g',      #green
        '#999999':'a',      #gray
        '#ffffff':'w',      #white
        '#ff7033':'o',      #orange
        '#8c3b00':'n',      #brown
        '#ab58a2':'p',      #purple
        '#000000':'k'}      #schematic black
    return doTheMath(wires,colors)


def getFromFritzing(filepath, filename):
    """
    Unzips the archive at filepath and parses the filename as xml,
    extracting the wire geometries and color, returning them as a list of tuples


    Fritzing files are .fzz files, which are a zipped directory containing
    a .fz file, which is just a lot of xml. In the xml tree is a list of instances
    of frizing parts, which contain 3 views, breadboard, schematic, and pcb.
    The instances also have an attribute called moduleIdRef, which is 'WireModuleID'
    for wires, which are what we care about. Within the breadboard view,
    each wire has a 'geometry' child, with attributes 'x2' and 'y2' which contain the
    x and y dimensions of a wire. Each wire also has a 'wireExtras' child, which has an
    attribute called 'color', which gives us the color. These are the only things we need,
    and this function extracts them from every wire and returns a list of tuples of (x,y,color)


    filepath: root location of the Fritzing .fzz file

    filename: name of the fritzing file
    """
    res=[]
    archive = zf.ZipFile(filepath+filename+'.fzz')
    sketch = archive.open(filename.replace('_',' ')+'.fz')
    tree = et.parse(sketch)
    root = tree.getroot()
    for wire in root.findall(".//instance[@moduleIdRef='WireModuleID']/views/breadboardView"):
        geometry = wire.find('geometry')
        color = wire.find('wireExtras').get('color')
        res.append((geometry.get('x2'),geometry.get('y2'),color))
    return res

def doTheMath(wireGeometries,colors):
    """
    Calculates the lengths to send to the arduino
    and converts colors from hex to single chars
    Returns list of tuples of (color, length)

    wireGeometries: list of tuples of wire data from the fritzing file
    """
    res = []
    for x,y,color in wireGeometries:
        length=math.sqrt(float(x)**2+float(y)**2)
        # map to number of steps on our stepper
        length*=.449
        length=int(round(length))
        if length>255:
            print "error, wire too long"
        else:
            res.append((colors[color],length))
    res.sort()
    return res



# if __name__ == '__main__':
#     main()