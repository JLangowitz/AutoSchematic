import zipfile as zf
import xml.etree.ElementTree as et

def main():
    archive = zf.ZipFile('./Fritzing/testSketch.fzz')
    sketch = archive.open('testSketch.fz')
    tree = et.parse(sketch)
    root = tree.getroot()
    for instance in root.findall(".//instance[@moduleIdRef='WireModuleID']/views/breadboardView"):
        geometry = instance.find('geometry')
        color = instance.find('wireExtras').get('color')
        print (geometry.get('x2'),geometry.get('y2'),color)


def getFromFritzing(filepath):
    pass
# fritzingFz=unzipFritzingFile(filepath)//use zipfile module
#     xmlData=et.parse(fritzingFz) //use ElementTree module
#     wireGeometries=extractWireData(xmlData)// Formatted as [[X,Y,Color]...]
#     wires=doTheMath(wireGeometries)//[[length,color]...]
#     return wires

def unzipFritzingFlie(filepath):
    pass
    # //extracts xml file from the .fzz file archive so we can parse it

def extractWireData(data):
    pass
    # //parses the xml to get the wire data and returns it in an array

def doTheMath(wireGeometries):
    pass
    # //takes the list of wire data and calculates lengths
    # //returns a list of tuples of wire lengths and colors to send to the arduino



if __name__ == '__main__':
    main()