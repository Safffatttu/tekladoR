$url = "esp8266-webupdate.local/update"
$filePath = './.pio/build/esp12e/firmware.bin'
$Form = @{
    name = Get-Item -Path $filePath
}

Invoke-RestMethod -Uri $url -Method Post -Form $Form