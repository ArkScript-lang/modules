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
	contentName = content[cs.MODULE_STRUCTURE_KEYS["DIR_CONTENT_NAME"]]
	
	if contentType == cs.DIRECTORY:
		rootDirectory = createDirectory(rootDirectory, contentName)
	elif contentType == cs.FILE:
		createFile(rootDirectory, contentName)

	if cs.MODULE_STRUCTURE_KEYS["DIR_CONTENTS"] not in content:
		return

	for subContent in content[cs.MODULE_STRUCTURE_KEYS["DIR_CONTENTS"]]:
		buildStructure(rootDirectory, subContent)

def createDirectory(rootDirectory, directoryName):
	'''
	Creates a given directory under the given root
	
	Parameters:
	rootDirectory (string): The parent directory under which the sub directory should be created
	directoryName (string): the name of the directory

	Returns:
	string: path of the current root directory
	'''
	path = os.path.join(rootDirectory, directoryName)
	os.mkdir(path)

	return path

def createFile(rootDirectory, fileName):
	'''
	Creates a file under the given root
	
	Parameters:
	rootDirectory (string): The parent directory under which the file should be created
	directoryName (string): the name of the directory
	'''
	path = os.path.join(rootDirectory, fileName)
	file = open(path, cs.FILE_PERMISSION)

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