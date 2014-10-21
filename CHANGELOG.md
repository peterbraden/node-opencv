
# Changelog

### 1.0.0

Ok, let's do a proper semver release :)

The big news in this release is that thanks to some amazing work by
@kaosat-dev, node-opencv now works with node 0.11.

There's also some general tidying up, including the examples by marcbachmann

Thanks all!

#### 0.7.0

Matrix constructors, and contour access from @oskardahlberg and @emallson.

#### 0.6.0

Many updates from the community, thank you to all.

Especially: @oskardahlberg, @salmanulhaq, @jcao75, @psayre23, @jhludwig
 , @coolblade, @ytham, @morganrallen and anyone I inadvertantly missed.


#### 0.5.0

Lots more opencv functions added, and bugfixes from a large number of
contributors. Thanks to all of them!

- Allow args for `HoughLinesP` by @clkao in #112
- `matchTemplate` and `minMaxLoc` by @ytham in #108
- updated `blockingWaitKey` by @hybridgroup in #98


#### 0.0.13 -> 0.4.0

( missing description... )

#### 0.0.13

- V Early support for face recognition - API is _likely_ to change. Have fun!
- *API Change*: VideoCapture.read now calls callback(err, im) instead of callback(im)

#### 0.0.12
- Matrix clone()
- NamedWindow Support

#### 0.0.11

- Bug Fixes
- ImageStream becomes ImageDataStream, and new ImageStream allows multiple images to be
streamed as matrices, for example, with an object detection stream.
- @ryansouza improved documentation
- Correcting matrix constructor (thanks @gluxon)
- @Michael Smith expanded Contours functionality.

Thanks all!

#### 0.0.10

- Bug Fixes
- @Contra added code that allows thickness and color args for ellipse
- Camshift Support
- @jtlebi added bindings for erode, gaussianBlur, arcLength, approxPolyDP, isConvex, cornerCount
- @gluxon added bindings for inRange

Thanks everyone!

#### 0.0.9

- toBuffer can now take a callback and be run async (re #21)
