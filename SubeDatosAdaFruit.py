"""
Camilo Perafan
Francis Sanabria
Hans Burmester
'publish.py'
=========================================
Publishes an incrementing value to a feed
Author(s): Brent Rubell, Todd Treece for Adafruit Industries
"""
# Import standard python modules
import time
import random

# Import Adafruit IO REST client.
from Adafruit_IO import Client, Feed

# holds the count for the feed
temp = 0
puerta = 0
luces = 0
sensorLuz = 0
TiempoReal = 0

# Set to your Adafruit IO key.
# Remember, your key is a secret,
# so make sure not to publish it when you publish this code!
#ADAFRUIT_IO_KEY = 'YOUR_AIO_KEY'

# Set to your Adafruit IO username.
# (go to https://accounts.adafruit.com to find your username)
#ADAFRUIT_IO_USERNAME = 'YOUR_AIO_USERNAME'

ADAFRUIT_IO_USERNAME = "FrancisSanabria"
ADAFRUIT_IO_KEY = "aio_xVBq37Fhu4njrKfGaZfENNdTmgD4"

# Create an instance of the REST client.
aio = Client(ADAFRUIT_IO_USERNAME, ADAFRUIT_IO_KEY)


while True:

    #Creando valores random
    temp = random.randint(5, 80)
    puerta = random.randint(0,1)
    luces = random.randint(0,1)
    sensorLuz = random.randint(1,100)
    TiempoReal = time.ctime(time.time())
    
    #imprime lo que se va a enviar a io
    print('sending temp: ', temp)
    aio.send_data('lab6-feeds.temperatura', temp)
    time.sleep(2)
    
    print('sending puerta: ', puerta)
    aio.send_data('lab6-feeds.llave-sensorhall', puerta)
    time.sleep(2)
    
    print('sending luces: ', luces)
    aio.send_data('lab6-feeds.luz-sensorsonido', luces)
    time.sleep(2)
    
    print('sending sensorLuz: ', sensorLuz)
    aio.send_data('lab6-feeds.sensorluz', sensorLuz)
    time.sleep(2)
    
    print('sending TiempoReal: ', TiempoReal)
    aio.send_data('lab6-feeds.tiemporeal', TiempoReal)
    time.sleep(2)
 
    
    # Adafruit IO is rate-limited for publishing
    # so we'll need a delay for calls to aio.send_data()
    
