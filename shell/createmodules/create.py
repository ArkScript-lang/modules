import os
import json
import argparse
import constant as cs


def build_structure(root_directory, content):
    """
    Builds the defined directory structure under the given root

    Parameters:
    root_directory (string): The parent directory under which the content should be created
    content (dict): the definition of the directories/files that should be created under the root
    """

    content_type = content[cs.MODULE_STRUCTURE_KEYS["DIR_CONTENT_TYPE"]]

    if content_type == cs.DIRECTORY:
        root_directory = create_directory(root_directory, content[cs.MODULE_STRUCTURE_KEYS["DIR_CONTENT_NAME"]])
    elif content_type == cs.FILE:
        create_file(root_directory, content)

    if cs.MODULE_STRUCTURE_KEYS["DIR_CONTENTS"] not in content:
        return

    for subContent in content[cs.MODULE_STRUCTURE_KEYS["DIR_CONTENTS"]]:
        build_structure(root_directory, subContent)


def create_directory(root_directory, directory_name):
    """
    Creates a given directory under the given root

    Parameters:
    root_directory (string): The parent directory under which the subdirectory should be created
    directory_name (string): name of the directory that should be created under the root

    Returns:
    string: path of the current root directory
    """
    path = os.path.join(root_directory, directory_name)
    os.mkdir(path)
    return path


def is_template_file(content):
    """
    Checks if a given file should be copied from a template file

    Parameters:
    content (dict): the definition of the directories/files that should be created under the root

    Returns:
    boolean: true if template file is available else false
    """
    is_template_key = cs.MODULE_STRUCTURE_KEYS["DIR_CONTENT_IS_TEMPLATE"]
    return is_template_key in content and content[is_template_key]


def read_template_file(file_name):
    """
    Reads the contents of the given template file

    Parameters:
    file_name (string): file name to be created

    Returns:
    string: contents of the corresponding template file
    """
    template_file_name = cs.TEMPLATE_FILE_NAME_PREFIX + file_name
    with open(os.path.join(cs.MODULE_CREATION_SHELL_DIR, template_file_name)) as f:
        return f.read()


def write_contents_to_file(file, content, root_directory):
    """
    Copies content from template file and writes to given file

    Parameters:
    file (File object): the new file to which the contents of the template are copied
    content (dict): the definition of the directories/files that should be created under the root
    root_directory (string): name of the directory under which the file is created

    Returns:
    boolean: true if template file is available else false
    """
    module_name = root_directory.split(os.path.sep)[-1]

    template_file_contents = read_template_file(content[cs.MODULE_STRUCTURE_KEYS["DIR_CONTENT_NAME"]])
    file.write(template_file_contents.replace(cs.MODULE_NAME_PLACEHOLDER, module_name))


def create_file(root_directory, content):
    """
    Creates a file under the given root

    Parameters:
    root_directory (string): The parent directory under which the file should be created
    content (dict): the definition of the directories/files that should be created under the root
    """
    file_name = content[cs.MODULE_STRUCTURE_KEYS["DIR_CONTENT_NAME"]]
    path = os.path.join(root_directory, file_name)
    file = open(path, "w+")

    if is_template_file(content):
        write_contents_to_file(file, content, root_directory)


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("name")
    directory = parser.parse_args()

    with open(os.path.join(os.getcwd(), cs.MODULE_CREATION_SHELL_DIR, cs.MODULE_STRUCTURE_FILE_NAME)) as f:
        directory_structure = json.load(f)
    root_dir = create_directory(os.path.join(os.getcwd(), "src"), directory.name)

    for content in directory_structure:
        build_structure(root_dir, content)

    print(f"""\nINFO: to compile your module, add

add_subdirectory({directory.name})

To modules/src/CMakeLists.txt""")
