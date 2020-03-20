import os
import cv2
import numpy as np
import sys
import math
from matplotlib import pyplot as plt
import matplotlib.image
from matplotlib.widgets import Slider, Button, RadioButtons
import argparse

def save_image(image_data, title):
  matplotlib.image.imsave(title + '.png', image_data, cmap='gray', format = 'png')

def get_dft_shift(image_data):
  dft = cv2.dft(np.float32(image_data),flags = cv2.DFT_COMPLEX_OUTPUT)
  dft_shift = np.fft.fftshift(dft)
  return dft_shift

def get_max_frequency(image_data):
  rows, cols = image_data.shape
  return ((rows/2)**2 + (cols/2)**2)**0.5

def get_ideal_filter(image_data, low, high):
  max_frequency = get_max_frequency(image_data)
  rows, cols = image_data.shape
  return get_idealibpf(rows, cols, max_frequency * low, max_frequency * high)
  
def get_gaussian_filter(image_data, low, high):
  max_frequency = get_max_frequency(image_data)
  rows, cols = image_data.shape
  return get_gaussianibpf(rows, cols, max_frequency * low, max_frequency * high)

def get_magnitude(complex_matrix):
  return cv2.magnitude(complex_matrix[:,:,0],complex_matrix[:,:,1])

def save_filter(filter, title):
  save_image(get_magnitude(filter), title)

def apply_filter(dft_shift, filter):
  fshift = np.multiply(dft_shift, filter)
  f_ishift = np.fft.ifftshift(fshift)
  img_back = cv2.idft(f_ishift)
  return get_magnitude(img_back)

# get gaussian high-pass filter
def get_gaussianhpf(rows, cols, cutoff):
  filter = np.ones((rows,cols,2),np.float32)
  for i in range(1, rows):
    for j in range(1, cols):
      dist = ((i-(rows/2.0))**2 + (j-(cols/2.0))**2)
      filter[i,j] = math.exp(-1.0*dist/(2.0*(cutoff**2)))
      filter[i,j] = 1.0 - filter[i,j]
  return filter

# get inverse gaussian band-pass filter
# frequency within the low ~ high band is filtered, not passed
def get_gaussianibpf(rows, cols, low, high):
  if (low == 0):
    return get_gaussianhpf(rows, cols, high)

  rows_half = (rows-1)/2.0
  cols_half = (cols-1)/2.0
  low_square = -2.0*(low**2)
  high_square = -2.0*(high**2)

  qrows = int((rows+1)/2)
  qcols = int((cols+1)/2)
  q0 = np.ones((qrows,qcols,2),np.float32)
  for i in range(0, qrows):
    for j in range(0, qcols):
      dist_square = (i-rows_half)**2 + (j-cols_half)**2
      q0[i,j] = 1.0 - math.exp(dist_square/high_square)*(1.0 - math.exp(dist_square/low_square))

  filter = np.ones((rows,cols,2),np.float32)
  filter[:qrows, :qcols] = q0
  filter[:qrows, cols-qcols:] = cv2.flip(q0, 1)
  filter[rows-qrows:, :qcols] = cv2.flip(q0, 0)
  filter[rows-qrows:, cols-qcols:] = cv2.flip(q0, -1)
  
  return filter

# get inverse ideal band-pass filter
# frequency within the low ~ high band is filtered, not passed
def get_idealibpf(rows, cols, low, high):
  filter = np.ones((rows,cols,2),np.float32)

  low = low**2
  high = high**2

  for i in range(1, rows):
    for j in range(1, cols):
      dist = ((i-(rows/2))**2 + (j-(cols/2))**2)
      if(dist < high and dist > low):
        filter[i,j] = 0.01
      else:
        filter[i,j] = 1

  return filter

