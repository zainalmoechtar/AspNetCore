$ServerState = Get-WindowsCapability -Online | Where-Object { $_.Name -like "OpenSSH.Server*" } | Select-Object -First 1 -ExpandProperty State
if ($ServerState -eq "NotPresent") {
    Add-WindowsCapability -Online -Name OpenSSH.Server~~~~0.0.1.0
}

if (Get-NetFirewallRule *ssh*) {
    Write-Host "SSH Server installed Successfully"
}
else {
    throw "Error installing SSH Server"
}

# Install PowerShell Core if not already installed
if (Get-Command pwsh -ErrorAction SilentlyContinue) {
    $PwshPath = (Get-Command pwsh).Definition
}
else {
    $dest = Join-Path ([System.IO.Path]::GetTempPath()) "pwsh.zip"
    if (Test-Path $dest) {
        Write-Host "Deleting old powershell installer"
        Remove-Item -Force $dest
    }

    $source = "https://github.com/PowerShell/PowerShell/releases/download/v6.1.3/PowerShell-6.1.3-win-x64.zip"
    Invoke-WebRequest $source -OutFile $dest

    # Install PowerShell Core
    mkdir "C:\pwsh"
    Expand-Archive -Path $source -DestinationPath "C:\pwsh"
    $PwshPath = "C:\pwsh\pwsh.exe"
}

$SubsystemPath = $PwshPath.Replace("\", "/");

if ($SubsystemPath.Contains(" ")) {
    throw "Spaces are not allowed in powershell core path!"
}

# Configure SSHD_CONFIG
$SshdConf = Join-Path (Join-Path $env:ProgramData "ssh") "sshd_config"
$SshdConfContent = [System.IO.File]::ReadAllText($SshdConf)

$nl = [Environment]::NewLine
$SshdConfContent = "PasswordAuthentication yes$($nl)$SshdConfContent"
$SshdConfContent = "Subsystem  powershell $SubsystemPath -sshs -NoLogo -NoProfile$($nl)$SshdConfContent"
[System.IO.File]::WriteAllText($SshdConf, $SshdConfContent)

# Start SSHD
Start-Service sshd
Set-Service -Name sshd -StartupType 'Automatic'
