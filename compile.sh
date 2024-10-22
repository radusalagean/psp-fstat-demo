#!/bin/bash

set -e

# Compile
mkdir -p build
cd build
psp-cmake -DCMAKE_BUILD_TYPE=Debug ..
make

# Create ISO file
mkdir -p ISO_ROOT/PSP_GAME/SYSDIR
mkdir -p ISO_ROOT/PSP_GAME/USRDIR
cp pspfstatdemo.prx ISO_ROOT/PSP_GAME/SYSDIR/EBOOT.BIN
cp ../crosshair.png ISO_ROOT/PSP_GAME/USRDIR/crosshair.png
cp PARAM.SFO ISO_ROOT/PSP_GAME/PARAM.SFO
ISO_VOLUME_NAME="pspfstatdemo"
ISO_FILE_NAME="${ISO_VOLUME_NAME}_mkisofs.iso"
PLATFORM_IDENTIFIER="PSP GAME"
mkisofs -iso-level 4 -xa -A "$PLATFORM_IDENTIFIER" -V "$ISO_VOLUME_NAME" \
    -sysid "$PLATFORM_IDENTIFIER" -volset "" -p "" -publisher "" \
    -o "$ISO_FILE_NAME" ISO_ROOT
