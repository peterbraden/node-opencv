// import {Contours} from './Contours';
import {Matrix} from './Matrix';

export declare const FACE_CASCADE: string;
export declare const FULLBODY_CASCADE: string;
export declare const CAR_SIDE_CASCADE: string;
export declare const EYE_CASCADE: string;
export declare const EYEGLASSES_CASCADE: string;

export const CV_TRESHOLD_TO_ZERO = 'Threshold to Zero';
export const CV_THRESHOLD_BINARY = 'Binary';
export const CV_THRESHOLD_BINARY_INVERTED = 'Binary Inverted';
export const CV_THRESHOLD_TRUNCATED = 'Threshold Truncated';
export const CV_TRESHOLD_TO_ZERO_INVERTED = 'Threshold to Zero Inverted';

export interface BoundRect {
  x: number,
  y: number,
  width: number,
  height: number
}

export type Color = number[];

/**
 * Reads the image at path.
 * @param path Image path.
 * @param cb Callback to be executed after reading completes.
 */
export function readImage(path: string, cb: (err: Error, img: Matrix) => void): void;
