import os

# constants requied to create a new module

DIRECTORY = "directory"
FILE = "file"
MODULE_CREATION_SHELL_DIR = "shell" + os.path.sep + "createmodules"
MODULE_STRUCTURE_FILE_NAME = "module_structure.json"

MODULE_STRUCTURE_KEYS = {
	"DIR_CONTENT_TYPE": "type",
	"DIR_CONTENT_NAME": "name",
	"DIR_CONTENTS": "contents"
}

FILE_PERMISSION = "w+"