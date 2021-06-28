# ATA
integrating ata with homekit doorbell via homepods

### thankyou ATA for so kindly sending the receiver, without it, none of this would have been possible.


# requirements
- homebridge server (http://homebridge.io)
- homebridge-http-regex (https://github.com/WouterJanson/homebridge-http-regex)
- homebridge-ffmpeg (https://github.com/homebridge/ffmpeg-for-homebridge)
- esp8266 or another webserver, locally hosted.
- RF transmitter (ATA KPX-7V2, https://www.ata-aust.com.au/accessories/keypads/kpx-7-wireless-digital-keypad) and reciver (ATA FHCRX-1, https://www.ata-aust.com.au/accessories/receivers/single-dual-channel-receivers)
- homekit automations
- power sources for everything above ^

# homebridge server
i wont go into how i set my homebridge server up.
i used the windows 10 version as a service. 
how to do this is here https://github.com/homebridge/homebridge/wiki/Install-Homebridge-on-Windows-10

# homebridge-http-regex
this plugin will need to be installed. 
```
{
            "accessory": "Regex",
            "name": "doorbell pressed",
            "endpoint": "http://192.168.0.236",
            "pattern": ".*1.",
            "interval": "500"
}
```
acessory: must be Regex
name: can be anything. same as what will appear in home app.
endpoint: IP address of local webserver.
pattern: regular expression (RegEx) pattern.
interval: time between pulls in ms.

the endpoint can show any data it wants. mine shows:
```
{
            "currentState": 0
}
```
when the doorbell is <b>not</b> pressed

and

```
{
            "currentState": 1
}
```
when the doorbell is pressed

it can be anything. adjust the RegEx depending on data. the regex opens a contact sensor when the pattern is matched. so, for me, the pattern is matched when there is a '1' between other data.

# homebridge-ffmpeg
the only way i have found to sound a doorbell is when a camera plugin is installed. this requires an IP camera. i used a reolink 410. my config file looks like:

```
{
    "platform": "Camera-ffmpeg",
    "cameras": [
        {
            "name": "Driveway",
            "switches": true,
            "motion": true,
            "unbridge": true,
            "doorbell": true,
            "videoConfig": {
                "source": "-i rtsp://user:password@192.168.0.90:554/h264Preview_01_main",
                "stillImageSource": "-i http://192.168.0.90/cgi-bin.cgi?cmd=Snap&channel=0&rs=1&user=user&password=password",
                "vcodec": "libx264",
                "maxStreams": 2,
                "maxWidth": 960,
                "maxHeight": 720,
                "maxFPS": 25,
                "maxBitrate": 2000,
                "audio": true,
                "packetSize": 188,
                "hflip": false,
                "debug": false
            }
        }
    ]
}
```
the username, password and IP address will need to be changed.
the important section is the \/. 
``` 
    "switches": true,
    "motion": true,
```

# esp8266

i custom coded a program in 'C' (i think) with Arduino IDE that adjusts data on a webserver depending on whether or not a pin in grounded. it shows the below when the pin is grounded
```
{"currentState": 1}
```

this program can be found here:
https://github.com/ixy055/ata/blob/main/esp8266_webserver.ino
i changed someone elses, im unsure of where i got it as i did it a long time ago; was initially a gate sensor i made but still works as this.
i dont plan on optimising this as i dont have the time.

just upload this program to the esp8266 using the arduino IDE. make sure to adjust the network credentials.

below is a schematic i used, works with software provided. 

![image](https://user-images.githubusercontent.com/55933691/123565352-ec898580-d7ff-11eb-8ca5-a39feaabdea1.png)

# transmitter and receiver. 

storing codes is really simple. it took me only 1 try.

the documentation was confusing so i had to re-read it several times. i hope the instructions below are more clear.

to store codes this follow these steps.
   1. hold the programing button (SW1) on the reciver <b>and keep it help down</b>
   2. hold the '0' key on the keypad (KPX-7V2) for 2 seconds
   3. let go for 2 seconds
   4. hold the '0' key on the keypad (KPX-7V2) for 2 seconds
   5. the LED on the reciver should go solid for a second and then turn off
   6. the code is saved. test by holding the '0' key on the keypad for over 0.5 seconds.

# homekit automations

requires a home hub (at least one of these)
- ipad
- homepod
- apple tv

after adding the regex contact sensor to homekit follow these steps <b>in the iOS home app.</b>

   1. press the plus
   2. press add automation
   3. press "a sensor detects something"
   4. press "doorbell pressed" (or whatever you named the regex plugin)
   5. press next
   6. press "opens"
   7. press next (unless you only want the doorbell to sound when you're home or during the day)
   8. press "<driveway> (or whatever you named the camera) doorbell trigger"
   9. press next
   10. turn on the doorbell trigger under the accessories heading
   11. press done
  
  *****
  
You are now done!

see the images here:
            https://github.com/ixy055/ata/blob/main/images.md
