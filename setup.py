import os
import re
import sys
import platform
import subprocess
from setuptools import setup, Extension, find_packages
from setuptools.command.build_ext import build_ext
from distutils.version import LooseVersion

# Custom CMakeExtension to tell setuptools that no source files are needed here.
class CMakeExtension(Extension):
    def __init__(self, name, sourcedir=''):
        # 'sources' is empty because CMake will handle the build.
        Extension.__init__(self, name, sources=[])
        self.sourcedir = os.path.abspath(sourcedir)

# Custom build_ext command that runs CMake.
class CMakeBuild(build_ext):
    def run(self):
        # Check that CMake is installed
        try:
            out = subprocess.check_output(['cmake', '--version'])
        except OSError:
            raise RuntimeError("CMake must be installed to build the following extensions: " +
                               ", ".join(e.name for e in self.extensions))
        # Check CMake version
        cmake_version = LooseVersion(re.search(r'version\s*([\d.]+)', out.decode()).group(1))
        if platform.system() == "Windows" and cmake_version < "3.1.0":
            raise RuntimeError("CMake >= 3.1.0 is required on Windows")
        for ext in self.extensions:
            self.build_extension(ext)

    def build_extension(self, ext):
        # Full path where the built module should end up.
        extdir = os.path.abspath(os.path.dirname(self.get_ext_fullpath(ext.name)))
        # Ensure CMake output directory is absolute.
        cmake_args = [
            f"-DCMAKE_LIBRARY_OUTPUT_DIRECTORY={extdir}",
            f"-DPYTHON_EXECUTABLE={sys.executable}",
            "-DFLOAT_TETWILD_ENABLE_PYBINDINGS=ON"
        ]
        build_args = []
        if platform.system() == "Windows":
            cmake_args += [f"-DCMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE={extdir}"]
            if sys.maxsize > 2**32:
                cmake_args += ["-A", "x64"]
            build_args += ["--config", "Release"]
        else:
            cmake_args += ["-DCMAKE_BUILD_TYPE=Release"]
        env = os.environ.copy()
        build_temp = self.build_temp
        if not os.path.exists(build_temp):
            os.makedirs(build_temp)
        # Run CMake configuration
        subprocess.check_call(["cmake", ext.sourcedir] + cmake_args, cwd=build_temp, env=env)
        # Build the extension
        subprocess.check_call(["cmake", "--build", "."] + build_args, cwd=build_temp)

# Setup configuration
setup(
    name="fTetWildPy",
    version="0.1",
    author="Your Name",
    author_email="you@example.com",
    description="Python bindings for fTetWild: Fast Tetrahedral Meshing in the Wild",
    long_description="",
    ext_modules=[CMakeExtension("fTetWildPy", sourcedir=".")],
    cmdclass={"build_ext": CMakeBuild},
    zip_safe=False,
    packages=find_packages(),
)
