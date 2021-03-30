.. Install instructions file

MyGL Instalation
================

Build the library
-----------------

You can build this libary from its sources using CMake, you will need to clone
the repository_ and build with CMake

.. code-block::

    $ git clone https://github.com/tretre91/MyGL.git

And then use CMake to build the project, the main options are :

`MYGL_STATIC`
    Builds the static version of the library (on by default)
`MYGL_SHARED`
    Builds the shared version of the library (on by default)
`MYGL_BUILD_DOCUMENTATION`
    Builds the documentation
`MYGL_BUILD_SAMPLE`
    Builds the file used for testing
`CMAKE_INSTALL_PREFIX`
    Sets the path to the folder where the files will be installed

.. _repository: https://github.com/tretre91/MyGL

Building the documentation
--------------------------

In order to build the documentation you will need to have doxygen_ installed. You will also need sphinx_, 
the read_the_doc theme and breathe (a program which will generate sphinx documentation from the doxygen output),
these can be installed from the command line using pip :

.. code-block::

    $ pip install -U sphinx
    $ pip install sphinx_rtd_theme
    $ pip install breathe

And then set the option MYGL_BUILD_DOCUMENTATION in CMake

.. _doxygen: https://www.doxygen.nl/download.html
.. _sphinx: https://www.sphinx-doc.org/en/master/usage/installation.html