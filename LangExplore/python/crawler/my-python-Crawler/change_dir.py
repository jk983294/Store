#!/usr/bin/python -tt
import sys
import os

class Change_Dir:
	def __init__(self,newPath):
		self.savedPath = os.getcwd()
		if not os.path.exists(newPath):
			os.mkdir(newPath)
		os.chdir(newPath)
	def __del__(self):
		os.chdir(self.savedPath)