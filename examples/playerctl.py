import serial
import subprocess
import time

ser = serial.Serial('/dev/ttyUSB0', 9600, timeout=1)
ser.flush()

last_artist = ""
last_album = ""
last_title = ""

while True:
    try:
        artist_result = subprocess.run(
            ['playerctl', 'metadata', 'artist'],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
            encoding='utf-8'
        )
        
        album_result = subprocess.run(
            ['playerctl', 'metadata', 'album'],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
            encoding='utf-8'
        )
        
        title_result = subprocess.run(
            ['playerctl', 'metadata', 'title'],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
            encoding='utf-8'
        )
        
        if all(r.returncode == 0 for r in [artist_result, album_result, title_result]):
            current_artist = artist_result.stdout.strip()
            current_album = album_result.stdout.strip()
            current_title = title_result.stdout.strip()
            current_extra = "hello"
            crrent_extra = current_extra.strip()
            
            if (current_artist != last_artist or 
                current_album != last_album or 
                current_title != last_title) and current_title:
                
                data_to_send = f"{current_artist}|{current_album}|{current_title}|{current_extra}\n"
                ser.write(data_to_send.encode('utf-8'))
                last_artist = current_artist
                last_album = current_album
                last_title = current_title
                
    except Exception as e:
        print(f"Error: {e}")
    
    time.sleep(1)
