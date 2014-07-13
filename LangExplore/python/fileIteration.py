import os
import json

class MPR:
    def to_JSON(self):
        return json.dumps(self, default=lambda o: o.__dict__, sort_keys=True, indent=4)
		
class MPR1:
	def set_meta(self,meta):
		self.meta = meta
	def set_project(self,project):
		self.project = project
	def set_release(self,release):
		self.release = release

def iterate_using_walk1(rootDir): 
	for subdir, dirs, files in os.walk(rootDir):
		for file in files:
			print subdir+'/'+file
		
def iterate_using_walk(rootDir): 
    list_dirs = os.walk(rootDir) 
    for root, dirs, files in list_dirs: 
        for d in dirs: 
            print os.path.join(root, d)      
        for f in files: 
            print os.path.join(root, f) 
			
def iterate_using_listdir(rootDir): 
    for lists in os.listdir(rootDir): 
        path = os.path.join(rootDir, lists) 
        print path 
        if os.path.isdir(path): 
            iterate_using_listdir(path) 

def tree_dir(rootDir, level=1): 
    if level==1: print rootDir 
    for lists in os.listdir(rootDir): 
        path = os.path.join(rootDir, lists) 
        print '|  '*(level-1)+'|--'+lists 
        if os.path.isdir(path): 
            tree_dir(path, level+1) 

def test_file_iteration():
	rootdir = 'D:/Github/Store/LangExplore'
	iterate_using_listdir(rootdir)
	print '\n'
	iterate_using_walk(rootdir)
	print '\n'
	iterate_using_walk1(rootdir)
	print '\n'
	tree_dir(rootdir)


			
if __name__ == '__main__':
	#test_file_iteration()
	mpr = MPR()
	mpr.meta = 'a'
	mpr.project = 'b'
	mpr.release = 'c'
	mpr.xml = []
	mpr.xml .append('x')
	mpr.xml .append('y')
	mpr.xml .append('z')
	print mpr.to_JSON()