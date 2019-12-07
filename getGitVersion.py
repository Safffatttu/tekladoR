Import("projenv")

import subprocess

versionHash = subprocess.check_output(["git", "rev-parse", "HEAD"])

projenv.Append(CPPDEFINES=[
    ("GIT_HASH", '\\"' + str(versionHash[:-1]) + '\\"' )
])
