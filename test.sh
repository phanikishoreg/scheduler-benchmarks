#!/bin/bash

n=0; while [[ $n -lt 10000 ]]; do $1; n=$((n+1)); done
