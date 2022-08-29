#!/bin/bash -e


make prj_name=digit_reg512 freq=200M overlay
make prj_name=digit_reg512 freq=200M -j12
