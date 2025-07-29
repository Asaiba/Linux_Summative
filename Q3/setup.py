from setuptools import setup, Extension

faststats_module = Extension('faststats', sources=['faststats.c'])

setup(
    name='faststats',
    version='1.0',
    description='Fast statistics library implemented in C',
    ext_modules=[faststats_module],
)
