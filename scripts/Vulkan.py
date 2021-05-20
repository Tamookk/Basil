import os
import subprocess
import sys
from pathlib import Path

import Utils

from io import BytesIO
from urllib.request import Request, urlopen
from zipfile import ZipFile

VULKAN_SDK = os.environ.get('VULKAN_SDK')
VULKAN_SDK_INSTALLER_URL = 'https://sdk.lunarg.com/sdk/download/latest/windows/vulkan-sdk.exe'
BASIL_VULKAN_VERSION = '1.2.176.1'
VULKAN_SDK_EXE_PATH = 'Basil/vendor/VulkanSDK/VulkanSDK.exe'

def install_Vulkan_SDK():
	if not os.path.exists(VULKAN_SDK_EXE_PATH[:-14]):
		os.makedirs(VULKAN_SDK_EXE_PATH[:-14])

	print('Downloading {} to {}'.format(VULKAN_SDK_INSTALLER_URL, VULKAN_SDK_EXE_PATH))
	Utils.download_file(VULKAN_SDK_INSTALLER_URL, VULKAN_SDK_EXE_PATH)
	print("Done!")
	print("Running Vulkan SDK installer...")
	os.startfile(os.path.abspath(VULKAN_SDK_EXE_PATH))
	print("Re-run this scrupt after installation")

def install_Vulkan_prompt():
	print("Would you like to install the Vulkan SDK?")
	install = Utils.yes_or_no()
	if install:
		install_Vulkan_SDK()
		quit()

def check_Vulkan_SDK():
	if VULKAN_SDK is None:
		print("You don't have the Vulkan SDK installed.")
		install_Vulkan_prompt()
		return False
	elif BASIL_VULKAN_VERSION not in VULKAN_SDK:
		print(f"Located Vulkan SDK at {VULKAN_SDK}")
		print(f"You don't have the correct Vulkan SDK version. Basil requires {BASIL_VULKAN_VERSION}")
		install_Vulkan_prompt();
		return False

	print(f"Correct Vulkan SDK located at {VULKAN_SDK}")
	return True

VulkanSDKDebugLibsURL = 'https://sdk.lunarg.com/sdk/download/1.2.176.1/windows/VulkanSDK-1.2.176.1-DebugLibs.zip'
OutputDirectory = "Basil/vendor/VulkanSDK"
TempZipFile = f"{OutputDirectory}/VulkanSDK.zip"

def check_Vulkan_SDK_debug_libs():
	shadercdLib = Path(f"{OutputDirectory}/Lib/shaderc_sharedd.lib")
	if not shadercdLib.exists():
		print(f"No Vulkan SDK debug libs found. Checked {shadercdLib}")
		print("Downloading", VulkanSDKDebugLibsURL)
		req = Request(VulkanSDKDebugLibsURL, headers={'User-Agent': 'Mozilla/5.0'})
		with urlopen(req) as zipresp:
			with ZipFile(BytesIO(zipresp.read())) as zfile:
				zfile.extractall(OutputDirectory)

	print(f"Vulkan SDK debug libs located at {OutputDirectory}")
	return True