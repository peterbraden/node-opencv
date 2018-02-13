import {Contours} from './Contours';
import {BoundRect, Color} from './utils';

type ThresholdType =
  'Binary'
  | 'Binary Inverted'
  | 'Threshold Truncated'
  | 'Threshold to Zero'
  | 'Threshold to Zero Inverted';

type ThresholdAlgorithm = 'Otsu'; // Others?

export declare class Matrix {
  /**
   * Creates a new image with the specified properties.
   * @param {number} rows Number of rows on matrix.
   * @param {number} cols Number of cols on matrix.
   * @param {number} type ???
   * @param {number} fillValue ???
   */
  constructor(rows: number, cols: number, type?: number, fillValue?: number);

  /**
   * Creates a new matrix of ones (White).
   * @param rows Image height.
   * @param cols Image width.
   */
  static Ones(rows: number, cols: number): Matrix;

  /**
   * Creates a new matrix of zeros (Black).
   * @param rows Image height.
   * @param cols Image width.
   */
  static Zeros(rows: number, cols: number): Matrix;

  /**
   * Gets a thresholded image (This needs to be a single channel image).
   * @param maxVal Value that pixels are set to if the conditions are satisfied.
   * @param adaptiveMethod Which method to use:
   * ADAPTIVE_THRESH_MEAN_C = 0,
   * ADAPTIVE_THRESH_GAUSSIAN_C = 1.
   * @param thresholdType 0 for binary or 1 for binary inverted.
   * @param blockSize Area around the pixel for comparing against (odd).
   * @param C Value subtracted from mean of every pixel.
   * @returns The thresholded image.
   */
  adaptiveThreshold(maxVal: number, adaptiveMethod: 0 | 1, thresholdType: 0 | 1, blockSize: number, C: number): Matrix;

  /**
   * TODO: IDK what it does, help!
   * @param unknown1
   * @param unknown2
   */
  brightness(alpha: number, beta: number): void;

  /**
   * Performs Canny's border detection with the specified threshold values.
   * @param lowThreshold Low threshold to consider.
   * @param highThreshold High threshold to consider.
   */
  canny(lowThreshold: number, highThreshold: number): void;

  /**
   * Converts this image to grayscale.
   */
  convertGrayscale(): void;

  /**
   * Converts this image to HSV (Hue, Saturation, Value) scale.
   */
  convertHSVscale(): void;

  /**
   * Copies this image into a new Matrix.
   * @returns A new image copied from this.
   */
  copy(): Matrix;

  /**
   * Crops the image at (x, y) with a size of width * height.
   * @param x Horizontal coordinate.
   * @param y Vertical coordinate.
   * @param width Horizontal crop size.
   * @param height Vertical crop size.
   * @returns A cropped image of this.
   */
  crop(x: number, y: number, width: number, height: number): Matrix;

  /**
   * Detects all objects matching what xml file describe.
   * @param xmlPath Path for xml file which describes patterns to search.
   * @param options
   * @param cb To be call after detection gets completed.
   */
  detectObject(xmlPath: string, options: any, cb: (err: Error, objects: BoundRect) => void): void;

  /**
   * TODO: Not sure about this description.
   * Performs iteration dilations using the given structuring element for
   * dilation.
   * @param iterations The number of times to iterate.
   * @param structEl The structuring element for dilation (default 3x3).
   */
  dilate(iterations: number, structEl?: any): void;

  /**
   * Draws on this image a set of given contours or one of the contours from
   * the given set..
   * @param contours contours wanted to be draw.
   * @param idx If provided, this will draw only idx-th contour.
   * @param color Color for the set.
   * @param thickness Contour thickness.
   * @param lineType Type of line to draw contour.
   */
  drawContour(contours: Contours, idx?: number, color?: Color, thickness?: number, lineType?: any): void;

  /**
   * Draws an ellipse on this image at (x, y) with a width and height.
   * @param x X coordinate of ellipse's center.
   * @param y Y coordinate of ellipse's center.
   * @param width Horizontal size.
   * @param height Vertical size.
   */
  ellipse(x: number, y: number, width: number, height: number, color?: Color, thickness?: number): void;

  /**
   * Equalizes this image data.-
   */
  equalizeHist(): void;

  /**
   * Finds all contours on a given image. It is prefferable to have executed
   * an edge detection algorithm before.
   * @returns The contours on image.
   */
  findContours(): Contours;

  /**
   * Performs a gaussian blur on this image.
   * @param maskSize Mask's size to use for this blur. The size must be
   * specified into an array as [width, height].
   */
  gaussianBlur(maskSize: number[]): void;

  /**
   * TODO: Not sure about this description.
   * Performs iteration erotions using the given structuring element for
   * erotion.
   * @param iterations The number of times to iterate.
   * @param structEl The structuring element for erotion (default 3x3).
   */
  erode(iterations: number, structEl?: any): void;

  /**
   * Gets this image's height.
   * @returns This image's height.
   */
  height(): number;

  /**
   * Draws a line from src to dst.
   * @param src [x, y] coordinates from source point.
   * @param dst [x, y] coordinates from destination point.
   * @param color [r, g, b] Line color.
   */
  line(src: number[], dst: number[], color: number[]): void;

  /**
   * Gets the intensities at (x, y).
   * @param x Coordinate
   * @param y Coordinate
   * @returns The BGR intensities at (x, y).
   */
  pixel(x: number, y: number): Color | number;

  /**
   * Saves this images in the specified path.
   * @param path Path to store image.
   * @param cb Callback to be executed when saving is done.
   */
  save(path: string, cb?: (err: Error, code: any) => void): void;

  /**
   * Splits this into 3 channels.
   * @returns A BGR array.
   */
  split(): Matrix[];

  /**
   * Performs a threshold on this,
   * @param low Low threshold.
   * @param high High threshold.
   * @param type Threshold type to perform.
   * @param algorithm Threshold type to perform.
   * @returns The thresholded image.
   */
  threshold(low: number, high: number, type?: ThresholdType, algorithm?: ThresholdAlgorithm): Matrix;

  /**
   * Transforms this image into a buffer.
   * @returns A buffer representing this.
   */
  toBuffer(): Buffer;

  /**
   * Gets this image's width.
   * @returns This image's width.
   */
  width(): number;
}
