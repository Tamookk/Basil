import os
import sys
import subprocess
from pathlib import Path

import Utils

from io import BytesIO
from urllib.request import urlopen

class VulkanConfiguration:
    requiredVulkanVersion = "1.2.176.1"
    vulkanDirectory = "./Basil/vendor/VulkanSDK"

    @classmethod
    def Validate(self):
        if (not self.CheckVulkanSDK()):
            print("Vulkan SDK not installed correctly.")
            return

        if (not self.CheckVulkanSDKDebugLibs()):
            print("Vulkan SDK debug libs not found.")

    @classmethod
    def CheckVulkanSDK(self):
        vulkanSDK = os.environ.get("VULKAN_SDK")
        if (vulkanSDK is None):
            print("\nYou don't have the Vulkan SDK installed!")
            self.__InstallVulkanSDK()
            return False
        else:
            print(f"\nLocated Vulkan SDK at {vulkanSDK}")

        if (self.requiredVulkanVersion not in vulkanSDK):
            print(f"You don't have the correct Vulkan SDK version! (Engine requires {self.requiredVulkanVersion})")
            self.__InstallVulkanSDK()
            return False

        print(f"Correct Vulkan SDK located at {vulkanSDK}")
        return True

    @classmethod
    def __InstallVulkanSDK(self):
        permissionGranted = False
        while not permissionGranted:
            reply = str(input("Would you like to install VulkanSDK {0:s}? [Y/N]: ".format(self.requiredVulkanVersion))).lower().strip()[:1]
            if reply == 'n':
                return
            permissionGranted = (reply == 'y')

        vulkanInstallURL = f"https://sdk.lunarg.com/sdk/download/{self.requiredVulkanVersion}/windows/VulkanSDK-{self.requiredVulkanVersion}-Installer.exe"
        vulkanPath = f"{self.vulkanDirectory}/VulkanSDK-{self.requiredVulkanVersion}-Installer.exe"
        print("Downloading {0:s} to {1:s}".format(vulkanInstallURL, vulkanPath))
        Utils.DownloadFile(vulkanInstallURL, vulkanPath)
        print("Running Vulkan SDK installer...")
        os.startfile(os.path.abspath(vulkanPath))
        print("Re-run this script after installation!")
        quit()

    @classmethod
    def CheckVulkanSDKDebugLibs(self):
        shadercdLib = Path(f"{self.vulkanDirectory}/Lib/shaderc_sharedd.lib")

        VulkanSDKDebugLibsURLlist = [
            f"https://sdk.lunarg.com/sdk/download/{self.requiredVulkanVersion}/windows/VulkanSDK-{self.requiredVulkanVersion}-DebugLibs.zip",
            f"https://files.lunarg.com/SDK-{self.requiredVulkanVersion}/VulkanSDK-{self.requiredVulkanVersion}-DebugLibs.zip"

        ]

        if not shadercdLib.exists():
            print(f"\nNo Vulkan SDK debug libs found. (Checked {shadercdLib})")
            vulkanPath = f"{self.vulkanDirectory}/VulkanSDK-{self.requiredVulkanVersion}-DebugLibs.zip"
            Utils.DownloadFile(VulkanSDKDebugLibsURLlist, vulkanPath)
            print("Extracting", vulkanPath)
            Utils.UnzipFile(vulkanPath, deleteZipFile=False)
            print(f"Vulkan SDK debug libs installed at {os.path.abspath(self.vulkanDirectory)}")
        else:
            print(f"\nVulkan SDK debug libs located at {os.path.abspath(self.vulkanDirectory)}")
        return True

if __name__ == "__main__":
    VulkanConfiguration.Validate()