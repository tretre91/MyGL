.. Install instructions file

MyGL Instalation
================

You can build this libary from its sources using CMake,
you will need to clone the `repository`_ and build 
with CMake

.. code-block::

    $ git clone https://github.com/tretre91/MyGL.git

.. _repository: https://github.com/tretre91/MyGL

Building this doc
=================

In order to build the documentation you will need to have doxygen installed (https://www.doxygen.nl/download.html).
You will also need sphinx (https://www.sphinx-doc.org/en/master/usage/installation.html), 
the read_the_doc theme and breathe (a program which will generate sphinx documentation
from the doxygen output), these can be installed from the command line using pip :

.. code-block::

    $ pip install -U sphinx
    $ pip install sphinx_rtd_theme
    $ pip install breathe

And then set the option MYGL_BUILD_DOCUMENTATION in CMake