import os
import subprocess
import platform

from SetupPython import PythonConfiguration as PythonRequirements

# Make sure everything we need is installed
PythonRequirements.Validate()

from SetupPremake import PremakeConfiguration as PremakeRequirements
from SetupVulkan import VulkanConfiguration as VulkanRequirements

# Change to root directory
os.chdir('./../')

premakeInstalled = PremakeRequirements.Validate()
VulkanRequirements.Validate()

print("Updating submodules...")
subprocess.call(["git", "submodule", "update", "--init", "--recursive"])

if premakeInstalled:
	if platform.system() == "Windows":
		print("\nRunning premake...")
		subprocess.call([os.path.abspath("./scripts/Win-GenProjects.bat"),
						"nopause"])

	print("\nSetup completed!")
else:
	print("Basil requires Premake to generate project files.")