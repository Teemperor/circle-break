# Circle-break

[![Build Status](https://travis-ci.org/Teemperor/circle-break.svg?branch=master)](https://travis-ci.org/Teemperor/circle-break)

## Building

circle-break requires boost and C++11. Type these commands to setup:

    mkdir build
    cmake ..
    make circle-break

## Usage

`cd` to the top-level directory of the SCRAM project. Then run `circle-break` to just get a list of
cyclic dependencies in the scram project. Run `circle-break --html` to generate a interactive HTML
report for the current project.

## Testing

After configuring, run:

   make circle-break-tests
   # cd into the tests/files directory of the source
   cd ../tests/files/
   ../../build/circle-break-tests
