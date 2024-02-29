#!/bin/sh

cd ${SRCROOT}
cd macOS

cd ../../../..

BIN_TO_C_TOOL=deps/bin_to_c/dist/MacOS/bin_to_c
DATA_DIR=assets/data/engine
OUT_DIR=include/GowEngine/embeddedData

rm -rf include/GowEngine/embeddedData
mkdir include/GowEngine/embeddedData

# json
$BIN_TO_C_TOOL $DATA_DIR/json/Assets.json engine_assets_json &> $OUT_DIR/engine_assets_json.h

$BIN_TO_C_TOOL $DATA_DIR/json/Config.json engine_config_json &> $OUT_DIR/engine_config_json.h

# shaders

$BIN_TO_C_TOOL $DATA_DIR/shaders/framebuffer.vsh framebuffer_vsh &> $OUT_DIR/framebuffer_vsh.h
$BIN_TO_C_TOOL $DATA_DIR/shaders/framebuffer.fsh framebuffer_fsh &> $OUT_DIR/framebuffer_fsh.h

$BIN_TO_C_TOOL $DATA_DIR/shaders/geometry.vsh geometry_vsh &> $OUT_DIR/geometry_vsh.h
$BIN_TO_C_TOOL $DATA_DIR/shaders/geometry.fsh geometry_fsh &> $OUT_DIR/geometry_fsh.h

$BIN_TO_C_TOOL $DATA_DIR/shaders/lights.vsh lights_vsh &> $OUT_DIR/lights_vsh.h
$BIN_TO_C_TOOL $DATA_DIR/shaders/lights.fsh lights_fsh &> $OUT_DIR/lights_fsh.h

$BIN_TO_C_TOOL $DATA_DIR/shaders/shockwave.vsh shockwave_vsh &> $OUT_DIR/shockwave_vsh.h
$BIN_TO_C_TOOL $DATA_DIR/shaders/shockwave.fsh shockwave_fsh &> $OUT_DIR/shockwave_fsh.h

$BIN_TO_C_TOOL $DATA_DIR/shaders/sprite.vsh sprite_vsh &> $OUT_DIR/sprite_vsh.h
$BIN_TO_C_TOOL $DATA_DIR/shaders/sprite.fsh sprite_fsh &> $OUT_DIR/sprite_fsh.h

# textures

$BIN_TO_C_TOOL $DATA_DIR/textures/sf_font.png sf_font_png &> $OUT_DIR/sf_font_png.h
