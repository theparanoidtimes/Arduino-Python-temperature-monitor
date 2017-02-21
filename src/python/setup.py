"""
py2exe setup script.

@author: Dejan Josifovic
"""

from distutils.core import setup
import py2exe
import matplotlib
import numpy
import os
import sys

# add any numpy directory containing a dll file to sys.path
def numpy_dll_paths_fix():
    paths = set()
    np_path = numpy.__path__[0]
    for dirpath, _, filenames in os.walk(np_path):
        for item in filenames:
            if item.endswith('.dll'):
                paths.add(dirpath)

    sys.path.append(*list(paths))

numpy_dll_paths_fix()

# setup configuration
setup(
    console=['main.py'],
	data_files=matplotlib.get_py2exe_datafiles(),
    options={'py2exe': {"includes": ["matplotlib.backends.backend_tkagg", "Tkinter", "FileDialog"],
						"dll_excludes": ["MSVCP90.dll"]}})
