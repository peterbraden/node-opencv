cv = require 'opencv'

# we create a function helper to display the images.
displayImage = (windowName, img) ->
  namedWindow = new cv.NamedWindow(windowName)
  namedWindow.show(img)
  namedWindow.blockingWaitKey(0, 5000)

# Load both sources to be blended
cv.readImage("./images/windows-logo.png", (err, src1) ->

  # Display Source 1
  displayImage('Source 1', src1)

  # Load source #2
  cv.readImage("./images/linux-logo.png", (err, src2) ->

    # Display Source 2
    displayImage('Source 2', src2)

    # We use the addWeighted opencv wrapper to blend the images.
    # we need to use a different image object, since src1 and src2
    # will be manipulated.
    result = new cv.Matrix(src1.width(), src2.height())
    result.addWeighted(src1, 0.7, src2, 0.9)
    result.save("./tmp/blended.png")

    # Display Blended result 2
    displayImage('Blended', result)
  )
)
