#!/bin/bash -e

make prj_name=optical_flow512 freq=200M overlay
make prj_name=optical_flow512 freq=200M -j12
