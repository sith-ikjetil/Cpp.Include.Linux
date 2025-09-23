#!/bin/bash
cppcheck  --enable=all \
          --std=c++17 \
          --suppress=missingIncludeSystem \
          .
