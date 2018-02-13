import {BoundRect} from './utils';

interface HuMoments {
    m00: number,
    m01: number,
    m10: number,
    m11: number
}

/**
 * Defines contours class. It will store information about the contours inside
 * an image.
 */
export declare class Contours {
    /**
     * I don't know if this actually has a constructor.
     */
    constructor();

    /**
     * Gets the perimeter of the area surrounded by the i-th contour.
     * @param i Contour requested.
     * @param closed Checks if its a closed contour or just a curve.
     * @returns Contour length.
     */
    arcLength(i: number, closed?: boolean): number;

    /**
     * Gets the area of the i-th contour.
     * @param i Element from which the area is wanted.
     * @returns The area of the i-th contour.
     */
    area(i: number): number;

    /**
     * Gets a rectangle which surrounds all of the i-th contour.
     * @param i Element from which the bounding rect is wanted.
     * @returns A bounding rectangle containing the i-th contour.
     */
    boundingRect(i: number): BoundRect;

    /**
     * Counts the number of corners on i-th contour.
     * @param i Contour wanted.
     * @returns Number of corners of i-th contour.
     */
    cornerCount(i: number): number;

    /**
     * Gets moments from the i-th contour.
     * @param i Index of the contour requested.
     */
    moments(i: number): HuMoments;

    /**
     * Gets the set of points composing the i-th countour.
     * @param i Countour requested.
     * @returns Each point on the i-th countour.
     */
    points(i: number): { x: number; y: number; }[];

    /**
     * Gets the number of contours on this.
     * @returns The number of contours on this.
     */
    size(): number;
}
