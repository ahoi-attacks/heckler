import os

from setuptools import setup
from setuptools.extension import Extension

script_dir = os.path.dirname(os.path.realpath(__file__)) + '/'
include_dir = script_dir + '../sev-step/sev-step-lib'
kernel_dir = script_dir + '../../linux/host/usr/include'
lib_dir = script_dir + '../sev-step/build/libs/'

extra_objects = [lib_dir + '/libsevstep.so']
libs = [lib_dir]

ext = Extension('heckler_bindings',
                sources=['heckler_bindings/heckler_module_def.c'],
                libraries=['sevstep', 'pthread', 'm'],
                library_dirs=libs,
                include_dirs=[include_dir, kernel_dir],
                extra_objects=extra_objects, extra_link_args=[])

setup(name='heckler_bindings',
      version='1.0',
      author='SECTRS',
      author_email='',
      url='',
      ext_modules=[ext]
)
