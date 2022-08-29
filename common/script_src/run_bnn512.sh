#!/bin/bash -e
make prj_name=bnn512 freq=150M overlay
make prj_name=bnn512 freq=150M -j12

