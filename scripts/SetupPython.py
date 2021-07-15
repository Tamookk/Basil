import sys
import subprocess
import importlib.util as importlib_util

class PythonConfiguration:
    @classmethod
    def Validate(self):
        if not self.__ValidatePython():
            return # cannot validate further

        for packageName in ["requests"]:
            if not self.__ValidatePackage(packageName):
                return # cannot validate further

    @classmethod
    def __ValidatePython(self, versionMajor = 3, versionMinor = 3):
        if sys.version is not None:
            print("Python version {0:d}.{1:d}.{2:d} detected.".format( \
                sys.version_info.major, sys.version_info.minor, sys.version_info.micro))
            if sys.version_info.major < versionMajor or (sys.version_info.major == versionMajor and sys.version_info.minor < versionMinor):
                print("Python version too low, expected version {0:d}.{1:d} or higher.".format( \
                    versionMajor, versionMinor))
                return False
            return True

    @classmethod
    def __ValidatePackage(self, packageName):
        if importlib_util.find_spec(packageName) is None:
            return self.__InstallPackage(packageName)
        return True

    @classmethod
    def __InstallPackage(self, packageName):
        permissionGranted = False
        while not permissionGranted:
            reply = str(input("Would you like to install Python package '{0:s}'? [Y/N]: ".format(packageName))).lower().strip()[:1]
            if reply == 'n':
                return False
            permissionGranted = (reply == 'y')

        print(f"Installing {packageName} module...")
        subprocess.check_call(['python', '-m', 'pip', 'install', packageName])

        return self.__ValidatePackage(packageName)

if __name__ == "__main__":
    PythonConfiguration.Validate()