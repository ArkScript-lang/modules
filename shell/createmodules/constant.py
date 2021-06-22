import os

# constants requied to create a new module

DIRECTORY = "directory"
FILE = "file"
MODULE_CREATION_SHELL_DIR = "shell" + os.path.sep + "createmodules"
MODULE_STRUCTURE_FILE_NAME = "module_structure.json"

MODULE_STRUCTURE_KEYS = {
	"DIR_CONTENT_TYPE": "type",
	"DIR_CONTENT_NAME": "name",
	"DIR_CONTENTS": "contents",
	"DIR_CONTENT_IS_TEMPLATE": "is-template"
}

FILE_CREATE_PERMISSION = "w+"
FILE_READ_PERMISSION = "r"

TEMPLATE_FILE_NAME_PREFIX = "Template_"
MODULE_NAME_PLACEHOLDER = "<module_name>"