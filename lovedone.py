import serial
import requests
import json
# s = serial.Serial('/dev/ttyACM1', 9600)
if True:
    # line = s.readline().split(",")
    line = "$GPRMC,181924.000,A,4217.6395,N,07115.8750,W,1.85,128.96,161114,,,A*70".split(",")
    if line[0] == "$GPRMC":
        if line[2] == "V":
            print "No fix yet"
        else:
            latdeg = int(line[3][0:2])
            latmin = float(line[3][2:])
            londeg = int(line[5][0:3])
            lonmin = float(line[5][3:])
            print latmin
            lathem = "+" if line[4] == "N" else "-"
            lonhem = "+" if line[4] == "E" else "-"
            lat = lathem + str(latdeg+latmin/60)
            lon = lonhem + str(londeg+lonmin/60)

            print lat+","+lon
            payload = {'latitude':lat,'longitude':lon}
            r = requests.post("http://mojito:3000/lovedone",params=payload)