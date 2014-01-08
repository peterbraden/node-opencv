cv = require('opencv')


cv.readImage("./images/windows-logo.png", (err, im) ->

  # The nodejs-opencv has some shortcodes for image manipulation.
  # We modify the image using the handy function im.convertGrayscale()
  # This will overwrite the current image with a grayscaled version.
  im.convertGrayscale()

  # Create new NamedWindow object to hold the image
  # NamedWindow takes two arguments String WindowName and String windowSize
  namedWindow = new cv.NamedWindow('Display Window', '400x400')

  # We then tell the image to show the image we loaded.
  namedWindow.show(im)

  console.log("Image should be displayed inside a window.")
  # Finally we tell the NamedWindow to wait for any key being pressed to close
  # itself (by passing a 0 as the first param, or wait a defined amount of time
  # by passing the time as a second argument (in milliseconds)
  #
  # If we do not tell the window to wait it will just load and show the image
  # and close so fast that it will appear nothing happened.
  namedWindow.blockingWaitKey(0, 5000)

  console.log("And the window should close automatically or by pressing any key on it.")
)
