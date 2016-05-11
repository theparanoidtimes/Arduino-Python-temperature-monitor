from distutils.core import setup
import py2exe

setup(
    console=['main.py'],
    options={'py2exe': {"includes": ["matplotlib.backends.backend_tkagg", "tkinter", "tkinter.filedialog"]}})
