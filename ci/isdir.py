#!/usr/bin/env python3
import os, sys
os.chdir(os.environ['MESON_SOURCE_ROOT'])
sys.exit(0 if os.path.isdir(sys.argv[1]) else 1)
