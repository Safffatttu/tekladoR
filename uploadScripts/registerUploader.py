from uploadScript import upload
import code

Import("env")

# Python callback


def on_upload(source, target, env):
    src_build_flags = env['SRC_BUILD_FLAGS']
    host_name = src_build_flags[0].split('"')[1]
    host_name = '{}.local/update'.format(host_name)
    firmware_path = str(source[0])
    upload(firmware_path, host_name)


env.Replace(UPLOADCMD=on_upload)
