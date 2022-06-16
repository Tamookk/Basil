import sys
import os
from pathlib import Path

import Utils

class PremakeConfiguration:
    premakeVersion = "5.0.0-beta1"
    premakeZipUrls = f"https://github.com/premake/premake-core/releases/download/v{premakeVersion}/premake-{premakeVersion}-windows.zip"
    premakeLicenseUrl = "https://raw.githubusercontent.com/premake/premake-core/master/LICENSE.txt"
    premakeDirectory = "./vendor/premake/bin"

    @classmethod
    def Validate(self):
        if (not self.CheckIfPremakeInstalled()):
            print("Premake is not installed.")
            return False

        print(f"Correct Premake located at {os.path.abspath(self.premakeDirectory)}")
        return True

    @classmethod
    def CheckIfPremakeInstalled(self):
        premakeExe = Path(f"{self.premakeDirectory}/premake5.exe");
        if (not premakeExe.exists()):
            return self.InstallPremake()

        return True

    @classmethod
    def InstallPremake(self):
        permissionGranted = False
        while not permissionGranted:
            reply = str(input("Premake not found. Would you like to download Premake {0:s}? [Y/N]: ".format(self.premakeVersion))).lower().strip()[:1]
            if reply == 'n':
                return False
            permissionGranted = (reply == 'y')

        premakePath = f"{self.premakeDirectory}/premake-{self.premakeVersion}-windows.zip"
        print("Downloading {0:s} to {1:s}".format(self.premakeZipUrls, premakePath))
        Utils.DownloadFile(self.premakeZipUrls, premakePath)
        print("Extracting", premakePath)
        Utils.UnzipFile(premakePath, deleteZipFile=True)
        print(f"Premake {self.premakeVersion} has been downloaded to '{self.premakeDirectory}'")

        premakeLicensePath = f"{self.premakeDirectory}/LICENSE.txt"
        print("Downloading {0:s} to {1:s}".format(self.premakeLicenseUrl, premakeLicensePath))
        Utils.DownloadFile(self.premakeLicenseUrl, premakeLicensePath)
        print(f"Premake License file has been downloaded to '{self.premakeDirectory}'")

        return True