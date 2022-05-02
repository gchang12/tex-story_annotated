from os.path import sep

otherfile=sep.join(['format','other.txt'])
with open(otherfile) as rfile:
    epname=rfile.readline().strip()+'.tex'

class AnnotationChecker:
    """ Checks if the number of annotation markings in the prose match the number of lines in the input files for the annotation sheets."""
    def __init__(self):
        self.ordering=sep.join(['format','ordering.txt'])
        self.epilogue_name=epname
        self.note_dir='annotations'
        self.scene_dir='scenes'
        self.chapter_list=list()

    def compileChapterList(self):
        chapter=list()
        with open(self.ordering) as rfile:
            for N,line in enumerate(rfile.readlines()):
                if not N:
                    continue
                line=line.strip()
                if not line:
                    self.chapter_list.append(chapter)
                    chapter=list()
                    continue
                chapter.append(line)
        self.chapter_list.append(chapter)
        self.chapter_list.append([self.epilogue_name])

    def countNotes(self,index):
        fname=sep.join([self.note_dir,'chapter%d.txt'%index])
        with open(fname) as rfile:
            count=len(rfile.readlines())
        return count

    def countByScene(self,file):
        count=0
        with open(file) as rfile:
            for line in rfile.readlines():
                count+=line.count('markAnnot')
        return count

    def countMarks(self,index):
        count=0
        for file in self.chapter_list[index]:
            file=sep.join([self.scene_dir,file])
            _count=self.countByScene(file)
            count+=_count
        return count

    def noteMarkMatches(self,index):
        x=self.countNotes(index+1),self.countMarks(index)
        return x[0],x[1]

    def makeCountDict(self,index):
        d=dict()
        for f in self.chapter_list[index]:
            path=sep.join([self.scene_dir,f])
            d[f]=self.countByScene(path)
        return d

    def getChapterName(self,index):
        f=sep.join(['format','titles.txt'])
        l=list()
        with open(f) as rfile:
            lines=rfile.readlines()
            for N,line in enumerate(lines):
                if N == index:
                    line=line.strip()
                    return line
        if index == len(lines):
            return 'Epilogue'

    def compareAll(self):
        d=dict()
        for n,chapter in enumerate(self.chapter_list):
            note_c,mark_c=self.noteMarkMatches(n)
            if note_c != mark_c:
                d[self.getChapterName(n)]=(note_c,mark_c)
        if d:
            for c,(note,mark) in d.items():
                print("Chapter: %s"%c)
                print("Notes: %d\nMarks: %d\n"%(note,mark))
            raise Exception

if __name__ == '__main__':
    x=AnnotationChecker()
    x.compileChapterList()
    x.compareAll()
