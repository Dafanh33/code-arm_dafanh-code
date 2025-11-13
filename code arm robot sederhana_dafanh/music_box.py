import serial
import time
from pycaw.pycaw import AudioUtilities

ser = serial.Serial('COM5', 115200, timeout=1)  # Ganti COM sesuai port ESP Master

def get_volume():
    sessions = AudioUtilities.GetAllSessions()
    total = 0
    for s in sessions:
        try:
            vol = s._ctl.QueryInterface(AudioUtilities.ISimpleAudioVolume)
            total += vol.GetMasterVolume()
        except:
            pass
    return total / len(sessions) if len(sessions) > 0 else 0

while True:
    vol = get_volume()
    if vol > 0.02:
        ser.write(b'1')
        print(" Musik hidup → kirim 1")
    else:
        ser.write(b'0')
        print(" Musik mati → kirim 0")
    time.sleep(0.5)
