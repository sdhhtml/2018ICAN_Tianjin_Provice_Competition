# Object tracking with keypoints example.
# Show the camera an object and then run the script. A set of keypoints will be extracted
# once and then tracked in the following frames. If you want a new set of keypoints re-run
# the script. NOTE: see the docs for arguments to tune find_keypoints and match_keypoints.
import sensor, time, image
from pyb import UART
import json
# Reset sensor
sensor.reset()

# Sensor settings
sensor.set_contrast(3)
sensor.set_gainceiling(16)
sensor.set_framesize(sensor.VGA)
sensor.set_windowing((320, 240))
sensor.set_pixformat(sensor.GRAYSCALE)

sensor.skip_frames(time = 2000)
sensor.set_auto_gain(False, value=100)

def draw_keypoints(img, kpts):
    if kpts:
        print(kpts)
        img.draw_keypoints(kpts)
        img = sensor.snapshot()
        time.sleep(1000)
uart = UART(3, 115200)

kpts1 = image.load_descriptor("/yxxjqr1.orb")
kpts2 = image.load_descriptor("/yxxjqr2.orb")
kpts3 = image.load_descriptor("/yxxjqr3.orb")
kpts4 = image.load_descriptor("/yxxjqr4.orb")
kpts5 = image.load_descriptor("/yxxjqr5.orb")
img = sensor.snapshot()
draw_keypoints(img, kpts1)

clock = time.clock()
while (True):
    clock.tick()
    img = sensor.snapshot()
    kpts10 = img.find_keypoints(max_keypoints=150, threshold=10, normalized=True)
    if (kpts10):
        match = image.match_descriptor(kpts1, kpts10, threshold=85)
        if (match.count()>10):
                # If we have at least n "good matches"
                # Draw bounding rectangle and cross.
            img.draw_rectangle(match.rect())
            img.draw_cross(match.cx(), match.cy(), size=10)
            output_str="[%d,%d]" % (match.cx(),match.cy())
            uart.write(output_str+'\r\n')
        print(kpts1, "matched:%d dt:%d"%(match.count(), match.theta()))
            # NOTE: uncomment if you want to draw the keypoints
            #img.draw_keypoints(kpts2, size=KEYPOINTS_SIZE, matched=True)

    # Draw FPS
    #img.draw_string(0, 0, "FPS:%.2f"%(clock.fps()))