class ImageProcessor(object):
  def __init__(self, image, low, high):
    self.image_name = image
    self.low = low
    self.high = high
    self.filter = None
    self.output = None
    self.filter_type = None
    self.padRows = None
    self.padCols = None

    original_input = cv2.imread(image,0)
    rows, cols = original_input.shape
    if(rows < cols):
      original_input = cv2.transpose(original_input)
      self.transposed = True
    else:
      self.transposed = False
      
    rows, cols = original_input.shape
    optimalRows = cv2.getOptimalDFTSize(rows)
    optimalCols = cv2.getOptimalDFTSize(cols)
    self.padRows = optimalRows - rows
    self.padCols = optimalCols - cols
    self.input = np.zeros((optimalRows,optimalCols))
    self.input[:rows,:cols] = original_input
    
    self.dftshift = get_dft_shift(self.input)

    plt.subplots_adjust(left=0, bottom=0.05, right=1, top=1, wspace=0, hspace=0)

    plt.subplot(131)
    plt.axis('off')
    plt.title('original image')
    plt.imshow(self.input, cmap = 'gray',interpolation='nearest')

    
    self.axslow = plt.axes([0.05, 0.01, 0.5, 0.02])
    self.slow = Slider(self.axslow, 'low', 0.01, 1, valinit=self.low)
    self.slow.on_changed(self.updateLow)

    self.axhigh = plt.axes([0.05, 0.03, 0.5, 0.02])
    self.shigh = Slider(self.axhigh, 'high', 0.01, 1, valinit=self.high)
    self.shigh.on_changed(self.updateHigh)

    self.slow.slidermax=self.shigh
    self.shigh.slidermin=self.slow


    self.axfilter = plt.axes([0.62, 0.01, 0.15, 0.04])
    self.rfilter = RadioButtons(self.axfilter, ('Gaussian Filter', 'Ideal Filter'))
    self.rfilter.on_clicked(self.updateFilterType)
    self.filter_type = self.rfilter.value_selected
    
    self.axprocess = plt.axes([0.8, 0.01, 0.08, 0.04])
    self.bprocess = Button(self.axprocess, 'Process')
    self.bprocess.on_clicked(self.process)

    self.axsave = plt.axes([0.88, 0.01, 0.08, 0.04])
    self.bsave = Button(self.axsave, 'Save')
    self.bsave.on_clicked(self.save)
    
  def updateFilter(self):
    print "create " + self.filter_type
    if(self.filter_type == 'Ideal Filter'):
      self.filter = get_ideal_filter(self.input, self.low, self.high)
    elif(self.filter_type == 'Gaussian Filter'):
      self.filter = get_gaussian_filter(self.input, self.low, self.high)
    else:
      raise RuntimeError('unknown filter type ' + self.filter_type)

    plt.subplot(133)
    plt.axis('off')
    plt.title('DFT filter')
    plt.imshow(get_magnitude(self.filter), cmap = 'gray')
    plt.draw()

  def process(self, unused):
    self.updateFilter()
    
    print "apply filter"
    self.output = apply_filter(self.dftshift, self.filter)
    plt.subplot(132)
    plt.axis('off')
    plt.title('processed image')
    plt.imshow(self.output, cmap = 'gray')
    plt.draw()

    print "process complete"

  def updateLow(self, val):
    self.low = round(self.slow.val, 2)
    
  def updateHigh(self, val):
    self.high = round(self.shigh.val, 2)

  def updateFilterType(self, val):
    self.filter_type = self.rfilter.value_selected
    
  def save(self, unused):
    filter_file_title = os.path.splitext(self.image_name)[0] + '_' + self.filter_type + '_' + str(self.low)+'_'+str(self.high)+'_filter'
    processed_file_title = os.path.splitext(self.image_name)[0] + '_' + self.filter_type + '_' + str(self.low)+'_'+str(self.high)
    
    if(self.padRows != 0 or self.padCols != 0):
      rows, cols = self.output.shape
      processed = self.output[:rows-self.padRows, :cols-self.padCols]
    else:
      processed = self.output
    
    if(self.transposed):
      save_filter(cv2.transpose(self.filter), filter_file_title)
      save_image(cv2.transpose(processed), processed_file_title)
    else:
      save_filter(self.filter, filter_file_title)
      save_image(processed, processed_file_title)
    
  def run(self):
    mng = plt.get_current_fig_manager()
    mng.window.state('zoomed')
    plt.show()

if __name__ == '__main__':
  parser = argparse.ArgumentParser()
  parser.add_argument('image', type=str, help='path of the image to be processed')
  parser.add_argument('low', type=float, help='initial low bound of DFT filter, between 0.01 and 1, must be smaller than the initial high bound')
  parser.add_argument('high', type=float, help='initial high bound of DFT filter, between 0.01 and 1, must be greater than the initial high bound')
  args = parser.parse_args()
  
  if(args.low > args.high or args.low < 0.01 or args.low > 1 or args.high < 0.01 or args.high > 1):
    parser.print_help()
    sys.exit()

  imageProcessor = ImageProcessor(args.image, args.low, args.high)
  imageProcessor.run()