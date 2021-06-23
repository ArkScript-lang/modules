import os
import json
import argparse
import constant as cs

def buildStructure(rootDirectory, content):
	'''
	Builds the defined directory structure under the given root
	
	Parameters:
	rootDirectory (string): The parent directory under which the content should be created
	content (dict): the definition of the directories/files that should be created under the root
	'''

	contentType = content[cs.MODULE_STRUCTURE_KEYS["DIR_CONTENT_TYPE"]]
	
	if contentType == cs.DIRECTORY:
		rootDirectory = createDirectory(rootDirectory, content[cs.MODULE_STRUCTURE_KEYS["DIR_CONTENT_NAME"]])
	elif contentType == cs.FILE:
		createFile(rootDirectory, content)

	if cs.MODULE_STRUCTURE_KEYS["DIR_CONTENTS"] not in content:
		return

	for subContent in content[cs.MODULE_STRUCTURE_KEYS["DIR_CONTENTS"]]:
		buildStructure(rootDirectory, subContent)

def createDirectory(rootDirectory, directoryName):
	'''
	Creates a given directory under the given root
	
	Parameters:
	rootDirectory (string): The parent directory under which the sub directory should be created
	content (dict): the definition of the directories/files that should be created under the root

	Returns:
	string: path of the current root directory
	'''
	path = os.path.join(rootDirectory, directoryName)
	os.mkdir(path)

	return path

def isTemplateFile(content):
	'''
	Checks if a given file should be copied from a template file
	
	Parameters:
	content (dict): the definition of the directories/files that should be created under the root

	Returns:
	boolean: true if template file is available else false
	'''
	isTemplateKey = cs.MODULE_STRUCTURE_KEYS["DIR_CONTENT_IS_TEMPLATE"]

	return (isTemplateKey in content and content[isTemplateKey])

def readTemplateFile(fileName):
	'''
	Reads the contents of the given template file
	
	Parameters:
	fileName (string): file name to be created

	Returns:
	string: contents of the corresponding template file
	'''
	templateFileName = cs.TEMPLATE_FILE_NAME_PREFIX + fileName
	path = os.path.join(cs.MODULE_CREATION_SHELL_DIR, templateFileName)

	return open(path, cs.FILE_READ_PERMISSION).read()

def writeContentsToFile(file, content, rootDirectory):
	'''
	Copies content from template file and writes to given file
	
	Parameters:
	file (File object): the new file to which the contents of the template are copied
	content (dict): the definition of the directories/files that should be created under the root
	rootDirectory (string): name of the directory under which the file is created

	Returns:
	boolean: true if template file is available else false
	'''
	moduleName = rootDirectory.split(os.path.sep)[-1]

	templateFileContents = readTemplateFile(content[cs.MODULE_STRUCTURE_KEYS["DIR_CONTENT_NAME"]])
	file.write(templateFileContents.replace(cs.MODULE_NAME_PLACEHOLDER, moduleName))


def createFile(rootDirectory, content):
	'''
	Creates a file under the given root
	
	Parameters:
	rootDirectory (string): The parent directory under which the file should be created
	content (dict): the definition of the directories/files that should be created under the root
	'''
	fileName = content[cs.MODULE_STRUCTURE_KEYS["DIR_CONTENT_NAME"]]
	path = os.path.join(rootDirectory, fileName)
	file = open(path, "w+")

	if isTemplateFile(content):
		writeContentsToFile(file, content, rootDirectory)

def getModuleName():
	'''
	Fetches the argument name passed to the script

	Returns:
	string: name of the module to be created
	'''
	parser = argparse.ArgumentParser()
	parser.add_argument("name")
	directory = parser.parse_args()

	return directory.name

def getDirectoryStructure():
	'''
	Reads the directory structure from file and builds a dictionary

	Returns
	dict: dictionary with the directory structure to be created
	'''
	jsonFile = os.getcwd() + os.path.sep + cs.MODULE_CREATION_SHELL_DIR + os.path.sep + cs.MODULE_STRUCTURE_FILE_NAME
	f = open(jsonFile)
	directoryStructure = json.load(f)
	f.close()
	
	return directoryStructure

if __name__ == "__main__":
	moduleName = getModuleName()
	directoryStructure = getDirectoryStructure()
	
	rootDir = createDirectory(os.getcwd(), moduleName)

	for content in directoryStructure:
		buildStructure(rootDir, content)
