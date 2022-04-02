from distutils.core import setup, Extension
import pkgconfig
import numpy as np

d = pkgconfig.parse('libavformat libswscale opencv4')

ntp_extractor = Extension('ntp_extractor',
                    include_dirs = ['/home/ffmpeg_sources/ffmpeg',
                                    *d['include_dirs'],
                                    np.get_include()],
                    library_dirs = d['library_dirs'],
                    libraries = d['libraries'],
                    sources = ['src/py_video_cap.cpp',
                               'src/video_cap.cpp',
                               'src/time_cvt.cpp'],
                    extra_compile_args = ['-std=c++11'],
                    extra_link_args = ['-fPIC', '-Wl,-Bsymbolic'])

setup (name = 'NTP EXTRACTOR',
       version = '1.0',
       description = 'Used to extract frames and timestamp of each frame from H.264 and MPEG-4 encoded.',
       ext_modules = [ntp_extractor])
