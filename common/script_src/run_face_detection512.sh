#!/bin/bash -e

make prj_name=face_detection512 freq=200M overlay
make prj_name=face_detection512 freq=200M -j12
