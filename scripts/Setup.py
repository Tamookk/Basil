import os
import subprocess
import CheckPython

# Make sure everything we need is installed
CheckPython.validate_packages()

import Vulkan

# Change to root directory
os.chdir('../')

if not Vulkan.check_Vulkan_SDK():
	print("Vulkan SDK not installed.")

if not Vulkan.check_Vulkan_SDK_debug_libs():
	print("Vulkan SDK debug libs not found.")

print("Running premake...")
subprocess.call(["vendor/premake/premake5.exe", "vs2019"])