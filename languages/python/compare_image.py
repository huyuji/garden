import sys
import cv2

def isMatIdentical(mat1, mat2):
    if (mat1.shape != mat2.shape):
      print "different shape"
      return False
      
    diff = mat1 - mat2
    nz = cv2.countNonZero(diff);
    print nz
    return nz==0;
    
if __name__ == '__main__':
  mat1 = cv2.imread(sys.argv[1],0)
  mat2 = cv2.imread(sys.argv[2],0)
  if(isMatIdentical(mat1, mat2)):
    print "images are identical"
  else:
    print "images are not identical"