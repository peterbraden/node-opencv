cv = require 'opencv'

# First we create a new VideoCapture object to get
# video from the camera (0 for default camera)
camera = new cv.VideoCapture(1)

# we create a window to display the Video frames
namedWindow = new cv.NamedWindow('Video',1)

# We set an interval to retrieve frames from the
# video source and we get the intervalId so we can
# stop the program from the video feed window.
intervalId = setInterval(()->

  # camera.read allows us to retrieve the current
  # frame to be displayed in the video window.
  camera.read((err, im) ->
    # We can check for errors and even break the
    # program execution if an error is detected here.
    console.log "The err ==>#{ err }" if err

    # There is no need to display the image width or Height
    # but I leave this here in case anyone needs to check them.
    #console.log("Width: #{im.width()}")
    #console.log("height: #{im.height()}")

    # We use the previously created namedWindow to display the
    # video feed frame, we need to check the image is already
    # available and has a width and height greater than 0,
    # otherwise namedWindow.show() will fail if any of those is 0.
    namedWindow.show(im) if im.width() > 0 and im.height() > 0

    # Finally we get the key pressed on the window to terminate
    # execution of the program.
    res = namedWindow.blockingWaitKey(0, 20)
    console.log("KEYPRESSED => #{ res } ")

    # In this case I terminate the program if any key is pressed.
    if res >= 0 then clearInterval(intervalId)
  )
, 500)
