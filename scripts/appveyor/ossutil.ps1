function OSS-Init {
    param(
        [string]$Endpoint,
        [string]$AccessKeyId,
        [string]$AccessKeySecret
    )

    Invoke-RestMethod -Uri "https://gosspublic.alicdn.com/ossutil/1.7.14/ossutil32.zip" -OutFile "ossutil32.zip"
    7z.exe x -tzip -y ossutil32.zip
    ./ossutil32/ossutil32.exe config -e $Endpoint -i $AccessKeyId -k $AccessKeySecret -L CH
    return
}

function OSS-UploadFile {
    param(
        [string]$File,
        [string]$Target
    )
    ./ossutil32/ossutil32.exe cp $File $Target
    return
}
