from sys import platform
from subprocess import check_call

def upload(source, host):
    check_call(['curl', '-F', 'image=@./.pio/build/esp12e/firmware.bin',
                host])