#!/bin/bash -e

make prj_name=spam_filter512 freq=200M overlay
make prj_name=spam_filter512 freq=200M -j12
