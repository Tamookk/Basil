import os
import subprocess
import platform

from SetupPython import PythonConfiguration as PythonRequirements

# Make sure everything we need is installed
PythonRequirements.Validate()

from SetupVulkan import VulkanConfiguration as VulkanRequirements

# Change to root directory
os.chdir('./../')

VulkanRequirements.Validate()

if platform.system() == "Windows":
	print("\nRunning premake...")
	subprocess.call([os.path.abspath("./scripts/Win-GenProjects.bat"), "nopause"])

print("\nSetup completed!")