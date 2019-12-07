from sys import platform
from subprocess import check_call


def uploadByEnv(source, target, env):
    if platform == "win32":
        check_call(['powershell.exe', './uploadScript.ps1'])
    else:
        check_call(['curl', '-F', 'image=@./.pio/build/esp12e/firmware.bin',
                    'esp8266-webupdate.local/update'])


def upload(source, host):
    check_call(['curl', '-F', 'image=@./.pio/build/esp12e/firmware.bin',
                host])
    # if platform == "win32":
    # check_call(['powershell.exe', './uploadScript.ps1', host])
    # else:
