. .\scripts\appveyor\appveyor_get_build.ps1
. .\scripts\appveyor\appveyor_get_artifacts.ps1

# Ignore commits without APPVEYOR_API_TOKEN envrionment variable
if (-not ($env:APPVEYOR_API_TOKEN)) { return }

# Only deploy when commit message contains "[deploy]"
if (!(Get-AppVeyorBuild).build.message.Contains('[deploy]')) { return }

# Deploy once on last job
if ($env:APPVEYOR_JOB_NUMBER -ne $env:job_to_deploy) { return }

# Wait for other jobs complete
Write-Host "Waiting for other jobs to complete"

[datetime]$stop = ([datetime]::Now).AddMinutes($env:time_out_mins)
[bool]$success = $false

while(!$success -and ([datetime]::Now) -lt $stop) {
    $success = $true
    (Get-AppVeyorBuild).build.jobs | foreach-object {
        if (($_.jobId -ne $env:APPVEYOR_JOB_ID) -and ($_.status -ne "success")) {
            $success = $false
        }
        Write-Host "Job `"$($_.jobId)`" $($_.status)";
    }
    if (!$success) { Start-sleep 5 }
}

if (!$success) {
    throw "Test jobs were not finished in $env:time_out_mins minutes"
}

# Set flag to deploy
$env:flag_to_deploy = $success

# get job artifacts
Write-Host "Start to download artifacts from other jobs"

[string[]] $artifacts = @()

(Get-AppVeyorBuild).build.jobs | foreach-object {
    $jobId = $_.jobId
    if ($jobId -ne $env:APPVEYOR_JOB_ID) {
        # Get job artifacts information
        (Get-AppVeyorArtifacts -Job $jobId) | foreach-object {
            # Create directory if not exists
            $filePath = $_.fileName.Substring(0, $_.fileName.LastIndexOf('/'))
            if (!(Test-Path -Path $filePath)) {
                New-Item -ItemType "directory" -Path $filePath
            }
            # Download artifact from other job
            Start-FileDownload "https://ci.appveyor.com/api/buildjobs/$jobId/artifacts/$($_.fileName)" -FileName $_.fileName
            $artifacts += $_.fileName
        }
    };
}

Write-Host "Start to package artifacts"

# Copy published files
$publishedFiles = ('.\Easy2D\include', '.\Easy2D\output', '.\scripts\7z\install.bat')
New-Item -ItemType "directory" -Path "published"
Copy-Item -Path $publishedFiles -Destination '.\published' -Recurse

# Packaging
7z.exe a -t7z -mmt -mx9 install.7z .\published*
cmd /c copy /b scripts\7z\7zS2.sfx + scripts\7z\7z-config.txt + install.7z installer.exe

# Upload artifacts
$artifactVersion = "easy2d-v$($env:APPVEYOR_BUILD_VERSION)"
$artifactName = $artifactVersion + '.7z'
Rename-Item 'install.7z' -NewName $artifactName
Push-AppveyorArtifact $artifactName -FileName $artifactName

$artifactName = $artifactVersion + '-installer.exe'
Rename-Item 'installer.exe' -NewName $artifactName
Push-AppveyorArtifact $artifactName -FileName $artifactName

# Delete redundant artifacts
foreach ($artifact in $artifacts) {
    Remove-Item -Path $artifact
}
