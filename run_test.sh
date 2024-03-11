#! /bin/bash


xmake build test_stress

./bin/test_stress | tail -n 2