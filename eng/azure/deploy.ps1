<#
.PARAMETER Template
    The JSON file specifying the template. Use either a fully-qualified URL or the name of a template in eng/azure/templates.
.PARAMETER ResourceGroupName
    The resource group to create or deploy in to. If this group does not exist, it will be created.
.PARAMETER SubscriptionName
    The Azure subscription to deploy to. Defaults to the 'ASP.NET Core Test' subscription. If you do not have access to this subscription, an error will occur.
.PARAMETER Location
    The Azure region to deploy resources to. Defaults to "westus2".
#>
param(
    [Parameter(Mandatory = $false)][string]$Template,
    [Parameter(Mandatory = $true)][string]$ResourceGroupName,
    [Parameter(Mandatory = $true)][string]$MachineName,
    [Parameter(Mandatory = $false)][string]$SubscriptionName = "ASP.NET Core Test",
    [Parameter(Mandatory = $false)][string]$Location = "westus2"
)

if (!(Get-Module Az)) {
    # 'Az' is a collection of modules with names that all start 'Az.'
    if (Get-Module -ListAvailable | Where-Object { $_.Name.StartsWith("Az.") }) {
        Write-Host "Importing 'Az' module..."
        Import-Module Az
    }
    else {
        throw "The 'Az' module must be installed to use this script. Run 'Install-Module Az -Scope CurrentUser' to install it."
    }
}

if ($Template -notmatch "^https://.*$") {
    # Check for modified files in 'eng/azure'
    if (git status --porcelain | Where-Object { $_ -match "eng/azure" }) {
        Write-Warning "There are changes to the 'eng/azure' directory."
        Write-Warning "Azure templates must be located on a public URL so that the ARM deployment system can access them."
        Write-Warning "If you have made changes to the templates, you have to check them in and push to GitHub in order to use the updated script!"
    }

    if (!$Template) {
        $Template = "win2019.json"
    }

    if (!$Template.EndsWith(".json")) {
        $Template = "$Template.json"
    }

    Write-Host "Resolving template name: $Template"

    # Determine the active branch
    $Branch = git rev-parse --abbrev-ref HEAD
    $Commit = git rev-parse HEAD
    Write-Host "Active branch: $Branch."
    Write-Host "Active commit: $Commit."

    $Template = "https://raw.githubusercontent.com/aspnet/AspNetCore/$Commit/eng/azure/templates/$Template"
}

Write-Host "Using template URL: $Template"

# Check if we're logged in
if (!(Get-AzContext)) {
    Write-Host "Not logged in to azure yet. Logging in..."
    Connect-AzAccount | Out-Null
}

# Get an AzContext
$AzContext = Get-AzContext -ListAvailable | Where-Object { $_.Subscription.Name -eq $SubscriptionName }

# Check for the resource group
$ResourceGroup = Get-AzResourceGroup -DefaultProfile $AzContext -Name $ResourceGroupName -ErrorAction SilentlyContinue
if (!$ResourceGroup) {
    Write-Host "Could not find resource group '$ResourceGroupName'. Creating it..."
    $ResourceGroup = New-AzResourceGroup -DefaultProfile $AzContext -Name $ResourceGroupName -Location $Location
}

Write-Host "Using resource group '$($ResourceGroup.ResourceGroupName)'."

# Deploy the template
Write-Host "Deploying template. This may take a while. You can see status on the Azure Portal at:"
Write-Host "https://ms.portal.azure.com/#resource$($ResourceGroup.ResourceId)/deployments"
$Deployment = New-AzResourceGroupDeployment -DefaultProfile $AzContext -TemplateUri $Template -ResourceGroupName $ResourceGroup.ResourceGroupName -TemplateParameterObject @{"vmName" = $MachineName}

