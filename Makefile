mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
mkfile_dir := $(dir $(mkfile_path))
SKETCH = $(mkfile_dir)/aes256.ino
UPLOAD_PORT = /dev/ttyUSB0
BOARD=huzzah
BUILD_DIR=$(mkfile_dir)/build

include $(mkfile_dir)/makeEspArduino/makeEspArduino.mk
