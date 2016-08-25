#!/bin/bash

n=0; while [[ $n -lt 1000000 ]]; do $1; n=$((n+1)); done
