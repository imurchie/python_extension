from distutils.core import setup, Extension

module1 = Extension('spam',
                    sources = ['src/spammodule.c'])

setup (name = 'Spam Spam Spam',
       version = '0.1',
       description = 'This is a demo package',
       ext_modules = [module1])
