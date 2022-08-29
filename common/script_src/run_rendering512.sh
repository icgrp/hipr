#!/bin/bash -e

make prj_name=rendering512 freq=200M overlay
make prj_name=rendering512 freq=200M -j12
